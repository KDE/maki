// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/DocumentHandler.h"

class DocumentTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();

private:
    QString getDocumentFilePath(const QString &) const;
};

QString DocumentTest::getDocumentFilePath(const QString &fileName) const
{
    return QLatin1String(DOCUMENT_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void DocumentTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    QString testDocumentFile = getDocumentFilePath(fileName);
    DocumentHandler documentHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testDocumentFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(documentHandler.canHandle(mimeType), "Document file not supported");
}

void DocumentTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("doc")  << QStringLiteral("test.doc");
    QTest::addRow("docx") << QStringLiteral("test.docx");
    // QTest::addRow("epub") << QStringLiteral("test.epub");     EPUB support in planning
    QTest::addRow("odp")  << QStringLiteral("test.odp");
    QTest::addRow("ods")  << QStringLiteral("test.ods");
    QTest::addRow("odt")  << QStringLiteral("test.odt");
    QTest::addRow("pdf")  << QStringLiteral("test.pdf");
    QTest::addRow("ppt")  << QStringLiteral("test.ppt");
    QTest::addRow("pptx") << QStringLiteral("test.pptx");
    QTest::addRow("xls")  << QStringLiteral("test.xls");
    QTest::addRow("xlsx") << QStringLiteral("test.xlsx");
}

QTEST_GUILESS_MAIN(DocumentTest)

#include "documenttest.moc"