// SPDX-FileCopyrightText: 2026 Alex Huber <alex.huber743@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include "WindowManager.h"

class DBusInterface : public QObject
{
    Q_OBJECT

public:
    DBusInterface(WindowManager* windowManager, QObject* parent = nullptr);

public Q_SLOTS:
    void showFile(QString url);

private:
    WindowManager* m_windowManager{ nullptr };
};
