// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <QSize>

class MAKILIB_EXPORT ImageHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")
    
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QSize imageSize READ imageSize NOTIFY imageSizeChanged)
    Q_PROPERTY(bool isAnimated READ isAnimated NOTIFY isAnimatedChanged)
public:
    explicit ImageHandler(QObject *parent = nullptr, MakiImageProvider *imageProvider = nullptr);

public:
    //Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    HandlerUtils::HandlerKey fileTypeKey() const override;
    void clearState() override;

    //QProperties
    QUrl imageUrl() const;
    Q_SIGNAL void imageUrlChanged();
    
    QSize imageSize() const;
    Q_SIGNAL void imageSizeChanged();

    bool isAnimated() const;
    Q_SIGNAL void isAnimatedChanged();

protected:
    QUrl m_imageUrl{ QUrl(QStringLiteral("Placeholder")) };
    QSize m_imageSize{ 1, 1 };

    bool m_isAnimated{ false };

    MakiImageProvider *m_imageProvider{ nullptr };
};
