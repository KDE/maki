// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include <QMimeDatabase>

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
    void setupTestFiles() const;
};

void TextTest::setupTestFiles() const
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("meson")      << QFINDTESTDATA("samplefiles/text/meson.build");
    QTest::addRow("adb")        << QFINDTESTDATA("samplefiles/text/test.adb");
    QTest::addRow("asm")        << QFINDTESTDATA("samplefiles/text/test.asm");
    QTest::addRow("c")          << QFINDTESTDATA("samplefiles/text/test.c");
    QTest::addRow("cfg")        << QFINDTESTDATA("samplefiles/text/test.cfg");
    QTest::addRow("clj")        << QFINDTESTDATA("samplefiles/text/test.clj");
    QTest::addRow("cmake")      << QFINDTESTDATA("samplefiles/text/test.cmake");
    QTest::addRow("cpp")        << QFINDTESTDATA("samplefiles/text/test.cpp");
    QTest::addRow("cr")         << QFINDTESTDATA("samplefiles/text/test.cr");
    QTest::addRow("cs")         << QFINDTESTDATA("samplefiles/text/test.cs");
    QTest::addRow("css")        << QFINDTESTDATA("samplefiles/text/test.css");
    QTest::addRow("d")          << QFINDTESTDATA("samplefiles/text/test.d");
    QTest::addRow("dart")       << QFINDTESTDATA("samplefiles/text/test.dart");
    QTest::addRow("dockerfile") << QFINDTESTDATA("samplefiles/text/test.dockerfile");
    QTest::addRow("erl")        << QFINDTESTDATA("samplefiles/text/test.erl");
    QTest::addRow("ex")         << QFINDTESTDATA("samplefiles/text/test.ex");
    QTest::addRow("f")          << QFINDTESTDATA("samplefiles/text/test.f");
    QTest::addRow("fish")       << QFINDTESTDATA("samplefiles/text/test.fish");
    QTest::addRow("fs")         << QFINDTESTDATA("samplefiles/text/test.fs");
    QTest::addRow("go")         << QFINDTESTDATA("samplefiles/text/test.go");
    QTest::addRow("groovy")     << QFINDTESTDATA("samplefiles/text/test.groovy");
    QTest::addRow("h")          << QFINDTESTDATA("samplefiles/text/test.h");
    QTest::addRow("hpp")        << QFINDTESTDATA("samplefiles/text/test.hpp");
    QTest::addRow("hs")         << QFINDTESTDATA("samplefiles/text/test.hs");
    QTest::addRow("html")       << QFINDTESTDATA("samplefiles/text/test.html");
    QTest::addRow("ini")        << QFINDTESTDATA("samplefiles/text/test.ini");
    QTest::addRow("java")       << QFINDTESTDATA("samplefiles/text/test.java");
    QTest::addRow("jl")         << QFINDTESTDATA("samplefiles/text/test.jl");
    QTest::addRow("js")         << QFINDTESTDATA("samplefiles/text/test.js");
    QTest::addRow("jsx")        << QFINDTESTDATA("samplefiles/text/test.jsx");
    QTest::addRow("kt")         << QFINDTESTDATA("samplefiles/text/test.kt");
    QTest::addRow("less")       << QFINDTESTDATA("samplefiles/text/test.less");
    QTest::addRow("lisp")       << QFINDTESTDATA("samplefiles/text/test.lisp");
    QTest::addRow("lua")        << QFINDTESTDATA("samplefiles/text/test.lua");
    QTest::addRow("m")          << QFINDTESTDATA("samplefiles/text/test.m");
    QTest::addRow("mk")         << QFINDTESTDATA("samplefiles/text/test.mk");
    QTest::addRow("ml")         << QFINDTESTDATA("samplefiles/text/test.ml");
    QTest::addRow("mm")         << QFINDTESTDATA("samplefiles/text/test.mm");
    QTest::addRow("nim")        << QFINDTESTDATA("samplefiles/text/test.nim");
    QTest::addRow("ninja")      << QFINDTESTDATA("samplefiles/text/test.ninja");
    QTest::addRow("pas")        << QFINDTESTDATA("samplefiles/text/test.pas");
    QTest::addRow("php")        << QFINDTESTDATA("samplefiles/text/test.php");
    QTest::addRow("pl")         << QFINDTESTDATA("samplefiles/text/test.pl");
    QTest::addRow("ps1")        << QFINDTESTDATA("samplefiles/text/test.ps1");
    QTest::addRow("py")         << QFINDTESTDATA("samplefiles/text/test.py");
    QTest::addRow("qml")        << QFINDTESTDATA("samplefiles/text/test.qml");
    QTest::addRow("r")          << QFINDTESTDATA("samplefiles/text/test.r");
    QTest::addRow("rb")         << QFINDTESTDATA("samplefiles/text/test.rb");
    QTest::addRow("rs")         << QFINDTESTDATA("samplefiles/text/test.rs");
    QTest::addRow("rst")        << QFINDTESTDATA("samplefiles/text/test.rst");
    QTest::addRow("rtx")        << QFINDTESTDATA("samplefiles/text/test.rtx");
    QTest::addRow("sass")       << QFINDTESTDATA("samplefiles/text/test.sass");
    QTest::addRow("scala")      << QFINDTESTDATA("samplefiles/text/test.scala");
    QTest::addRow("scm")        << QFINDTESTDATA("samplefiles/text/test.scm");
    QTest::addRow("scss")       << QFINDTESTDATA("samplefiles/text/test.scss");
    QTest::addRow("sh")         << QFINDTESTDATA("samplefiles/text/test.sh");
    QTest::addRow("svelte")     << QFINDTESTDATA("samplefiles/text/test.svelte");
    QTest::addRow("swift")      << QFINDTESTDATA("samplefiles/text/test.swift");
    QTest::addRow("tcl")        << QFINDTESTDATA("samplefiles/text/test.tcl");
    QTest::addRow("tex")        << QFINDTESTDATA("samplefiles/text/test.tex");
    QTest::addRow("toml")       << QFINDTESTDATA("samplefiles/text/test.toml");
    QTest::addRow("ts")         << QFINDTESTDATA("samplefiles/text/test.ts");
    QTest::addRow("tsx")        << QFINDTESTDATA("samplefiles/text/test.tsx");
    QTest::addRow("txt")        << QFINDTESTDATA("samplefiles/text/test.txt");
    QTest::addRow("vb")         << QFINDTESTDATA("samplefiles/text/test.vb");
    QTest::addRow("yaml")       << QFINDTESTDATA("samplefiles/text/test.yaml");
    QTest::addRow("zig")        << QFINDTESTDATA("samplefiles/text/test.zig");
    QTest::addRow("zsh")        << QFINDTESTDATA("samplefiles/text/test.zsh");
}

void TextTest::testSupportStatus()
{
    QFETCH(QString, fileName);
    TextHandler textHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);
    
    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");
    QVERIFY2(textHandler.canHandle(mimeType), "Text file not supported");
}

void TextTest::testSupportStatus_data()
{
    setupTestFiles();
}

void TextTest::testContent()
{
    QFETCH(QString, fileName);
    TextHandler textHandler{this};

    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileName);

    QVERIFY2(mimeType.isValid(), "Failed to determine mimetype");

    textHandler.process(QUrl::fromLocalFile(fileName), mimeType);

    //All test files have a newline, so compare for that as well
    QCOMPARE(textHandler.text(), QStringLiteral("test\n"));
}

void TextTest::testContent_data()
{
    setupTestFiles();
}

QTEST_GUILESS_MAIN(TextTest)

#include "TextTest.moc"