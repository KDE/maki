// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QApplication>
#include <QObject>
#include <qqmlintegration.h>
#include <QQuickTextDocument>
#include <QSize>
#include <QStandardItemModel>
#include <QUrl>
#include <QMimeDatabase>

#include <KArchive>

#include "ImageProvider.h"

class WindowManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool visible READ getVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QString fileType READ getFileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(QUrl url READ getURL WRITE setURL NOTIFY urlChanged)
    Q_PROPERTY(QUrl fallbackURL READ getFallbackURL NOTIFY fallbackURLChanged)
    Q_PROPERTY(QUrl audioCoverURL READ getAudioCoverURL NOTIFY audioCoverURLChanged)
    Q_PROPERTY(QUrl infoPanelURL READ getInfoPanelURL NOTIFY infoPanelURLChanged)
    Q_PROPERTY(QString fileName READ getFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QSize windowSize READ getWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(QString fileText READ getFileText NOTIFY fileTextChanged)
    Q_PROPERTY(QStandardItemModel* archiveModel READ getArchiveModel NOTIFY archiveModelChanged)
    Q_PROPERTY(int pageNum READ getPageNum NOTIFY pageNumChanged)

public:
    explicit WindowManager(QObject *parent = nullptr);

    void initApp(QApplication *app);

    void setImageProvider(MakiImageProvider *provider);

public:
    bool getVisible() const;
    void setVisible(bool visible);
    Q_SIGNAL void visibleChanged();

    QSize getWindowSize() const;
    Q_SIGNAL void windowSizeChanged();

    QUrl getURL() const;
    void setURL(const QUrl &url);
    Q_SIGNAL void urlChanged();

    QUrl getFallbackURL() const;
    Q_SIGNAL void fallbackURLChanged();

    QUrl getAudioCoverURL() const;
    Q_SIGNAL void audioCoverURLChanged();

    QUrl getInfoPanelURL() const;
    Q_SIGNAL void infoPanelURLChanged();

    QString getFileName() const;
    Q_SIGNAL void fileNameChanged();

    QString getFileType() const;
    void setFileType(const QString &fileType);
    Q_SIGNAL void fileTypeChanged();

    QString getFileText() const;
    Q_SIGNAL void fileTextChanged();

    QStandardItemModel* getArchiveModel() const;
    Q_SIGNAL void archiveModelChanged();

    int getPageNum() const;
    Q_SIGNAL void pageNumChanged();

    Q_INVOKABLE void openInDefaultApp();

    Q_INVOKABLE void releaseResources();

private:
    KArchive* createArchive(const QString &filePath);
    void populateArchiveModel(QStandardItem *parent, const KArchiveDirectory *dir);

private:
    QApplication *m_app{nullptr};

    bool m_visible{ false };

    QSize m_windowSize;

    QUrl m_url;
    QUrl m_fallbackURL;
    QUrl m_audioCoverURL;
    QUrl m_infoPanelURL;

    QString m_fileType{QStringLiteral("Placeholder")};
    QString m_fileText{QStringLiteral("Placeholder")};

    QStandardItemModel *m_archiveModel{nullptr};

    MakiImageProvider *m_imageProvider{nullptr};

    int m_pageNum{0};

    QMimeDatabase db;
};
