// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MakiLibExport.h"

#include <QQuickImageProvider>
#include <QPixmap>

class MAKILIB_EXPORT MakiImageProvider : public QQuickImageProvider
{
public:
    MakiImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        Q_UNUSED(size)
        Q_UNUSED(requestedSize)

        if (id.contains(QStringLiteral("image"))) {
            return m_pixmap;
        }

        if (id == QStringLiteral("fallback")) {
            return m_fallbackImage;
        }

        if (id.contains(QStringLiteral("audioCover"))) {
            return m_audioCover;
        }

        if (id.contains(QStringLiteral("infoPanel"))) {
            return m_infoPanelImage;
        }

        int page = id.toInt();

        if (page == 0 || page >= static_cast<int>(m_documentImages.size() + 1)) {
            return QPixmap();
        } else {
            return m_documentImages[page - 1];
        }
    }

    void setPixmap(const QPixmap &pixmap)
    {
        m_pixmap = pixmap;
    }

    void setFallbackImage(const QPixmap &pixmap)
    {
        m_fallbackImage = pixmap;
    }

    void setAudioCover(const QPixmap &pixmap)
    {
        m_audioCover = pixmap;
    }
    
    void setInfoPanelImage(const QPixmap &pixmap)
    {
        m_infoPanelImage = pixmap;
    }

    void setDocumentImages(const std::vector<QImage> documentImages)
    {
        m_documentImages.clear();

        for (QImage image : documentImages) {
            m_documentImages.push_back(QPixmap::fromImage(image));
        }
    }

    void releaseResources()
    {
        m_pixmap = QPixmap();
        m_fallbackImage = QPixmap();
        m_audioCover = QPixmap();
        m_documentImages.clear();
        m_documentImages.shrink_to_fit();
    }

private:
    QPixmap m_pixmap;

    QPixmap m_fallbackImage;

    QPixmap m_audioCover;

    QPixmap m_infoPanelImage;

    std::vector<QPixmap> m_documentImages;
};
