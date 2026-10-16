// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DocumentHandler.h"

#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QtConcurrentRun>

DocumentHandler::DocumentHandler(QObject *parent, MakiImageProvider *imageProvider)
    : PreviewHandler(parent)
    , m_imageProvider(imageProvider)
{
    connect(&m_watcher, &QFutureWatcher<std::shared_ptr<QTemporaryFile>>::finished, this, [this]() {
        // necessary to be able to clear the shared ptr from m_watcher
        if (m_watcher.future().resultCount() == 0) {
            return;
        }

        m_isLoading = false;
        Q_EMIT isLoadingChanged();

        m_tempPdf = m_watcher.result();

        m_documentUrl = QUrl::fromLocalFile(m_tempPdf->fileName());
        Q_EMIT documentUrlChanged();
    });
}

DocumentHandler::~DocumentHandler()
{
    clearState();
}

bool DocumentHandler::canHandle(const QMimeType &mime) const
{
    return mime.inherits(QStringLiteral("application/pdf")) || isOfficeFile(mime);
}

void DocumentHandler::process(const QUrl &url, const QMimeType &mime)
{
    if (mime.inherits(QStringLiteral("application/pdf"))) {
        m_documentUrl = url;
        Q_EMIT documentUrlChanged();
    } else if (isOfficeFile(mime)) {
        if (m_isLoading) {
            return;
        }

        m_isLoading = true;
        Q_EMIT isLoadingChanged();

        std::shared_ptr<QTemporaryFile> tempFile = std::make_shared<QTemporaryFile>();

        tempFile->setFileTemplate(QDir::tempPath() + QStringLiteral("/preview_XXXXXX.pdf"));

        if (!tempFile->open()) {
            m_isLoading = false;
            Q_EMIT isLoadingChanged();
            return;
        }

        QString inputFilePath{url.toLocalFile()};
        QString tempFilePath{tempFile->fileName()};
        tempFile->close();

        QFuture<std::shared_ptr<QTemporaryFile>> future = QtConcurrent::run([this, inputFilePath, tempFile]() {
            convertOfficeFile(inputFilePath, tempFile->fileName());
            return tempFile;
        });

        m_watcher.setFuture(future);
    }
}

HandlerUtils::HandlerKey DocumentHandler::fileTypeKey() const
{
    return HandlerUtils::HandlerKey::Document;
}

void DocumentHandler::clearState()
{
    // Delete all instances of the shared ptr
    m_watcher.setFuture(QFuture<std::shared_ptr<QTemporaryFile>>());
    m_tempPdf.reset();
}

QUrl DocumentHandler::documentUrl() const
{
    return m_documentUrl;
}

bool DocumentHandler::isLoading() const
{
    return m_isLoading;
}

bool DocumentHandler::isOfficeFile(const QMimeType &mime) const
{
    // clang-format off
    const QSet<QString> officeMimes {
        // OpenDocument files
        QStringLiteral("application/vnd.oasis.opendocument.text"),
        QStringLiteral("application/vnd.oasis.opendocument.spreadsheet"),
        QStringLiteral("application/vnd.oasis.opendocument.presentation"),
        QStringLiteral("application/vnd.oasis.opendocument.graphics"),
        QStringLiteral("application/vnd.oasis.opendocument.formula"),
        QStringLiteral("application/vnd.oasis.opendocument.text-template"),
        QStringLiteral("application/vnd.oasis.opendocument.spreadsheet-template"),
        QStringLiteral("application/vnd.oasis.opendocument.presentation-template"),

        // MS Office files
        QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
        QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
        QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.presentation"),
        QStringLiteral("application/vnd.ms-word.document.macroEnabled.12"),
        QStringLiteral("application/vnd.ms-excel.sheet.macroEnabled.12"),
        QStringLiteral("application/vnd.ms-powerpoint.presentation.macroEnabled.12"),
        QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.template"),
        QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.template"),
        QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.template"),

        // Legacy MS Office files
        QStringLiteral("application/msword"),
        QStringLiteral("application/vnd.ms-excel"),
        QStringLiteral("application/vnd.ms-powerpoint")
    };
    // clang-format on

    if (officeMimes.contains(mime.name())) {
        return true;
    }

    return false;
}

void DocumentHandler::convertOfficeFile(const QString &inputFilePath, const QString &outputFilePath) const
{
    if (QStandardPaths::findExecutable(QStringLiteral("libreoffice")).isEmpty()) {
        // TODO: Implement warning to user to install libreoffice to preview office files
        return;
    }

    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return;
    }

    // clang-format off
    QProcess process;
    process.start(QStringLiteral("libreoffice"), {
        QStringLiteral("--headless"),
        QStringLiteral("--convert-to"), QStringLiteral("pdf"),
        QStringLiteral("--outdir"), tempDir.path(),
        inputFilePath
    });
    process.waitForFinished(-1);
    // clang-format on

    if (process.exitCode() != 0) {
        return;
    }

    // Get the file path of the generated file by extending the original file with .pdf
    QFileInfo inputFileInfo{inputFilePath};
    QString generatedFilePath{tempDir.filePath(inputFileInfo.completeBaseName() + QStringLiteral(".pdf"))};

    QFile srcFile{generatedFilePath};
    QFile destFile{outputFilePath};

    if (!srcFile.open(QIODevice::ReadOnly) || !destFile.open(QIODevice::WriteOnly)) {
        return;
    }
    destFile.write(srcFile.readAll());
}