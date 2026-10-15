// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include <QtQml/qqmlregistration.h>

class MAKILIB_EXPORT VideoHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(QUrl videoUrl READ videoUrl NOTIFY videoUrlChanged)

public: 
    explicit VideoHandler(QObject *parent = nullptr);

public:
    //Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    HandlerUtils::HandlerKey fileTypeKey() const override;
    void clearState() override;

    QUrl videoUrl() const;
    Q_SIGNAL void videoUrlChanged();

private:
    QUrl m_videoUrl;
};
