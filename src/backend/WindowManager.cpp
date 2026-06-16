// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WindowManager.h"

#include <KIO/OpenUrlJob>
#include <KIO/JobUiDelegateFactory>
#include <KZip>
#include <K7Zip>
#include <KTar>

#include <poppler-qt6.h>

#include <QFile>

#include <QIcon>

#include <QImageReader>

#include <QMimeType>

#include <QTimer>

#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , m_archiveModel(new QStandardItemModel(this))
{
}

void WindowManager::initApp(QApplication *app)
{
    m_app = app;
}

void WindowManager::setImageProvider(MakiImageProvider *provider)
{
    m_imageProvider = provider;
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

QSize WindowManager::getWindowSize() const
{
    return m_windowSize;
}

QUrl WindowManager::getURL() const
{
    return m_url;
}

void WindowManager::setURL(const QUrl &url)
{
    releaseResources();

    m_url = url;

    QMimeType mime = db.mimeTypeForUrl(url);

    if (mime.name().startsWith(QStringLiteral("image/"))) {
        setFileType(QStringLiteral("image"));
        if (m_imageProvider) {
            QPixmap image = QPixmap(url.toLocalFile());
            m_imageProvider->setPixmap(image);
            m_url = QUrl(QStringLiteral("image://Maki/image?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
            m_windowSize = image.size();
            Q_EMIT windowSizeChanged();
        }
        
    } else if (mime.name().startsWith(QStringLiteral("text/"))) {
        QFile file(url.toLocalFile());

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_fileText = QString::fromUtf8(file.readAll());
            Q_EMIT fileTextChanged();
        }

        setFileType(QStringLiteral("text"));
    } else if (mime.name().startsWith(QStringLiteral("audio/"))) {
        TagLib::MPEG::File file(url.toLocalFile().toUtf8().constData());

        if (file.isValid()) {
            // Extract all other metadata here

            // Extract the cover art
            bool imageFound = false;
            if (file.ID3v2Tag()) {
                auto frames = file.ID3v2Tag()->frameListMap()["APIC"];
                if (!frames.isEmpty()) {
                    auto *frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
                    if (frame) {
                        QImage cover;
                        cover.loadFromData(reinterpret_cast<const uchar *>(frame->picture().data()), frame->picture().size());
                        if (!cover.isNull()) {
                            if (m_imageProvider)
                                m_imageProvider->setAudioCover(QPixmap::fromImage(cover));
                            m_audioCoverURL = QUrl(QStringLiteral("image://Maki/audioCover?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
                            Q_EMIT audioCoverURLChanged();

                            imageFound = true;
                        }
                    }
                }
            }

            if (!imageFound) {
                QIcon icon = QIcon::fromTheme(mime.iconName());

                if (m_imageProvider)
                    m_imageProvider->setAudioCover(icon.pixmap(512, 512));
                m_audioCoverURL = QUrl(QStringLiteral("image://Maki/audioCover?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
                Q_EMIT audioCoverURLChanged();
                qWarning() << "fall back to generic";
            }
        }

        setFileType(QStringLiteral("audio"));
    } else if (mime.name().startsWith(QStringLiteral("video/"))) {
        setFileType(QStringLiteral("video"));
    } else if (mime.name() == QStringLiteral("application/pdf")) {
        std::unique_ptr<Poppler::Document> doc = Poppler::Document::load(url.toLocalFile());

        if (!doc) {
            return;
        }

        doc->setRenderHint(Poppler::Document::RenderHint::Antialiasing, true);
        doc->setRenderHint(Poppler::Document::RenderHint::TextAntialiasing, true);
        doc->setRenderHint(Poppler::Document::RenderHint::ThinLineSolid, true);

        int numPages = doc->numPages();
        m_pageNum = numPages;

        std::vector<QImage> pageImages;

        pageImages.reserve(numPages);

        for (int i = 0; i < numPages; i++) {
            std::unique_ptr<Poppler::Page> page = doc->page(i);

            if (page) {
                // TODO: DPI as config?
                pageImages.push_back(page->renderToImage(72, 72));
            }
        }

        // TODO: Currently sets the window size to the first page's size
        //       But if there is ever a bigger page in the document
        //       It will clip. Maybe set the size to the dimensions
        //       of the biggest pages? So the width will be the max
        //       width of all the pages and the height as well
        m_windowSize = pageImages[0].size();
        Q_EMIT windowSizeChanged();

        if (m_imageProvider) {
            m_imageProvider->setDocumentImages(pageImages);
        }

        Q_EMIT pageNumChanged();

        setFileType(QStringLiteral("pdf"));
    } else if (KArchive *archive = createArchive(url.toLocalFile())) {
        if (!m_archiveModel) {
            m_archiveModel = new QStandardItemModel(this);
        }

        if (archive->open(QIODevice::ReadOnly)) {
            m_archiveModel->clear();
            populateArchiveModel(m_archiveModel->invisibleRootItem(), archive->directory());

            if (m_imageProvider) {
                m_imageProvider->setInfoPanelImage(QIcon::fromTheme(mime.iconName()).pixmap(128, 128));
                m_infoPanelURL = QUrl(QStringLiteral("image://Maki/infoPanel?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
                Q_EMIT infoPanelURLChanged();
            }

            archive->close();
        }

        delete archive;
        archive = nullptr;

        setFileType(QStringLiteral("archive"));
        Q_EMIT fileTypeChanged();
        Q_EMIT archiveModelChanged();
    } else {
        QIcon icon = QIcon::fromTheme(mime.iconName());

        if (m_imageProvider) {
            // TODO: Should this be in a config?
            m_imageProvider->setFallbackImage(icon.pixmap(512, 512));

            m_fallbackURL = QUrl(QStringLiteral("image://Maki/fallback"));
            Q_EMIT fallbackURLChanged();
        }

        setFileType(QStringLiteral("None"));
    }

    setVisible(true);

    Q_EMIT urlChanged();
    Q_EMIT fileNameChanged();
}

QUrl WindowManager::getFallbackURL() const 
{
    return m_fallbackURL;
}

QUrl WindowManager::getAudioCoverURL() const
{
    return m_audioCoverURL;
}

QUrl WindowManager::getInfoPanelURL() const
{
    return m_infoPanelURL;
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

QString WindowManager::getFileText() const
{
    return m_fileText;
}

QStandardItemModel* WindowManager::getArchiveModel() const
{
    return m_archiveModel;
}

int WindowManager::getPageNum() const
{
    return m_pageNum;
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
    m_audioCoverURL = QUrl();
    m_fileType = QString();
    m_fileText = QString();
    if (m_imageProvider) {
        m_imageProvider->releaseResources();
    }
    if (m_archiveModel) {
        m_archiveModel->clear();
    }
    
    Q_EMIT fallbackURLChanged();
    Q_EMIT audioCoverURLChanged();
    Q_EMIT fileTypeChanged();
    Q_EMIT fileTextChanged();
}

KArchive* WindowManager::createArchive(const QString &filePath)
{
    QMimeType mime = db.mimeTypeForFile(filePath);
    
    if (mime.name() == QStringLiteral("application/zip")
    ||  mime.name() == QStringLiteral("application/zip-compressed")
    ||  mime.name() == QStringLiteral("application/x-zip-compressed")) {
        return new KZip(filePath);
    }
    
    if (mime.name() == QStringLiteral("application/x-7z-compressed")) {
        return new K7Zip(filePath);
    }
    
    if (mime.name() == QStringLiteral("application/x-gtar" )
    ||  mime.name() == QStringLiteral("application/x-gzip" )
    ||  mime.name() == QStringLiteral("application/x-tar"  )
    ||  mime.name() == QStringLiteral("application/x-bzip" )
    ||  mime.name() == QStringLiteral("application/x-bzip2")
    ||  mime.name() == QStringLiteral("application/x-xz"   )) {
        return new KTar(filePath);
    }

    return nullptr;
}

void WindowManager::populateArchiveModel(QStandardItem *parent, const KArchiveDirectory *dir)
{
    for (const QString &name : dir->entries()) {
        const KArchiveEntry *entry = dir->entry(name);
        auto *item = new QStandardItem(name);

        const QString iconName = entry->isDirectory()
            ? QStringLiteral("inode-directory")
            : QString(db.mimeTypeForFile(name, QMimeDatabase::MatchExtension).iconName());

        item->setData(iconName, Qt::ToolTipRole);
        item->setText(name);

        if (entry->isDirectory()) {
            populateArchiveModel(item, static_cast<const KArchiveDirectory*>(entry));
        }
        
        parent->appendRow(item);
    }
}
