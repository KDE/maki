// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FallbackHandler.h"

FallbackHandler::FallbackHandler(QObject *parent)
: PreviewHandler(parent)
{}

bool FallbackHandler::canHandle(const QMimeType &mime) const 
{
    return true;
}

void FallbackHandler::process(const QUrl &url, const QMimeType &mime)
{
    Q_UNUSED(url);

    m_fallbackIcon = mime.iconName();
    Q_EMIT fallbackIconChanged();
}

QString FallbackHandler::fileTypeKey() const 
{
    return QStringLiteral("none");
}

void FallbackHandler::clearState()
{
    m_fallbackIcon = QString();
}

QString FallbackHandler::fallbackIcon() const
{
    return m_fallbackIcon;
}
