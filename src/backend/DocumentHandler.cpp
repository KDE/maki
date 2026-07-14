// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DocumentHandler.h"

#include <poppler-qt6.h>

DocumentHandler::DocumentHandler(QObject *parent, MakiImageProvider *imageProvider)
: PreviewHandler(parent)
, m_imageProvider(imageProvider)
{}

bool DocumentHandler::canHandle(const QMimeType &mime) const 
{
    return mime.inherits(QStringLiteral("application/pdf"));
}

void DocumentHandler::process(const QUrl &url, const QMimeType &mime)
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

QString DocumentHandler::fileTypeKey() const 
{
    return QStringLiteral("pdf");
}

void DocumentHandler::clearState()
{
    
}

int DocumentHandler::pageNum() const
{
    return m_pageNum;
}

QSize DocumentHandler::pageSize() const
{
    return m_pageSize;
}