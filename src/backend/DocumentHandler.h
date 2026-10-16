// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <QFutureWatcher>
#include <QTemporaryFile>
#include <QtQml/qqmlregistration.h>

class MAKILIB_EXPORT DocumentHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(QUrl documentUrl READ documentUrl NOTIFY documentUrlChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

public:
    explicit DocumentHandler(QObject *parent = nullptr, MakiImageProvider *imageProvider = nullptr);
    ~DocumentHandler();

public:
    // Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    HandlerUtils::HandlerKey fileTypeKey() const override;
    void clearState() override;

    QUrl documentUrl() const;
    Q_SIGNAL void documentUrlChanged();

    bool isLoading() const;
    Q_SIGNAL void isLoadingChanged();

private:
    void extractPageImages(const QUrl &url);
    bool isOfficeFile(const QMimeType &mime) const;
    void convertOfficeFile(const QString &inputFilePath, const QString &outputFilePath) const;

private:
    QUrl m_documentUrl;
    bool m_isLoading{false};

    QFutureWatcher<std::shared_ptr<QTemporaryFile>> m_watcher;
    std::shared_ptr<QTemporaryFile> m_tempPdf;
    MakiImageProvider *m_imageProvider{nullptr};
};
