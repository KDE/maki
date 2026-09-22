// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewHandler.h"

#include "ImageProvider.h"

#include <KArchive>

#include <QStandardItemModel>
#include <QtQml/qqmlregistration.h>

class MAKILIB_EXPORT ArchiveHandler : public PreviewHandler
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Handlers are managed by WindowManager")

    Q_PROPERTY(QString fileSize READ fileSize NOTIFY fileSizeChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QUrl infoPanelUrl READ infoPanelUrl NOTIFY infoPanelUrlChanged)
    Q_PROPERTY(QStandardItemModel* archiveModel READ archiveModel NOTIFY archiveModelChanged)

public:
    explicit ArchiveHandler(QObject *parent, MakiImageProvider *imageProvider = nullptr);

public:
    //Overrides
    bool canHandle(const QMimeType &mime) const override;
    void process(const QUrl &url, const QMimeType &mime) override;
    QString fileTypeKey() const override;
    void clearState() override;

    //QProperty accessors
    QString fileSize() const;
    Q_SIGNAL void fileSizeChanged();
    
    QString fileName() const;
    Q_SIGNAL void fileNameChanged();
    
    QUrl infoPanelUrl() const;
    Q_SIGNAL void infoPanelUrlChanged();

    QStandardItemModel* archiveModel() const;
    Q_SIGNAL void archiveModelChanged();

private:
    KArchive* createArchive(const QString &filePath);
    void populateArchiveModel(QStandardItem *parent, const KArchiveDirectory *dir);
    qint64 getDirSize(const KArchiveDirectory *dir);

private:

    QString m_fileSize{ QStringLiteral("Placeholder") };
    QString m_fileName{ QStringLiteral("Placeholder") };
    QUrl m_infoPanelUrl{ QStringLiteral("Placeholder") };
    QStandardItemModel *m_archiveModel{ nullptr };

    MakiImageProvider *m_imageProvider{ nullptr };
};