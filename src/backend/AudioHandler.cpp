// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AudioHandler.h"

#include <KFileMetaData/EmbeddedImageData>
#include <KFileMetaData/ExtractorCollection>
#include <KFileMetaData/SimpleExtractionResult>

#include <QFile>
#include <QIcon>
#include <QImage>

AudioHandler::AudioHandler(QObject *parent)
    : PreviewHandler(parent)
{
}

bool AudioHandler::canHandle(const QMimeType &mime) const
{
    return mime.name().startsWith(QStringLiteral("audio/"));
}

void AudioHandler::process(const QUrl &url, const QMimeType &mime)
{
    Q_UNUSED(mime)

    m_audioUrl = url;
    Q_EMIT audioUrlChanged();

    const QString encodedPath = QString::fromUtf8(QUrl::toPercentEncoding(url.toLocalFile()));
    m_audioCoverUrl = QUrl(QStringLiteral("image://audioCover/") + encodedPath);
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