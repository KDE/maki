// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KAboutData>
#include <KDBusService>
#include <KIconTheme>
#include <KLocalization>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KirigamiAddons/App/KirigamiAppDefaults>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QUrl>
#include <QtQml>

#include "../MakiVersion.h"

#include "backend/DBusInterface.h"
#include "backend/WindowManager.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("maki"));

    QApplication app(argc, argv);

    KirigamiAppDefaults::apply(&app);

    KAboutData aboutData(QStringLiteral("maki"),
                         i18nc("@title", "Maki"),
                         QStringLiteral(MAKI_VERSION_STRING),
                         i18nc("@info", "Quick File Previewer"),
                         KAboutLicense::GPL_V3,
                         i18nc("@info:credit", "Maki contributors"),
                         QString(),
                         QStringLiteral("https://invent.kde.org/fluxmusic/maki"),
                         QStringLiteral("https://invent.kde.org/fluxmusic/maki/-/work_items"));

    aboutData.addAuthor(QStringLiteral("Alex Huber"), i18nc("@label", "Creator"), QStringLiteral("dev.flu7x@gmail.com"));
    aboutData.addAuthor(QStringLiteral("Kristen McWilliam"), i18nc("@label", "Author"), QStringLiteral("kristen@kde.org"));

    KAboutData::setApplicationData(aboutData);

    app.setQuitOnLastWindowClosed(false);
    app.setQuitLockEnabled(false);

    QCommandLineParser parser;

    // clang-format off
    QCommandLineOption daemonOption ({QStringLiteral("d"), QStringLiteral("daemon")},  i18n("Starts Maki in the background. Doesn't show a window"));
    QCommandLineOption previewOption({QStringLiteral("p"), QStringLiteral("preview")}, i18n("Shows a preview window for the specified file"), QStringLiteral("url"));
    QCommandLineOption replaceOption({QStringLiteral("r"), QStringLiteral("replace")}, i18n("Replaces the Maki process with a new one"));
    // clang-format on

    parser.addOption(daemonOption);
    parser.addOption(previewOption);
    parser.addOption(replaceOption);

    aboutData.setupCommandLine(&parser);

    parser.process(app);
    aboutData.processCommandLine(&parser);

    // Only one instance should exist at a time
    KDBusService service(KDBusService::Unique | KDBusService::StartupOption(parser.isSet(replaceOption) ? KDBusService::Replace : 0));

    QQmlApplicationEngine engine;

    MakiImageProvider *imageProvider = new MakiImageProvider();

    engine.addImageProvider(QStringLiteral("Maki"), imageProvider);
    KLocalization::setupLocalizedContext(&engine);
    engine.loadFromModule("org.kde.maki", "Main");

    WindowManager *manager = engine.singletonInstance<WindowManager *>("org.kde.maki", "WindowManager");

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

    QObject::connect(&service, &KDBusService::activateRequested, [&](const QStringList &args, const QString &) {
        parser.parse(args);
        setup();
    });

    return app.exec();
}