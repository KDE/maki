// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QMimeDatabase>
#include <QTest>

#include "../src/backend/DocumentHandler.h"

class DocumentTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();
};

void DocumentTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    DocumentHandler documentHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(documentHandler.canHandle(mimeType), "Document file not supported");
}

void DocumentTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    // clang-format off
    QTest::addRow("doc")  << QFINDTESTDATA("samplefiles/document/test.doc");
    QTest::addRow("docx") << QFINDTESTDATA("samplefiles/document/test.docx");
    // QTest::addRow("epub") << QFINDTESTDATA("samplefiles/document/test.epub");    // EPUB support in planning
    QTest::addRow("odp")  << QFINDTESTDATA("samplefiles/document/test.odp");
    QTest::addRow("ods")  << QFINDTESTDATA("samplefiles/document/test.ods");
    QTest::addRow("odt")  << QFINDTESTDATA("samplefiles/document/test.odt");
    QTest::addRow("pdf")  << QFINDTESTDATA("samplefiles/document/test.pdf");
    QTest::addRow("ppt")  << QFINDTESTDATA("samplefiles/document/test.ppt");
    QTest::addRow("pptx") << QFINDTESTDATA("samplefiles/document/test.pptx");
    QTest::addRow("xls")  << QFINDTESTDATA("samplefiles/document/test.xls");
    QTest::addRow("xlsx") << QFINDTESTDATA("samplefiles/document/test.xlsx");
    // clang-format on
}

QTEST_GUILESS_MAIN(DocumentTest)

#include "DocumentTest.moc"