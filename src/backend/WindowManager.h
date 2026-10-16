// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MakiLibExport.h"

#include <QApplication>
#include <QMimeDatabase>
#include <QObject>
#include <QSize>
#include <QUrl>
#include <qqmlintegration.h>

#include "PreviewHandler.h"

class MAKILIB_EXPORT WindowManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool visible READ getVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(HandlerUtils::HandlerKey fileKey READ fileKey NOTIFY fileKeyChanged)
    Q_PROPERTY(QUrl url READ getURL WRITE setURL NOTIFY urlChanged)
    Q_PROPERTY(QString fileName READ getFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString defaultAppName READ defaultAppName NOTIFY defaultAppNameChanged)
    Q_PROPERTY(QString defaultAppIcon READ defaultAppIcon NOTIFY defaultAppIconChanged)
    Q_PROPERTY(PreviewHandler *activeHandler READ activeHandler NOTIFY activeHandlerChanged)

public:
    explicit WindowManager(QObject *parent = nullptr);

public:
    bool getVisible() const;
    void setVisible(bool visible);
    Q_SIGNAL void visibleChanged();

    QUrl getURL() const;
    void setURL(const QUrl &url);
    Q_SIGNAL void urlChanged();

    QString getFileName() const;
    Q_SIGNAL void fileNameChanged();

    QString defaultAppName() const;
    Q_SIGNAL void defaultAppNameChanged();

    QString defaultAppIcon() const;
    Q_SIGNAL void defaultAppIconChanged();

    HandlerUtils::HandlerKey fileKey() const;
    Q_SIGNAL void fileKeyChanged();

    PreviewHandler *activeHandler() const;
    Q_SIGNAL void activeHandlerChanged();

    Q_INVOKABLE void openInDefaultApp();
    Q_INVOKABLE void releaseResources();

private:
    void initHandlers();

private:
    bool m_visible{false};
    QUrl m_url;

    HandlerUtils::HandlerKey m_fileKey{HandlerUtils::HandlerKey::None};

    QString m_defaultAppName{QStringLiteral("Placeholder")};
    QString m_defaultAppIcon{QStringLiteral("Placeholder")};

    QMimeDatabase db;

    std::vector<std::unique_ptr<PreviewHandler>> m_handlers;
    PreviewHandler *m_activeHandler{nullptr};
};
