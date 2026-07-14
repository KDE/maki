// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <QtQml/qqmlregistration.h>

class DocumentHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(int pageNum READ pageNum NOTIFY pageNumChanged)
    Q_PROPERTY(QSize pageSize READ pageSize NOTIFY pageSizeChanged)

public:
    explicit DocumentHandler(QObject *parent = nullptr, MakiImageProvider *imageProvider = nullptr);

public:
    //Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    QString fileTypeKey() const override;
    void clearState() override;

    int pageNum() const;
    Q_SIGNAL void pageNumChanged();

    QSize pageSize() const;
    Q_SIGNAL void pageSizeChanged();

private:
    int m_pageNum{ 0 };
    QSize m_pageSize{ 1, 1 };

    MakiImageProvider *m_imageProvider{ nullptr };
};
