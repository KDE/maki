// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MakiLibExport.h"

#include <KFileMetaData/EmbeddedImageData>
#include <KFileMetaData/ExtractorCollection>
#include <KFileMetaData/SimpleExtractionResult>
#include <QQuickImageProvider>
#include <QPixmap>

class MAKILIB_EXPORT EmbeddedCoverImageProvider : public QQuickImageProvider
{
public:
    EmbeddedCoverImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        const QString decodedId = QUrl::fromPercentEncoding(id.toUtf8());

        const QMimeDatabase db;
        const QMimeType mime = db.mimeTypeForFile(decodedId);

        KFileMetaData::ExtractorCollection ec;
        KFileMetaData::SimpleExtractionResult result(decodedId, mime.name(), KFileMetaData::ExtractionResult::ExtractImageData);

        const auto extractors{ ec.fetchExtractors(mime.name()) };
        for (const auto &extractor : extractors) {
            extractor->extract(&result);
        }
        
        const auto imageData = result.imageData();

        if (!imageData.isEmpty()) {
            if (imageData.contains(KFileMetaData::EmbeddedImageData::FrontCover)) {
                QImage coverImage{ QImage::fromData(imageData[KFileMetaData::EmbeddedImageData::FrontCover]) };
                return QPixmap::fromImage(coverImage);
            }
        }

        return QPixmap();
    }
};
