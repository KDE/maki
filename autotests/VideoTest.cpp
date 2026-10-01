// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "../src/backend/VideoHandler.h"

class VideoTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testVideoFiles();
    void testVideoFiles_data();
};

void VideoTest::testVideoFiles()
{
    QFETCH(QString, fileName);
    VideoHandler videoHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(videoHandler.canHandle(mimeType), "Video file not supported");
}

void VideoTest::testVideoFiles_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("avi")  << QFINDTESTDATA("samplefiles/video/test.avi");
    QTest::addRow("mkv")  << QFINDTESTDATA("samplefiles/video/test.mkv");
    QTest::addRow("mov")  << QFINDTESTDATA("samplefiles/video/test.mov");
    QTest::addRow("mp4")  << QFINDTESTDATA("samplefiles/video/test.mp4");
    QTest::addRow("mpg")  << QFINDTESTDATA("samplefiles/video/test.mpg");
    QTest::addRow("vob")  << QFINDTESTDATA("samplefiles/video/test.vob");
    QTest::addRow("webm") << QFINDTESTDATA("samplefiles/video/test.webm");
    QTest::addRow("wmv")  << QFINDTESTDATA("samplefiles/video/test.wmv");
}

QTEST_GUILESS_MAIN(VideoTest)

#include "VideoTest.moc"