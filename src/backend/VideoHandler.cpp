// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VideoHandler.h"

VideoHandler::VideoHandler(QObject *parent)
: PreviewHandler(parent)
{}

bool VideoHandler::canHandle(const QMimeType &mime) const 
{
    return mime.name().startsWith(QStringLiteral("video/"));
}

void VideoHandler::process(const QUrl &url, const QMimeType &mime)
{
    Q_UNUSED(mime);

    m_videoUrl = url;
    Q_EMIT videoUrlChanged();
}

QString VideoHandler::fileTypeKey() const 
{
    return QStringLiteral("video");
}

void VideoHandler::clearState()
{
    
}

QUrl VideoHandler::videoUrl() const
{
    return m_videoUrl;
}