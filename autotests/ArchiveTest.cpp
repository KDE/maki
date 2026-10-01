// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "../src/backend/ArchiveHandler.h"

class ArchiveTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();
};

void ArchiveTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    ArchiveHandler archiveHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(archiveHandler.canHandle(mimeType), "Archive file not supported");
}

void ArchiveTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    //Archives
    QTest::addRow("7z")       << QFINDTESTDATA("samplefiles/archive/test.7z");
    QTest::addRow("tar")      << QFINDTESTDATA("samplefiles/archive/test.tar");
    QTest::addRow("tar.bz2")  << QFINDTESTDATA("samplefiles/archive/test.tar.bz2");
    QTest::addRow("tar.gz")   << QFINDTESTDATA("samplefiles/archive/test.tar.gz");
    QTest::addRow("tar.lz")   << QFINDTESTDATA("samplefiles/archive/test.tar.lz");
    // QTest::addRow("tar.lzma") << QFINDTESTDATA("samplefiles/archive/test.tar.lzma");      // LZMA Tars are currently bugged with KArchive. Fix pending
    QTest::addRow("tar.xz")   << QFINDTESTDATA("samplefiles/archive/test.tar.xz");
    QTest::addRow("tar.zst")  << QFINDTESTDATA("samplefiles/archive/test.tar.zst");
    QTest::addRow("zip")      << QFINDTESTDATA("samplefiles/archive/test.zip");
    //Raw compression
    QTest::addRow("raw.bz2")  << QFINDTESTDATA("samplefiles/archive/test.txt.bz2");
    QTest::addRow("raw.gz")   << QFINDTESTDATA("samplefiles/archive/test.txt.gz");
    QTest::addRow("raw.xz")   << QFINDTESTDATA("samplefiles/archive/test.txt.xz");
    QTest::addRow("raw.lz")   << QFINDTESTDATA("samplefiles/archive/test.txt.lz");
    QTest::addRow("raw.lzma") << QFINDTESTDATA("samplefiles/archive/test.txt.lzma");
    QTest::addRow("raw.zst")  << QFINDTESTDATA("samplefiles/archive/test.txt.zst");
}

QTEST_GUILESS_MAIN(ArchiveTest)

#include "ArchiveTest.moc"