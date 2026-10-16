// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <QtQml/qqmlregistration.h>

class MAKILIB_EXPORT AudioHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(QUrl audioUrl READ audioUrl NOTIFY audioUrlChanged)
    Q_PROPERTY(QUrl audioCoverUrl READ audioCoverUrl NOTIFY audioCoverUrlChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)

public:
    explicit AudioHandler(QObject *parent = nullptr, MakiImageProvider *imageProvider = nullptr);

public:
    // Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    HandlerUtils::HandlerKey fileTypeKey() const override;
    void clearState() override;

    // QProperties
    QUrl audioCoverUrl() const;
    Q_SIGNAL void audioCoverUrlChanged();

    QUrl audioUrl() const;
    Q_SIGNAL void audioUrlChanged();

    QString fileName() const;
    Q_SIGNAL void fileNameChanged();

private:
    QUrl m_audioUrl{QStringLiteral("Placeholder")};
    QUrl m_audioCoverUrl{QStringLiteral("Placeholder")};

    MakiImageProvider *m_imageProvider{nullptr};
};
