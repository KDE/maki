// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TextHandler.h"

#include <QFile>

TextHandler::TextHandler(QObject *parent)
: PreviewHandler(parent)
{}

bool TextHandler::canHandle(const QMimeType &mime) const 
{
    return mime.name().startsWith(QStringLiteral("text/"));
}

void TextHandler::process(const QUrl &url, const QMimeType &mime)
{
    Q_UNUSED(mime);
    
    QFile textFile(url.toLocalFile());
    m_fileName = textFile.fileName();
    Q_EMIT fileNameChanged();

    if (textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_text = QString::fromUtf8(textFile.readAll());
        Q_EMIT textChanged();
    }
}

QString TextHandler::fileTypeKey() const 
{
    return QStringLiteral("text");
}

void TextHandler::clearState()
{
    m_text = QString();
}

QString TextHandler::text() const
{
    return m_text;
}

QString TextHandler::fileName() const
{
    return m_fileName;
}