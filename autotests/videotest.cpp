// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/VideoHandler.h"

class VideoTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testVideoFiles();
    void testVideoFiles_data();

private:
    QString getVideoFilePath(const QString &) const;
};

QString VideoTest::getVideoFilePath(const QString &fileName) const
{
    return QLatin1String(VIDEO_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void VideoTest::testVideoFiles()
{
    QFETCH(QString, fileName);
    QString testVideoFile = getVideoFilePath(fileName);
    VideoHandler videoHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testVideoFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(videoHandler.canHandle(mimeType), "Video file not supported");
}

void VideoTest::testVideoFiles_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("avi")  << QStringLiteral("test.avi");
    QTest::addRow("mkv")  << QStringLiteral("test.mkv");
    QTest::addRow("mov")  << QStringLiteral("test.mov");
    QTest::addRow("mp4")  << QStringLiteral("test.mp4");
    QTest::addRow("mpg")  << QStringLiteral("test.mpg");
    QTest::addRow("vob")  << QStringLiteral("test.vob");
    QTest::addRow("webm") << QStringLiteral("test.webm");
    QTest::addRow("wmv")  << QStringLiteral("test.wmv");
}

QTEST_GUILESS_MAIN(VideoTest)

#include "videotest.moc"