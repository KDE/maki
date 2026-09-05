// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ImageHandler.h"

#include <KZip>

#include <QDateTime>
#include <QPixmap>

ImageHandler::ImageHandler(QObject *parent, MakiImageProvider *imageProvider)
: PreviewHandler(parent)
, m_imageProvider(imageProvider)
{}

bool ImageHandler::canHandle(const QMimeType &mime) const 
{
    return (mime.name().startsWith(QStringLiteral("image/")) 
         || mime.inherits(QStringLiteral("application/x-krita")));
}

void ImageHandler::process(const QUrl &url, const QMimeType &mime)
{
    QPixmap image;

    if (mime.name().startsWith(QStringLiteral("image"))) {
        //Parse regular images
        image = QPixmap(url.toLocalFile());
    } else if (mime.inherits(QStringLiteral("application/x-krita"))) {
        //Parse Krita files
        image = QPixmap::fromImage(extractKritaImage(url.toLocalFile()));
    }
    
    image = image.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (m_imageProvider) {
        m_imageProvider->setPixmap(image);
    }

    m_imageUrl = QUrl(QStringLiteral("image://Maki/image?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
    m_imageSize = image.size();
    Q_EMIT imageUrlChanged();
    Q_EMIT imageSizeChanged();
}

QString ImageHandler::fileTypeKey() const 
{
    return QStringLiteral("image");
}

void ImageHandler::clearState()
{

}

QUrl ImageHandler::imageUrl() const
{
    return m_imageUrl;
}

QSize ImageHandler::imageSize() const
{
    return m_imageSize;
}

QImage ImageHandler::extractKritaImage(const QString &fileName) const
{
    KZip kritaArchive{ fileName };

    if (!kritaArchive.open(QIODevice::ReadOnly)) {
        return QImage();
    }

    if (!kritaArchive.directory()) {
        return QImage();
    }

    const KArchiveDirectory *rootDir { kritaArchive.directory() };

    if (const KArchiveEntry *mergedImageEntry{ rootDir->entry(QStringLiteral("mergedimage.png")) }) {
        //File is a .kra file, we can extract the mergedimage.png for higher resolution
        const KArchiveFile *imageFile = static_cast<const KArchiveFile *>(mergedImageEntry);
        
        if (!imageFile) {
            return QImage();
        }

        QByteArray imageData{ imageFile->data() };
        QImage mergedImage;

        if (mergedImage.loadFromData(imageData, "PNG")) {
            return mergedImage;
        }
    } else if (const KArchiveEntry *previewImageEntry = rootDir->entry(QStringLiteral("preview.png"))) {
        //File is a .krz file, fall back to the lower resolution preview.png file
        //Or if for whatever reason the mergedimage.png is missing, use preview.png instead
        const KArchiveFile *imageFile = static_cast<const KArchiveFile *>(previewImageEntry);
        
        if (!imageFile) {
            return QImage();
        }

        QByteArray imageData{ imageFile->data() };
        QImage previewImage;

        if (previewImage.loadFromData(imageData, "PNG")) {
            return previewImage;
        }
    }

    //if everything fails, the file is corrupted or not a krita file
    return QImage();
}