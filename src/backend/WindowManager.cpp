// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WindowManager.h"

#include "ArchiveHandler.h"
#include "AudioHandler.h"
#include "DocumentHandler.h"
#include "FallbackHandler.h"
#include "ImageHandler.h"
#include "KritaHandler.h"
#include "TextHandler.h"
#include "VideoHandler.h"

#include <KApplicationTrader>
#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenUrlJob>
#include <KService>

#include <QIcon>
#include <QMimeType>

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
{
}

void WindowManager::initApp(QApplication *app)
{
    m_app = app;
}

void WindowManager::setImageProvider(MakiImageProvider *provider)
{
    m_imageProvider = provider;
    initHandlers();
}

bool WindowManager::getVisible() const
{
    return m_visible;
}

void WindowManager::setVisible(bool visible)
{
    m_visible = visible;
    Q_EMIT visibleChanged();
}

QUrl WindowManager::getURL() const
{
    return m_url;
}

void WindowManager::setURL(const QUrl &url)
{
    releaseResources();

    m_url = url;
    Q_EMIT urlChanged();
    Q_EMIT fileNameChanged();

    setVisible(true);

    QMimeType mime = db.mimeTypeForUrl(url);

    KService::Ptr service = KApplicationTrader::preferredService(mime.name());

    if (service) {
        m_defaultAppName = service->name();
        Q_EMIT defaultAppNameChanged();

        QString defaultAppIcon = service->icon();
        if (defaultAppIcon.isEmpty()) {
            m_defaultAppIcon = mime.iconName();
        } else {
            m_defaultAppIcon = defaultAppIcon;
        }
        Q_EMIT defaultAppIconChanged();
    }

    for (auto &handler : m_handlers) {
        if (handler->canHandle(mime)) {
            m_activeHandler = handler.get();
            handler->process(url, mime);
            m_fileKey = handler->fileTypeKey();
            Q_EMIT fileKeyChanged();
            // Emit this signal last so you don't get warnings about assigning the wrong handler
            Q_EMIT activeHandlerChanged();

            return;
        }
    }
}

QString WindowManager::getFileName() const
{
    return m_url.fileName();
}

QString WindowManager::defaultAppName() const
{
    return m_defaultAppName;
}

QString WindowManager::defaultAppIcon() const
{
    return m_defaultAppIcon;
}

HandlerUtils::HandlerKey WindowManager::fileKey() const
{
    return m_fileKey;
}

PreviewHandler *WindowManager::activeHandler() const
{
    return m_activeHandler;
}

void WindowManager::openInDefaultApp()
{
    auto *job = new KIO::OpenUrlJob(m_url);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));

    connect(job, &KJob::result, this, [this](KJob *job) {
        if (job->error() == 0) {
            releaseResources();
            setVisible(false);
        }
    });

    job->start();
}

void WindowManager::releaseResources()
{
    if (m_imageProvider) {
        m_imageProvider->releaseResources();
    }

    for (auto &handler : m_handlers) {
        handler->clearState();
    }
}

void WindowManager::initHandlers()
{
    m_handlers.push_back(std::make_unique<ImageHandler>(this, m_imageProvider));
    m_handlers.push_back(std::make_unique<KritaHandler>(this, m_imageProvider));
    m_handlers.push_back(std::make_unique<TextHandler>(this));
    m_handlers.push_back(std::make_unique<AudioHandler>(this, m_imageProvider));
    m_handlers.push_back(std::make_unique<VideoHandler>(this));
    m_handlers.push_back(std::make_unique<DocumentHandler>(this, m_imageProvider));
    // ArchiveHandler needs to be after DocumentHandler to ensure proper handling of Office files
    // Since the inherit from zip archive mimes, the ArchiveHandler would be able to handle it
    m_handlers.push_back(std::make_unique<ArchiveHandler>(this, m_imageProvider));
    // FallbackHandler always evaluates to canHandle() == true, so it has to be last
    m_handlers.push_back(std::make_unique<FallbackHandler>(this));
}