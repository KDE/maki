// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

#include "testconfig.h"
#include "../src/backend/TextHandler.h"

class TextTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSupportStatus();
    void testSupportStatus_data();
    void testContent();
    void testContent_data();

private:
    QString getTextFilePath(const QString &) const;
};

QString TextTest::getTextFilePath(const QString &fileName) const
{
    return QLatin1String(TEXT_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void TextTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    QString testTextFile = getTextFilePath(fileName);
    TextHandler textHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testTextFile);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(textHandler.canHandle(mimeType), "Text file not supported");
}

void TextTest::testSupportStatus_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("meson")      << QStringLiteral("meson.build");
    QTest::addRow("adb")        << QStringLiteral("test.adb");
    QTest::addRow("asm")        << QStringLiteral("test.asm");
    QTest::addRow("c")          << QStringLiteral("test.c");
    QTest::addRow("cfg")        << QStringLiteral("test.cfg");
    QTest::addRow("clj")        << QStringLiteral("test.clj");
    QTest::addRow("cmake")      << QStringLiteral("test.cmake");
    QTest::addRow("cpp")        << QStringLiteral("test.cpp");
    QTest::addRow("cr")         << QStringLiteral("test.cr");
    QTest::addRow("cs")         << QStringLiteral("test.cs");
    QTest::addRow("css")        << QStringLiteral("test.css");
    QTest::addRow("d")          << QStringLiteral("test.d");
    QTest::addRow("dart")       << QStringLiteral("test.dart");
    QTest::addRow("dockerfile") << QStringLiteral("test.dockerfile");
    QTest::addRow("erl")        << QStringLiteral("test.erl");
    QTest::addRow("ex")         << QStringLiteral("test.ex");
    QTest::addRow("f")          << QStringLiteral("test.f");
    QTest::addRow("fish")       << QStringLiteral("test.fish");
    QTest::addRow("fs")         << QStringLiteral("test.fs");
    QTest::addRow("go")         << QStringLiteral("test.go");
    QTest::addRow("groovy")     << QStringLiteral("test.groovy");
    QTest::addRow("h")          << QStringLiteral("test.h");
    QTest::addRow("hpp")        << QStringLiteral("test.hpp");
    QTest::addRow("hs")         << QStringLiteral("test.hs");
    QTest::addRow("html")       << QStringLiteral("test.html");
    QTest::addRow("ini")        << QStringLiteral("test.ini");
    QTest::addRow("java")       << QStringLiteral("test.java");
    QTest::addRow("jl")         << QStringLiteral("test.jl");
    QTest::addRow("js")         << QStringLiteral("test.js");
    QTest::addRow("jsx")        << QStringLiteral("test.jsx");
    QTest::addRow("kt")         << QStringLiteral("test.kt");
    QTest::addRow("less")       << QStringLiteral("test.less");
    QTest::addRow("lisp")       << QStringLiteral("test.lisp");
    QTest::addRow("lua")        << QStringLiteral("test.lua");
    QTest::addRow("m")          << QStringLiteral("test.m");
    QTest::addRow("mk")         << QStringLiteral("test.mk");
    QTest::addRow("ml")         << QStringLiteral("test.ml");
    QTest::addRow("mm")         << QStringLiteral("test.mm");
    QTest::addRow("nim")        << QStringLiteral("test.nim");
    QTest::addRow("ninja")      << QStringLiteral("test.ninja");
    QTest::addRow("pas")        << QStringLiteral("test.pas");
    QTest::addRow("php")        << QStringLiteral("test.php");
    QTest::addRow("pl")         << QStringLiteral("test.pl");
    QTest::addRow("ps1")        << QStringLiteral("test.ps1");
    QTest::addRow("py")         << QStringLiteral("test.py");
    QTest::addRow("qml")        << QStringLiteral("test.qml");
    QTest::addRow("r")          << QStringLiteral("test.r");
    QTest::addRow("rb")         << QStringLiteral("test.rb");
    QTest::addRow("rs")         << QStringLiteral("test.rs");
    QTest::addRow("rst")        << QStringLiteral("test.rst");
    QTest::addRow("rtx")        << QStringLiteral("test.rtx");
    QTest::addRow("sass")       << QStringLiteral("test.sass");
    QTest::addRow("scala")      << QStringLiteral("test.scala");
    QTest::addRow("scm")        << QStringLiteral("test.scm");
    QTest::addRow("scss")       << QStringLiteral("test.scss");
    QTest::addRow("sh")         << QStringLiteral("test.sh");
    QTest::addRow("svelte")     << QStringLiteral("test.svelte");
    QTest::addRow("swift")      << QStringLiteral("test.swift");
    QTest::addRow("tcl")        << QStringLiteral("test.tcl");
    QTest::addRow("tex")        << QStringLiteral("test.tex");
    QTest::addRow("toml")       << QStringLiteral("test.toml");
    QTest::addRow("ts")         << QStringLiteral("test.ts");
    QTest::addRow("tsx")        << QStringLiteral("test.tsx");
    QTest::addRow("txt")        << QStringLiteral("test.txt");
    QTest::addRow("vb")         << QStringLiteral("test.vb");
    QTest::addRow("yaml")       << QStringLiteral("test.yaml");
    QTest::addRow("zig")        << QStringLiteral("test.zig");
    QTest::addRow("zsh")        << QStringLiteral("test.zsh");
}

void TextTest::testContent()
{
    QFETCH(QString, fileName);
    QString testTextFile = getTextFilePath(fileName);
    TextHandler textHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(testTextFile);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");

    textHandler.process(QUrl::fromLocalFile(testTextFile), mimeType);

    //All test files have a newline, so compare for that as well
    QCOMPARE(textHandler.text(), QStringLiteral("test\n"));
}

void TextTest::testContent_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("meson")      << QStringLiteral("meson.build");
    QTest::addRow("adb")        << QStringLiteral("test.adb");
    QTest::addRow("asm")        << QStringLiteral("test.asm");
    QTest::addRow("c")          << QStringLiteral("test.c");
    QTest::addRow("cfg")        << QStringLiteral("test.cfg");
    QTest::addRow("clj")        << QStringLiteral("test.clj");
    QTest::addRow("cmake")      << QStringLiteral("test.cmake");
    QTest::addRow("cpp")        << QStringLiteral("test.cpp");
    QTest::addRow("cr")         << QStringLiteral("test.cr");
    QTest::addRow("cs")         << QStringLiteral("test.cs");
    QTest::addRow("css")        << QStringLiteral("test.css");
    QTest::addRow("d")          << QStringLiteral("test.d");
    QTest::addRow("dart")       << QStringLiteral("test.dart");
    QTest::addRow("dockerfile") << QStringLiteral("test.dockerfile");
    QTest::addRow("erl")        << QStringLiteral("test.erl");
    QTest::addRow("ex")         << QStringLiteral("test.ex");
    QTest::addRow("f")          << QStringLiteral("test.f");
    QTest::addRow("fish")       << QStringLiteral("test.fish");
    QTest::addRow("fs")         << QStringLiteral("test.fs");
    QTest::addRow("go")         << QStringLiteral("test.go");
    QTest::addRow("groovy")     << QStringLiteral("test.groovy");
    QTest::addRow("h")          << QStringLiteral("test.h");
    QTest::addRow("hpp")        << QStringLiteral("test.hpp");
    QTest::addRow("hs")         << QStringLiteral("test.hs");
    QTest::addRow("html")       << QStringLiteral("test.html");
    QTest::addRow("ini")        << QStringLiteral("test.ini");
    QTest::addRow("java")       << QStringLiteral("test.java");
    QTest::addRow("jl")         << QStringLiteral("test.jl");
    QTest::addRow("js")         << QStringLiteral("test.js");
    QTest::addRow("jsx")        << QStringLiteral("test.jsx");
    QTest::addRow("kt")         << QStringLiteral("test.kt");
    QTest::addRow("less")       << QStringLiteral("test.less");
    QTest::addRow("lisp")       << QStringLiteral("test.lisp");
    QTest::addRow("lua")        << QStringLiteral("test.lua");
    QTest::addRow("m")          << QStringLiteral("test.m");
    QTest::addRow("mk")         << QStringLiteral("test.mk");
    QTest::addRow("ml")         << QStringLiteral("test.ml");
    QTest::addRow("mm")         << QStringLiteral("test.mm");
    QTest::addRow("nim")        << QStringLiteral("test.nim");
    QTest::addRow("ninja")      << QStringLiteral("test.ninja");
    QTest::addRow("pas")        << QStringLiteral("test.pas");
    QTest::addRow("php")        << QStringLiteral("test.php");
    QTest::addRow("pl")         << QStringLiteral("test.pl");
    QTest::addRow("ps1")        << QStringLiteral("test.ps1");
    QTest::addRow("py")         << QStringLiteral("test.py");
    QTest::addRow("qml")        << QStringLiteral("test.qml");
    QTest::addRow("r")          << QStringLiteral("test.r");
    QTest::addRow("rb")         << QStringLiteral("test.rb");
    QTest::addRow("rs")         << QStringLiteral("test.rs");
    QTest::addRow("rst")        << QStringLiteral("test.rst");
    QTest::addRow("rtx")        << QStringLiteral("test.rtx");
    QTest::addRow("sass")       << QStringLiteral("test.sass");
    QTest::addRow("scala")      << QStringLiteral("test.scala");
    QTest::addRow("scm")        << QStringLiteral("test.scm");
    QTest::addRow("scss")       << QStringLiteral("test.scss");
    QTest::addRow("sh")         << QStringLiteral("test.sh");
    QTest::addRow("svelte")     << QStringLiteral("test.svelte");
    QTest::addRow("swift")      << QStringLiteral("test.swift");
    QTest::addRow("tcl")        << QStringLiteral("test.tcl");
    QTest::addRow("tex")        << QStringLiteral("test.tex");
    QTest::addRow("toml")       << QStringLiteral("test.toml");
    QTest::addRow("ts")         << QStringLiteral("test.ts");
    QTest::addRow("tsx")        << QStringLiteral("test.tsx");
    QTest::addRow("txt")        << QStringLiteral("test.txt");
    QTest::addRow("vb")         << QStringLiteral("test.vb");
    QTest::addRow("yaml")       << QStringLiteral("test.yaml");
    QTest::addRow("zig")        << QStringLiteral("test.zig");
    QTest::addRow("zsh")        << QStringLiteral("test.zsh");
}

QTEST_GUILESS_MAIN(TextTest)

#include "texttest.moc"