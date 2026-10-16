// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AudioHandler.h"

#include <KFileMetaData/EmbeddedImageData>
#include <KFileMetaData/ExtractorCollection>
#include <KFileMetaData/SimpleExtractionResult>

#include <QFile>
#include <QIcon>
#include <QImage>

AudioHandler::AudioHandler(QObject *parent, MakiImageProvider *imageProvider)
    : PreviewHandler(parent)
    , m_imageProvider(imageProvider)
{
}

bool AudioHandler::canHandle(const QMimeType &mime) const
{
    return mime.name().startsWith(QStringLiteral("audio/"));
}

void AudioHandler::process(const QUrl &url, const QMimeType &mime)
{
    m_audioUrl = url;
    Q_EMIT audioUrlChanged();

    bool imageFound{false};

    KFileMetaData::ExtractorCollection ec;
    KFileMetaData::SimpleExtractionResult result(url.toLocalFile(), mime.name(), KFileMetaData::ExtractionResult::ExtractImageData);

    const auto extractors{ec.fetchExtractors(mime.name())};
    for (const auto &extractor : extractors) {
        extractor->extract(&result);
    }

    const auto imageData = result.imageData();

    if (!imageData.isEmpty()) {
        if (imageData.contains(KFileMetaData::EmbeddedImageData::FrontCover)) {
            QImage coverImage{QImage::fromData(imageData[KFileMetaData::EmbeddedImageData::FrontCover])};
            if (m_imageProvider) {
                m_imageProvider->setAudioCover(QPixmap::fromImage(coverImage));
                imageFound = true;
            }
        }
    }

    // Fallback if no image
    if (!imageFound) {
        QIcon icon = QIcon::fromTheme(mime.iconName());

        if (m_imageProvider) {
            m_imageProvider->setAudioCover(icon.pixmap(64, 64));
        }
    }

    m_audioCoverUrl = QUrl(QStringLiteral("image://Maki/audioCover?") + QString::number(QDateTime::currentMSecsSinceEpoch()));
    Q_EMIT audioCoverUrlChanged();
}

HandlerUtils::HandlerKey AudioHandler::fileTypeKey() const
{
    return HandlerUtils::HandlerKey::Audio;
}

void AudioHandler::clearState()
{
}

QUrl AudioHandler::audioCoverUrl() const
{
    return m_audioCoverUrl;
}

QUrl AudioHandler::audioUrl() const
{
    return m_audioUrl;
}
QString AudioHandler::fileName() const
{
    return m_audioUrl.fileName();
}