// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include <QtQml/qqmlregistration.h>

class MAKILIB_EXPORT FallbackHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(QString fallbackIcon READ fallbackIcon NOTIFY fallbackIconChanged)

public:
    explicit FallbackHandler(QObject *parent = nullptr);

public:
    // Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    HandlerUtils::HandlerKey fileTypeKey() const override;
    void clearState() override;

    QString fallbackIcon() const;
    Q_SIGNAL void fallbackIconChanged();

private:
    QString m_fallbackIcon;
};
