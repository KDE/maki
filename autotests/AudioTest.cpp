// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/AudioHandler.h"

class AudioTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();

private:
    QString getAudioFilePath(const QString &) const;
};

QString AudioTest::getAudioFilePath(const QString &fileName) const
{
    return QLatin1String(AUDIO_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void AudioTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    QString testAudioFile = getAudioFilePath(fileName);
    AudioHandler audioHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testAudioFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(audioHandler.canHandle(mimeType), "Audio file not supported");
}

void AudioTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("aiff")      << QStringLiteral("test.aiff");
    QTest::addRow("ape")       << QStringLiteral("test.ape");
    QTest::addRow("flac")      << QStringLiteral("test.flac");
    QTest::addRow("flac.ogg")  << QStringLiteral("test.flac.ogg");
    QTest::addRow("m4a")       << QStringLiteral("test.m4a");
    QTest::addRow("mp3")       << QStringLiteral("test.mp3");
    QTest::addRow("mpc")       << QStringLiteral("test.mpc");
    QTest::addRow("ogg")       << QStringLiteral("test.ogg");
    QTest::addRow("opus")      << QStringLiteral("test.opus");
    QTest::addRow("opus.ogg")  << QStringLiteral("test.opus.ogg");
    QTest::addRow("speex.ogg") << QStringLiteral("test.speex.ogg");
    QTest::addRow("spx")       << QStringLiteral("test.spx");
    QTest::addRow("wav")       << QStringLiteral("test.wav");
    QTest::addRow("wma")       << QStringLiteral("test.wma");
    QTest::addRow("wv")        << QStringLiteral("test.wv");
}

QTEST_GUILESS_MAIN(AudioTest)

#include "AudioTest.moc"