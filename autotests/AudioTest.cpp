// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QMimeDatabase>
#include <QTest>

#include "../src/backend/AudioHandler.h"

class AudioTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();
};

void AudioTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    AudioHandler audioHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(audioHandler.canHandle(mimeType), "Audio file not supported");
}

void AudioTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    // clang-format off
    QTest::addRow("aiff")      << QFINDTESTDATA("samplefiles/audio/test.aiff");
    QTest::addRow("ape")       << QFINDTESTDATA("samplefiles/audio/test.ape");
    QTest::addRow("flac")      << QFINDTESTDATA("samplefiles/audio/test.flac");
    QTest::addRow("flac.ogg")  << QFINDTESTDATA("samplefiles/audio/test.flac.ogg");
    QTest::addRow("m4a")       << QFINDTESTDATA("samplefiles/audio/test.m4a");
    QTest::addRow("mp3")       << QFINDTESTDATA("samplefiles/audio/test.mp3");
    QTest::addRow("mpc")       << QFINDTESTDATA("samplefiles/audio/test.mpc");
    QTest::addRow("ogg")       << QFINDTESTDATA("samplefiles/audio/test.ogg");
    QTest::addRow("opus")      << QFINDTESTDATA("samplefiles/audio/test.opus");
    QTest::addRow("opus.ogg")  << QFINDTESTDATA("samplefiles/audio/test.opus.ogg");
    QTest::addRow("speex.ogg") << QFINDTESTDATA("samplefiles/audio/test.speex.ogg");
    QTest::addRow("spx")       << QFINDTESTDATA("samplefiles/audio/test.spx");
    QTest::addRow("wav")       << QFINDTESTDATA("samplefiles/audio/test.wav");
    QTest::addRow("wma")       << QFINDTESTDATA("samplefiles/audio/test.wma");
    QTest::addRow("wv")        << QFINDTESTDATA("samplefiles/audio/test.wv");
    // clang-format on
}

QTEST_GUILESS_MAIN(AudioTest)

#include "AudioTest.moc"