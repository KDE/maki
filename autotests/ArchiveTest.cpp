// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/ArchiveHandler.h"

class ArchiveTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();

private:
    QString getArchiveFilePath(const QString &) const;
};

QString ArchiveTest::getArchiveFilePath(const QString &fileName) const
{
    return QLatin1String(ARCHIVE_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void ArchiveTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    QString testArchiveFile = getArchiveFilePath(fileName);
    ArchiveHandler archiveHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testArchiveFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(archiveHandler.canHandle(mimeType), "Archive file not supported");
}

void ArchiveTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    //Archives
    QTest::addRow("7z")       << QStringLiteral("test.7z");
    QTest::addRow("tar")      << QStringLiteral("test.tar");
    QTest::addRow("tar.bz2")  << QStringLiteral("test.tar.bz2");
    QTest::addRow("tar.gz")   << QStringLiteral("test.tar.gz");
    QTest::addRow("tar.lz")   << QStringLiteral("test.tar.lz");
    QTest::addRow("tar.lzma") << QStringLiteral("test.tar.lzma");
    QTest::addRow("tar.xz")   << QStringLiteral("test.tar.xz");
    QTest::addRow("tar.zst")  << QStringLiteral("test.tar.zst");
    QTest::addRow("zip")      << QStringLiteral("test.zip");
    //Raw compression
    QTest::addRow("raw.bz2")  << QStringLiteral("test.txt.bz2");
    QTest::addRow("raw.gz")   << QStringLiteral("test.txt.gz");
    QTest::addRow("raw.xz")   << QStringLiteral("test.txt.xz");
    QTest::addRow("raw.lz")   << QStringLiteral("test.txt.lz");
    QTest::addRow("raw.lzma") << QStringLiteral("test.txt.lzma");
    QTest::addRow("raw.zst")  << QStringLiteral("test.txt.zst");
}

QTEST_GUILESS_MAIN(ArchiveTest)

#include "ArchiveTest.moc"