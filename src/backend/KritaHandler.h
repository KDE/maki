// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ImageHandler.h"

#include <QSize>
#include <QTemporaryFile>

class MAKILIB_EXPORT KritaHandler : public ImageHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

public:
    explicit KritaHandler(QObject *parent = nullptr);

public:
    // Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    void clearState() override;

    // Also gets called in an autotest, so needs to stay public for now.
    // Also the reason why a QImage is returned, since QPixmap needs to be on the GUI Thread.
    QImage extractKritaImage(const QString &fileName) const;

private:
    std::unique_ptr<QTemporaryFile> m_tempFile{nullptr};
};
