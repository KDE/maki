// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickWindow>
#include <KAboutData>
#include <KDBusService>
#include <KLocalization>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KIconTheme>

#include "../MakiVersion.h"

#include "backend/WindowManager.h"
#include "backend/DBusInterface.h"

int main(int argc, char* argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    
    KIconTheme::initTheme();

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("maki"));

    QApplication app(argc, argv);

    QApplication::setStyle(QStringLiteral("breeze"));

    KAboutData aboutData(QStringLiteral("maki"), 
                         i18nc("@title", "Maki"), 
                         QStringLiteral(MAKI_VERSION_STRING), 
                         i18nc("@info", "Quick File Previewer"), 
                         KAboutLicense::GPL_V3,
                         i18nc("@info:credit", "Maki contributors"),
                         QString(),
                         QStringLiteral("https://invent.kde.org/fluxmusic/maki"),
                         QStringLiteral("https://invent.kde.org/fluxmusic/maki/-/work_items"));

    KAboutData::setApplicationData(aboutData);
    
    app.setQuitOnLastWindowClosed(false);
    app.setQuitLockEnabled(false);

    QCommandLineParser parser;

    QCommandLineOption daemonOption ({QStringLiteral("d"), QStringLiteral("daemon")},  i18n("Starts Maki in the background. Doesn't show a window"));
    QCommandLineOption previewOption({QStringLiteral("p"), QStringLiteral("preview")}, i18n("Shows a preview window for the specified file"), QStringLiteral("url"));
    QCommandLineOption replaceOption({QStringLiteral("r"), QStringLiteral("replace")}, i18n("Replaces the Maki process with a new one"));

    parser.addOption(daemonOption);
    parser.addOption(previewOption);
    parser.addOption(replaceOption);

    aboutData.setupCommandLine(&parser);

    parser.process(app);
    aboutData.processCommandLine(&parser);

    //Only one instance should exist at a time
    KDBusService service(KDBusService::Unique | KDBusService::StartupOption(parser.isSet(replaceOption) ? KDBusService::Replace : 0));

    QQmlApplicationEngine engine;

    MakiImageProvider* imageProvider = new MakiImageProvider();
    
    engine.addImageProvider(QStringLiteral("Maki"), imageProvider);
    KLocalization::setupLocalizedContext(&engine);
    engine.loadFromModule("org.kde.maki", "Main");
    
    WindowManager* manager = engine.singletonInstance<WindowManager*>("org.kde.maki", "WindowManager");

    DBusInterface interface(manager);

    auto setup = [&]() {
        if (manager) {
            manager->initApp(&app);
            manager->setImageProvider(imageProvider);

            if (parser.isSet(daemonOption)) {
                manager->setVisible(false);
            } else if (parser.isSet(previewOption)) {
                manager->setURL(QUrl::fromUserInput(parser.value(previewOption)));
            } else if (parser.isSet(replaceOption)) {
                qDebug() << "Replacing process";
            } else {
                parser.showHelp(1);
            }
        } else {
            qWarning() << "nullptr";
        }
    };

    setup();

    QObject::connect(&service, &KDBusService::activateRequested, 
        [&](const QStringList& args, const QString&) {
            parser.parse(args);
            setup();
        });

    return app.exec();
}