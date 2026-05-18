// SPDX-FileCopyrightText: 2026 Alex Huber <alex.huber743@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickWindow>
#include <KDBusService>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>

#include "backend/WindowManager.h"
#include "backend/DBusInterface.h"

int main(int argc, char* argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    
    KIconTheme::initTheme();

    QApplication app(argc, argv);
    QApplication::setOrganizationName  (QStringLiteral("FluxMusic"));
    QApplication::setOrganizationDomain(QStringLiteral("FluxMusic.github.io"));
    QApplication::setApplicationName   (QStringLiteral("Maki"));

    QApplication::setStyle(QStringLiteral("breeze"));

    app.setQuitOnLastWindowClosed(false);
    app.setQuitLockEnabled(false);

    //Only one instance should exist at a time
    KDBusService service(KDBusService::Unique);

    QQmlApplicationEngine engine;

    MakiImageProvider* imageProvider = new MakiImageProvider();
    
    engine.addImageProvider(QStringLiteral("Maki"), imageProvider);
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("io.github.FluxMusic.Maki", "Main");
    
    WindowManager* manager = engine.singletonInstance<WindowManager*>("io.github.FluxMusic.Maki", "WindowManager");

    DBusInterface interface(manager);

    auto setup = [manager, &app, imageProvider](const QStringList& args) {
        if (manager) {
            manager->initApp(&app);
            manager->setImageProvider(imageProvider);

            if (args.size() > 1) {
                if (args[1] == QStringLiteral("daemon")) {
                    manager->setVisible(false);
                } else {
                    manager->setURL(QUrl::fromUserInput(args[1]));
                }
            } else {
                qWarning() << "no arguments provided";
            }
        } else {
            qWarning() << "nullptr";
        }
    };

    setup(app.arguments());

    QObject::connect(&service, &KDBusService::activateRequested, 
        [&](const QStringList& args, const QString&) {
            setup(args);
        });

    return app.exec();
}