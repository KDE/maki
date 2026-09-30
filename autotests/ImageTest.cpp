// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/ImageHandler.h"
#include "../src/backend/KritaHandler.h"

class ImageTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testImageSupportStatus();
    void testImageSupportStatus_data();
    void testKritaSupportStatus();
    void testKritaSupportStatus_data();
    void testKritaFiles();
    void testKritaFiles_data();

private:
    QString getImageFilePath(const QString &) const;
};

QString ImageTest::getImageFilePath(const QString &fileName) const
{
    return QLatin1String(IMAGE_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void ImageTest::testImageSupportStatus()
{
    QFETCH(QString, fileName);
    QString testImageFile = getImageFilePath(fileName);
    ImageHandler imageHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testImageFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(imageHandler.canHandle(mimeType), "Image file not supported");
}

void ImageTest::testImageSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("avif") << QStringLiteral("test.avif");
    QTest::addRow("bmp")  << QStringLiteral("test.bmp");
    QTest::addRow("gif")  << QStringLiteral("test.gif");
    QTest::addRow("heic") << QStringLiteral("test.heic");
    QTest::addRow("heif") << QStringLiteral("test.heif");
    QTest::addRow("icns") << QStringLiteral("test.icns");
    QTest::addRow("ico")  << QStringLiteral("test.ico");
    QTest::addRow("jpeg") << QStringLiteral("test.jpeg");
    QTest::addRow("jpg")  << QStringLiteral("test.jpg");
    QTest::addRow("png")  << QStringLiteral("test.png");
    QTest::addRow("svg")  << QStringLiteral("test.svg");
    QTest::addRow("tiff") << QStringLiteral("test.tiff");
    QTest::addRow("webp") << QStringLiteral("test.webp");
    QTest::addRow("xbm")  << QStringLiteral("test.xbm");
    QTest::addRow("xpm")  << QStringLiteral("test.xpm");
}

void ImageTest::testKritaSupportStatus()
{
    QFETCH(QString, fileName);
    QString testImageFile = getImageFilePath(fileName);
    KritaHandler kritaHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testImageFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(kritaHandler.canHandle(mimeType), "Image file not supported");
}

void ImageTest::testKritaSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("kra")  << QStringLiteral("test.kra");
    QTest::addRow("krz")  << QStringLiteral("test.krz");
}

void ImageTest::testKritaFiles()
{
    QFETCH(QString, fileName);
    QFETCH(QString, mergedImageFileName);
    QFETCH(QString, previewFileName);
    QString testImageFile    = getImageFilePath(fileName);
    QString mergedImageFile  = getImageFilePath(mergedImageFileName);
    QString previewImageFile = getImageFilePath(previewFileName);

    KritaHandler kritaHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testImageFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(mimeType.inherits(QStringLiteral("application/x-krita")), "File is not a Krita file");

    kritaHandler.process(QUrl::fromLocalFile(testImageFile), mimeType);

    const QImage extractedImage = QImage(kritaHandler.imageUrl().toLocalFile());
    QImage mergedImage  { mergedImageFile };
    QImage previewImage { previewImageFile };
    
    //KritaHandler scales the images, so these need to be scaled too
    mergedImage = mergedImage.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    previewImage = previewImage.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QVERIFY2(!extractedImage.isNull(), "Extracted image is null");
    QVERIFY2(!mergedImage.isNull(),    "mergedimage.png is null");
    QVERIFY2(!previewImage.isNull(),   "preview.png is null");

    const bool matchesMergedImage  { extractedImage == mergedImage };
    const bool matchesPreviewImage { extractedImage == previewImage };

    QVERIFY2(matchesMergedImage || matchesPreviewImage, "Extracted image doesn't match mergedimage.png nor preview.png");
}

void ImageTest::testKritaFiles_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("mergedImageFileName");
    QTest::addColumn<QString>("previewFileName");

    //kra files can have preview.png extracted if mergedimage.png is missing, so check for both
    //krz files don't have mergedimage.png, so compare preview.png twice for simplicity
    QTest::addRow("kra") << QStringLiteral("test.kra") << QStringLiteral("kra.mergedimage.png") << QStringLiteral("kra.preview.png");
    QTest::addRow("krz") << QStringLiteral("test.krz") << QStringLiteral("krz.preview.png") << QStringLiteral("krz.preview.png");
}

QTEST_GUILESS_MAIN(ImageTest)

#include "ImageTest.moc"