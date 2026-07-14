// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ArchiveHandler.h"

#include <KCompressionDevice>
#include <KZip>
#include <K7Zip>
#include <KTar>

#include <QFileInfo>
#include <QMimeDatabase>

ArchiveHandler::ArchiveHandler(QObject *parent, MakiImageProvider *imageProvider)
: PreviewHandler(parent)
, m_imageProvider(imageProvider)
{}

bool ArchiveHandler::canHandle(const QMimeType &mime) const 
{
    return mime.inherits(QStringLiteral("application/zip"))
        || mime.inherits(QStringLiteral("application/zip-compressed"))
        || mime.inherits(QStringLiteral("application/x-zip-compressed"))
        || mime.inherits(QStringLiteral("application/7z-compressed"))
        || mime.inherits(QStringLiteral("application/x-tar"))
        || mime.inherits(QStringLiteral("application/x-gtar"))
        || mime.inherits(QStringLiteral("application/x-compressed-tar"))
        || mime.inherits(QStringLiteral("application/x-bzip-compressed-tar"))
        || mime.inherits(QStringLiteral("application/x-xz-compressed-tar"))
        //Raw compressed files
        || mime.inherits(QStringLiteral("application/gzip"))
        || mime.inherits(QStringLiteral("application/x-gzip"))
        || mime.inherits(QStringLiteral("application/x-bzip"))
        || mime.inherits(QStringLiteral("application/x-bzip2"))
        || mime.inherits(QStringLiteral("application/x-lzip"))
        || mime.inherits(QStringLiteral("application/x-lzma"))
        || mime.inherits(QStringLiteral("application/x-zstd"))
        || mime.inherits(QStringLiteral("application/x-xz"));
}

void ArchiveHandler::process(const QUrl &url, const QMimeType &mime)
{
    m_fileName = url.fileName();
    Q_EMIT fileNameChanged();

    if (KArchive *archive = createArchive(url.toLocalFile())) {
        QFileInfo fileInfo(url.toLocalFile());

        if (fileInfo.exists() && fileInfo.isFile()) {
            m_fileSize = QLocale::system().formattedDataSize(fileInfo.size(), 1);
            Q_EMIT fileSizeChanged();
        }

        if (!m_archiveModel) {
            m_archiveModel = new QStandardItemModel(this);
        }

        if (archive->open(QIODevice::ReadOnly)) {
            m_archiveModel->clear();
            
            populateArchiveModel(m_archiveModel->invisibleRootItem(), archive->directory());

            if (m_imageProvider) {
                m_imageProvider->setInfoPanelImage(QIcon::fromTheme(mime.iconName()).pixmap(128, 128));
                m_infoPanelUrl = QUrl(QStringLiteral("image://Maki/infoPanel?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
                Q_EMIT infoPanelUrlChanged();
            }

            archive->close();
        }

        delete archive;
        archive = nullptr;

        Q_EMIT archiveModelChanged();
    } else if (KCompressionDevice::compressionTypeForMimeType(mime.name()) != KCompressionDevice::None) {
        //Raw compressed file, extract without KArchive
        auto compType = KCompressionDevice::compressionTypeForMimeType(mime.name());

        QMimeDatabase db;

        KCompressionDevice compDevice(url.toLocalFile(), compType);
        
        if (!m_archiveModel) {
            m_archiveModel = new QStandardItemModel(this);
        }

        m_archiveModel->clear();
        
        QFileInfo fileInfo(url.toLocalFile());
        m_fileSize = QLocale::system().formattedDataSize(fileInfo.size(), 1);
        Q_EMIT fileSizeChanged();

        if (m_imageProvider) {
            m_imageProvider->setInfoPanelImage(QIcon::fromTheme(mime.iconName()).pixmap(128, 128));
            m_infoPanelUrl = QUrl(QStringLiteral("image://Maki/infoPanel?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
            Q_EMIT infoPanelUrlChanged();
        }
        
        QString innerFileName = fileInfo.completeBaseName();
        
        auto *item = new QStandardItem(innerFileName);
        item->setText(innerFileName);
        
        if (compDevice.open(QIODevice::ReadOnly)) {
            QByteArray decompressedData = compDevice.readAll();
            compDevice.close();
            
            QMimeType innerMime;
            QFileInfo innerFileInfo(innerFileName);

            if (!innerFileInfo.suffix().isEmpty()) {
                //Suffix is there, use it to get mime type
                innerMime = db.mimeTypeForFile(innerFileName);
            } else {
                //pray that mimeTypeForData is accurate
                innerMime = db.mimeTypeForData(decompressedData);
            }
            
            item->setData(innerMime.iconName(), Qt::ToolTipRole);
            item->setData(QLocale::system().formattedDataSize(decompressedData.size(), 1), Qt::StatusTipRole);
        }

        m_archiveModel->appendRow(item);

        Q_EMIT archiveModelChanged();
    }
}

QString ArchiveHandler::fileTypeKey() const 
{
    return QStringLiteral("archive");
}

void ArchiveHandler::clearState()
{
    
}

QString ArchiveHandler::fileSize() const
{
    return m_fileSize;
}

QString ArchiveHandler::fileName() const
{
    return m_fileName;
}

QUrl ArchiveHandler::infoPanelUrl() const
{
    return m_infoPanelUrl;
}

QStandardItemModel* ArchiveHandler::archiveModel() const
{
    return m_archiveModel;
}

KArchive* ArchiveHandler::createArchive(const QString &filePath)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath);
    
    if (mime.inherits(QStringLiteral("application/zip"))
    ||  mime.inherits(QStringLiteral("application/zip-compressed"))
    ||  mime.inherits(QStringLiteral("application/x-zip-compressed"))) {
        return new KZip(filePath);
    }
    
    if (mime.inherits(QStringLiteral("application/x-7z-compressed"))) {
        return new K7Zip(filePath);
    }
    
    if (mime.inherits(QStringLiteral("application/x-tar"))
    ||  mime.inherits(QStringLiteral("application/x-gtar"))
    ||  mime.inherits(QStringLiteral("application/x-compressed-tar"))
    ||  mime.inherits(QStringLiteral("application/x-bzip-compressed-tar"))
    ||  mime.inherits(QStringLiteral("application/x-xz-compressed-tar"))) {
        return new KTar(filePath);
    }

    return nullptr;
}

void ArchiveHandler::populateArchiveModel(QStandardItem *parent, const KArchiveDirectory *dir)
{
    QMimeDatabase db;
    for (const QString &name : dir->entries()) {
        const KArchiveEntry *entry = dir->entry(name);
        auto *item = new QStandardItem(name);

        const QString iconName = entry->isDirectory()
            ? QStringLiteral("inode-directory")
            : QString(db.mimeTypeForFile(name, QMimeDatabase::MatchExtension).iconName());

        item->setData(iconName, Qt::ToolTipRole);
        item->setText(name);

        if (entry->isDirectory()) {
            const KArchiveDirectory *dir = static_cast<const KArchiveDirectory*>(entry);
            item->setData(QLocale::system().formattedDataSize(getDirSize(dir), 1), Qt::StatusTipRole);
            populateArchiveModel(item, dir);
        } 
        
        if (entry->isFile()) {
            const auto *file = static_cast<const KArchiveFile *>(entry);
            item->setData(QLocale::system().formattedDataSize(file->size(), 1), Qt::StatusTipRole);
        }
        
        parent->appendRow(item);
    }
}

qint64 ArchiveHandler::getDirSize(const KArchiveDirectory *dir)
{
    if (!dir) {
        return 0;
    }

    qint64 size{0};

    for (const QString &entryName : dir->entries())
    {
        const KArchiveEntry *entry = dir->entry(entryName);

        if (entry->isFile()) {
            const KArchiveFile *file = static_cast<const KArchiveFile *>(entry);
            if (file) {
                size += file->size();
            }
        } else if (entry->isDirectory()) {
            size += getDirSize(static_cast<const KArchiveDirectory*>(entry));
        }
    }

    return size;
}