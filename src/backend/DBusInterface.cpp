// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DBusInterface.h"

#include "makiadaptor.h"
#include <QDBusConnection>
#include <QString>
#include <QUrl>

DBusInterface::DBusInterface(WindowManager *windowManager, QObject *parent)
    : m_windowManager(windowManager)
{
    Q_UNUSED(parent);

    new MakiAdaptor(this);
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/maki"), this);
}

void DBusInterface::showFile(QString url)
{
    if (m_windowManager) {
        m_windowManager->setURL(QUrl::fromUserInput(url));
    } else {
        qDebug() << "Failed to access window Manager!";
    }
}
