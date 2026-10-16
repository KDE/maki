// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "KritaHandler.h"

#include <KZip>

#include <QDateTime>
#include <QDir>
#include <QPixmap>

KritaHandler::KritaHandler(QObject *parent)
    : ImageHandler(parent)
{
}

bool KritaHandler::canHandle(const QMimeType &mime) const
{
    return mime.inherits(QStringLiteral("application/x-krita"));
}

void KritaHandler::process(const QUrl &url, const QMimeType &mime)
{
    Q_UNUSED(mime);

    KZip kritaArchive{url.toLocalFile()};

    QImage image;

    if (!kritaArchive.open(QIODevice::ReadOnly)) {
        return;
    }

    if (!kritaArchive.directory()) {
        return;
    }

    const KArchiveDirectory *rootDir{kritaArchive.directory()};

    if (const KArchiveEntry *mergedImageEntry{rootDir->entry(QStringLiteral("mergedimage.png"))}) {
        // File is a .kra file, we can extract the mergedimage.png for higher resolution
        const KArchiveFile *imageFile = static_cast<const KArchiveFile *>(mergedImageEntry);

        if (!imageFile) {
            return;
        }

        QByteArray imageData{imageFile->data()};
        QImage mergedImage;

        if (mergedImage.loadFromData(imageData, "PNG")) {
            image = mergedImage;
        }
    } else if (const KArchiveEntry *previewImageEntry = rootDir->entry(QStringLiteral("preview.png"))) {
        // File is a .krz file, fall back to the lower resolution preview.png file
        // Or if for whatever reason the mergedimage.png is missing, use preview.png instead
        const KArchiveFile *imageFile = static_cast<const KArchiveFile *>(previewImageEntry);

        if (!imageFile) {
            return;
        }

        QByteArray imageData{imageFile->data()};
        QImage previewImage;

        if (previewImage.loadFromData(imageData, "PNG")) {
            image = previewImage;
        }
    }

    image = image.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_tempFile = std::make_unique<QTemporaryFile>(QDir::tempPath() + QStringLiteral("/krita_preview_XXXXXX.png"));
    if (m_tempFile->open()) {
        if (image.save(m_tempFile.get(), "PNG")) {
            m_imageUrl = QUrl::fromLocalFile(m_tempFile->fileName());
            m_imageSize = image.size();
        }
    }
    m_tempFile->close();

    Q_EMIT imageUrlChanged();
    Q_EMIT imageSizeChanged();
}

void KritaHandler::clearState()
{
    m_tempFile.reset();
}