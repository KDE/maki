// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TextHandler.h"

#include <QFile>
#include <QSet>

TextHandler::TextHandler(QObject *parent)
: PreviewHandler(parent)
{}

bool TextHandler::canHandle(const QMimeType &mime) const 
{
    //Files that are essentially just text, but whose mimetype for some reason doesn't start with 'text/' 
    const QSet<QString> extraMimes {
        QStringLiteral("application/vnd.dart"),
        QStringLiteral("application/x-fishscript"),
        QStringLiteral("application/x-php"),
        QStringLiteral("application/x-perl"),
        QStringLiteral("application/x-powershell"),
        QStringLiteral("application/x-ruby"),
        QStringLiteral("application/toml"),
        QStringLiteral("application/typescript"),
        QStringLiteral("application/x-tiled-tsx"),
        QStringLiteral("application/yaml")
    };

    return mime.name().startsWith(QStringLiteral("text/")) || extraMimes.contains(mime.name());
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