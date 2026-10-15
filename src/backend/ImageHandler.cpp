// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ImageHandler.h"

#include <KZip>

#include <QDateTime>
#include <QImageReader>
#include <QPixmap>

ImageHandler::ImageHandler(QObject *parent, MakiImageProvider *imageProvider)
: PreviewHandler(parent)
, m_imageProvider(imageProvider)
{}

bool ImageHandler::canHandle(const QMimeType &mime) const 
{
    return mime.name().startsWith(QStringLiteral("image/"));
}

void ImageHandler::process(const QUrl &url, const QMimeType &mime)
{
    Q_UNUSED(mime);

    QImageReader imageReader(url.toLocalFile());
    imageReader.setAutoTransform(true);
    m_isAnimated = imageReader.supportsAnimation();
    QImage image(imageReader.read());
    image = image.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_imageSize = image.size();

    m_imageUrl = url;
    Q_EMIT imageUrlChanged();
    Q_EMIT imageSizeChanged();
    Q_EMIT isAnimatedChanged();
}

HandlerUtils::HandlerKey ImageHandler::fileTypeKey() const 
{
    return HandlerUtils::HandlerKey::Image;
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

bool ImageHandler::isAnimated() const
{
    return m_isAnimated;
}
