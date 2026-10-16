// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MakiLibExport.h"

#include <QMimeType>
#include <QObject>
#include <QUrl>

#include <QtQml/qqmlregistration.h>

namespace HandlerUtils
{
Q_NAMESPACE_EXPORT(MAKILIB_EXPORT)

QML_NAMED_ELEMENT(HandlerUtils)

enum HandlerKey {
    None = 0,

    Archive = 1,
    Audio = 2,
    Document = 3,
    Image = 4,
    Text = 5,
    Video = 6
};
Q_ENUM_NS(HandlerKey)
}

class MAKILIB_EXPORT PreviewHandler : public QObject
{
    Q_OBJECT
public:
    explicit PreviewHandler(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
    virtual ~PreviewHandler() = default;

    virtual bool canHandle(const QMimeType &mime) const = 0;
    virtual void process(const QUrl &url, const QMimeType &mime) = 0;
    virtual HandlerUtils::HandlerKey fileTypeKey() const = 0;

    virtual void clearState() = 0;
};
