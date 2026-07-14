// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WindowManager.h"

#include "ImageHandler.h"
#include "TextHandler.h"
#include "AudioHandler.h"
#include "VideoHandler.h"
#include "DocumentHandler.h"
#include "ArchiveHandler.h"

#include <KIO/OpenUrlJob>
#include <KIO/JobUiDelegateFactory>

#include <QIcon>
#include <QMimeType>

WindowManager::WindowManager(QObject *parent)
: QObject(parent)
{}

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

    setVisible(true);

    QMimeType mime = db.mimeTypeForUrl(url);

    for (auto &handler : m_handlers) {

        if (handler->canHandle(mime)) {
            m_activeHandler = handler.get();
            Q_EMIT activeHandlerChanged();
            handler->process(url, mime);
            setFileType(handler->fileTypeKey());
            
            return;
        }    
    }
    
    //Fallback to generic
    QIcon icon = QIcon::fromTheme(mime.iconName());

    if (m_imageProvider) {
        // TODO: Should this be in a config?
        m_imageProvider->setFallbackImage(icon.pixmap(128, 128));

        m_fallbackURL = QUrl(QStringLiteral("image://Maki/fallback"));
        Q_EMIT fallbackURLChanged();
    }

    setFileType(QStringLiteral("None"));
}

QUrl WindowManager::getFallbackURL() const 
{
    return m_fallbackURL;
}

QString WindowManager::getFileName() const
{
    return m_url.fileName();
}

QString WindowManager::getFileType() const
{
    return m_fileType;
}

void WindowManager::setFileType(const QString &fileType)
{
    m_fileType = fileType;
    Q_EMIT fileTypeChanged();
}

PreviewHandler* WindowManager::activeHandler() const
{
    return m_activeHandler;
}

void WindowManager::openInDefaultApp()
{
    auto *job = new KIO::OpenUrlJob(m_url);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));

    connect(job, &KJob::result, this, [this](KJob *job) {
        if (job->error() == 0)
        {
            m_app->quit();
        }
    });

    job->start();
}

void WindowManager::releaseResources()
{
    m_fallbackURL = QUrl();
    m_fileType = QString();
    if (m_imageProvider) {
        m_imageProvider->releaseResources();
    }
    
    Q_EMIT fallbackURLChanged();
    Q_EMIT fileTypeChanged();
}

void WindowManager::initHandlers()
{
    m_handlers.push_back(std::make_unique<ImageHandler>(this, m_imageProvider));
    m_handlers.push_back(std::make_unique<TextHandler>(this));
    m_handlers.push_back(std::make_unique<AudioHandler>(this, m_imageProvider));
    m_handlers.push_back(std::make_unique<VideoHandler>(this));
    m_handlers.push_back(std::make_unique<DocumentHandler>(this, m_imageProvider));
    m_handlers.push_back(std::make_unique<ArchiveHandler>(this, m_imageProvider));
}

void WindowManager::createFallbackImage(QMimeType mime)
{
    QIcon icon = QIcon::fromTheme(mime.iconName());

    if (m_imageProvider) {
        // TODO: Should this be in a config?
        m_imageProvider->setFallbackImage(icon.pixmap(512, 512));

        m_fallbackURL = QUrl(QStringLiteral("image://Maki/fallback"));
        Q_EMIT fallbackURLChanged();
    }

    setFileType(QStringLiteral("None"));
}