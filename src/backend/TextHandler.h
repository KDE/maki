// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include <QtQml/qqmlregistration.h>

class MAKILIB_EXPORT TextHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")
    
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)

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

    QString fileName() const;
    Q_SIGNAL void fileNameChanged();

private:
    QString m_text{ QStringLiteral("Placeholder") };
    QString m_fileName{ QStringLiteral("Placeholder") };
};
