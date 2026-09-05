#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/ImageHandler.h"

class ImageTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();

private:
    QString getImageFilePath(const QString &) const;
};

QString ImageTest::getImageFilePath(const QString &fileName) const
{
    return QLatin1String(IMAGE_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void ImageTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    QString testImageFile = getImageFilePath(fileName);
    ImageHandler imageHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testImageFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(imageHandler.canHandle(mimeType), "Video file not supported");
}

void ImageTest::testSupportStatus_data()
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
    QTest::addRow("kra")  << QStringLiteral("test.kra");
    QTest::addRow("krz")  << QStringLiteral("test.krz");
    QTest::addRow("png")  << QStringLiteral("test.png");
    QTest::addRow("svg")  << QStringLiteral("test.svg");
    QTest::addRow("tiff") << QStringLiteral("test.tiff");
    QTest::addRow("webp") << QStringLiteral("test.webp");
    QTest::addRow("xbm")  << QStringLiteral("test.xbm");
    QTest::addRow("xpm")  << QStringLiteral("test.xpm");
}

QTEST_GUILESS_MAIN(ImageTest)

#include "imagetest.moc"