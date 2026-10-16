// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QMimeDatabase>
#include <QTest>

#include "../src/backend/ImageHandler.h"
#include "../src/backend/KritaHandler.h"

class ImageTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testImageSupportStatus();
    void testImageSupportStatus_data();
    void testKritaSupportStatus();
    void testKritaSupportStatus_data();
    void testKritaFiles();
    void testKritaFiles_data();
};

void ImageTest::testImageSupportStatus()
{
    QFETCH(QString, fileName);
    ImageHandler imageHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(imageHandler.canHandle(mimeType), "Image file not supported");
}

void ImageTest::testImageSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    // clang-format off
    QTest::addRow("avif") << QFINDTESTDATA("samplefiles/image/test.avif");
    QTest::addRow("bmp")  << QFINDTESTDATA("samplefiles/image/test.bmp");
    QTest::addRow("gif")  << QFINDTESTDATA("samplefiles/image/test.gif");
    QTest::addRow("heic") << QFINDTESTDATA("samplefiles/image/test.heic");
    QTest::addRow("heif") << QFINDTESTDATA("samplefiles/image/test.heif");
    QTest::addRow("icns") << QFINDTESTDATA("samplefiles/image/test.icns");
    QTest::addRow("ico")  << QFINDTESTDATA("samplefiles/image/test.ico");
    QTest::addRow("jpeg") << QFINDTESTDATA("samplefiles/image/test.jpeg");
    QTest::addRow("jpg")  << QFINDTESTDATA("samplefiles/image/test.jpg");
    QTest::addRow("png")  << QFINDTESTDATA("samplefiles/image/test.png");
    QTest::addRow("svg")  << QFINDTESTDATA("samplefiles/image/test.svg");
    QTest::addRow("tiff") << QFINDTESTDATA("samplefiles/image/test.tiff");
    QTest::addRow("webp") << QFINDTESTDATA("samplefiles/image/test.webp");
    QTest::addRow("xbm")  << QFINDTESTDATA("samplefiles/image/test.xbm");
    QTest::addRow("xpm")  << QFINDTESTDATA("samplefiles/image/test.xpm");
    // clang-format on
}

void ImageTest::testKritaSupportStatus()
{
    QFETCH(QString, fileName);
    KritaHandler kritaHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(kritaHandler.canHandle(mimeType), "Image file not supported");
}

void ImageTest::testKritaSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("kra") << QFINDTESTDATA("samplefiles/image/test.kra");
    QTest::addRow("krz") << QFINDTESTDATA("samplefiles/image/test.krz");
}

void ImageTest::testKritaFiles()
{
    QFETCH(QString, fileName);
    QFETCH(QString, mergedImageFileName);
    QFETCH(QString, previewFileName);

    KritaHandler kritaHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(mimeType.inherits(QStringLiteral("application/x-krita")), "File is not a Krita file");

    kritaHandler.process(QUrl::fromLocalFile(fileName), mimeType);

    const QImage extractedImage = QImage(kritaHandler.imageUrl().toLocalFile());
    QImage mergedImage{mergedImageFileName};
    QImage previewImage{previewFileName};

    // KritaHandler scales the images, so these need to be scaled too
    mergedImage = mergedImage.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    previewImage = previewImage.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QVERIFY2(!extractedImage.isNull(), "Extracted image is null");
    QVERIFY2(!mergedImage.isNull(), "mergedimage.png is null");
    QVERIFY2(!previewImage.isNull(), "preview.png is null");

    const bool matchesMergedImage{extractedImage == mergedImage};
    const bool matchesPreviewImage{extractedImage == previewImage};

    QVERIFY2(matchesMergedImage || matchesPreviewImage, "Extracted image doesn't match mergedimage.png nor preview.png");
}

void ImageTest::testKritaFiles_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("mergedImageFileName");
    QTest::addColumn<QString>("previewFileName");

    // kra files can have preview.png extracted if mergedimage.png is missing, so check for both
    // krz files don't have mergedimage.png, so compare preview.png twice for simplicity
    // clang-format off
    QTest::addRow("kra") << QFINDTESTDATA("samplefiles/image/test.kra") 
                         << QFINDTESTDATA("samplefiles/image/kra.mergedimage.png") 
                         << QFINDTESTDATA("samplefiles/image/kra.preview.png");
    QTest::addRow("krz") << QFINDTESTDATA("samplefiles/image/test.krz") 
                         << QFINDTESTDATA("samplefiles/image/krz.preview.png") 
                         << QFINDTESTDATA("samplefiles/image/krz.preview.png");
    // clang-format on
}

QTEST_GUILESS_MAIN(ImageTest)

#include "ImageTest.moc"