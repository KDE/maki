// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DocumentHandler.h"

#include <poppler-qt6.h>

#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QtConcurrentRun>
#include <QTemporaryDir>

DocumentHandler::DocumentHandler(QObject *parent, MakiImageProvider *imageProvider)
: PreviewHandler(parent)
, m_imageProvider(imageProvider)
{
    connect(&m_watcher, &QFutureWatcher<std::shared_ptr<QTemporaryFile>>::finished, this, [this]() {
        //necessary to be able to clear the shared ptr from m_watcher
        if (m_watcher.future().resultCount() == 0) {
            return;
        }
        
        m_isLoading = false;
        Q_EMIT isLoadingChanged();
        
        m_tempPdf = m_watcher.result();

        extractPageImages(QUrl::fromLocalFile(m_tempPdf->fileName()));
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
    //delete the Temp file
    clearState();

    if (mime.inherits(QStringLiteral("application/pdf"))) {
        extractPageImages(url);
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

        QString inputFilePath{ url.toLocalFile() };
        QString tempFilePath{ tempFile->fileName() };
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
    //Delete all instances of the shared ptr
    m_watcher.setFuture(QFuture<std::shared_ptr<QTemporaryFile>>());
    m_tempPdf.reset();

    //only pageNum cleanup is necessary
    m_pageNum = 0;
    Q_EMIT pageNumChanged();
}

int DocumentHandler::pageNum() const
{
    return m_pageNum;
}

QSize DocumentHandler::pageSize() const
{
    return m_pageSize;
}

bool DocumentHandler::isLoading() const
{
    return m_isLoading;
}

void DocumentHandler::extractPageImages(const QUrl &url)
{
    std::unique_ptr<Poppler::Document> doc = Poppler::Document::load(url.toLocalFile());

    if (!doc) {
        return;
    }

    doc->setRenderHint(Poppler::Document::RenderHint::Antialiasing, true);
    doc->setRenderHint(Poppler::Document::RenderHint::TextAntialiasing, true);
    doc->setRenderHint(Poppler::Document::RenderHint::ThinLineSolid, true);

    int numPages = doc->numPages();
    m_pageNum = numPages;

    std::vector<QImage> pageImages;

    pageImages.reserve(numPages);

    for (int i = 0; i < numPages; i++) {
        std::unique_ptr<Poppler::Page> page = doc->page(i);

        if (page) {
            // TODO: DPI as config?
            pageImages.push_back(page->renderToImage(72, 72));
        }
    }

    // TODO: Currently sets the window size to the first page's size
    //       But if there is ever a bigger page in the document
    //       It will clip. Maybe set the size to the dimensions
    //       of the biggest pages? So the width will be the max
    //       width of all the pages and the height as well
    m_pageSize = pageImages[0].size();
    Q_EMIT pageSizeChanged();

    if (m_imageProvider) {
        m_imageProvider->setDocumentImages(pageImages);
    }

    Q_EMIT pageNumChanged();
}

bool DocumentHandler::isOfficeFile(const QMimeType &mime) const
{
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

    if (officeMimes.contains(mime.name())) {
        return true;
    }

    return false;
}

void DocumentHandler::convertOfficeFile(const QString &inputFilePath, const QString &outputFilePath) const
{
    if (QStandardPaths::findExecutable(QStringLiteral("libreoffice")).isEmpty()) {
        //TODO: Implement warning to user to install libreoffice to preview office files
        return;
    }
        
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return;
    }
    
    QProcess process;
    process.start(QStringLiteral("libreoffice"), {
        QStringLiteral("--headless"),
        QStringLiteral("--convert-to"), QStringLiteral("pdf"),
        QStringLiteral("--outdir"), tempDir.path(),
        inputFilePath
    });
    process.waitForFinished(-1);
    
    if (process.exitCode() != 0) {
        return;
    }

    //Get the file path of the generated file by extending the original file with .pdf
    QFileInfo inputFileInfo{ inputFilePath };
    QString generatedFilePath{ tempDir.filePath(inputFileInfo.completeBaseName() + QStringLiteral(".pdf")) };

    QFile srcFile{ generatedFilePath };
    QFile destFile{ outputFilePath };

    if (!srcFile.open(QIODevice::ReadOnly) || !destFile.open(QIODevice::WriteOnly)) {
        return;
    }
    destFile.write(srcFile.readAll());
}