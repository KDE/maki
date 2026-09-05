// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <QSize>

class ImageHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")
    
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QSize imageSize READ imageSize NOTIFY imageSizeChanged)
public:
    explicit ImageHandler(QObject *parent = nullptr, MakiImageProvider *imageProvider = nullptr);

public:
    //Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    QString fileTypeKey() const override;
    void clearState() override;

    //QProperties
    QUrl imageUrl() const;
    Q_SIGNAL void imageUrlChanged();
    
    QSize imageSize() const;
    Q_SIGNAL void imageSizeChanged();

    // Also gets called in an autotest, so needs to stay public for now. 
    // Also the reason why a QImage is returned, since QPixmap needs to be on the GUI Thread.
    QImage extractKritaImage(const QString &fileName) const;

private:
    QUrl m_imageUrl{ QUrl(QStringLiteral("Placeholder")) };
    QSize m_imageSize{ 1, 1 };

    MakiImageProvider *m_imageProvider{ nullptr };
};
