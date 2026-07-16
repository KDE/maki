// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <QFutureWatcher>
#include <QTemporaryFile>
#include <QtQml/qqmlregistration.h>

class DocumentHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(int pageNum READ pageNum NOTIFY pageNumChanged)
    Q_PROPERTY(QSize pageSize READ pageSize NOTIFY pageSizeChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

public:
    explicit DocumentHandler(QObject *parent = nullptr, MakiImageProvider *imageProvider = nullptr);
    ~DocumentHandler();

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

    bool isLoading() const;
    Q_SIGNAL void isLoadingChanged();

private:
    void extractPageImages(const QUrl &url);
    bool isOfficeFile(const QMimeType &mime) const;
    void convertOfficeFile(const QString &inputFilePath, const QString &outputFilePath) const;

private:
    int m_pageNum{ 0 };
    QSize m_pageSize{ 1, 1 };
    
    bool m_isLoading{ false };
    QFutureWatcher<std::shared_ptr<QTemporaryFile>> m_watcher;
    std::shared_ptr<QTemporaryFile> m_tempPdf;
    MakiImageProvider *m_imageProvider{ nullptr };
};
