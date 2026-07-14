// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include <QtQml/qqmlregistration.h>

class TextHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")
    
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
    explicit TextHandler(QObject *parent = nullptr);

public:
    //Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    QString fileTypeKey() const override;
    void clearState() override;

    //QProperties
    QString text() const;
    Q_SIGNAL void textChanged();

private:
    QString m_text{ QStringLiteral("Placeholder") };
};
