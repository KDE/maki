// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>

#include "../src/backend/WindowManager.h"

class FileRoutingTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRouting();
    void testRouting_data();
};

void FileRoutingTest::testRouting()
{
    QFETCH(QString, fileName);
    QFETCH(QString, expectedHandlerKey);

    WindowManager windowManager;
    //Necessary right now to construct all handlers.
    //Image Provider will be removed at some point
    MakiImageProvider imageProvider;
    windowManager.setImageProvider(&imageProvider);
    windowManager.setURL(QUrl::fromLocalFile(fileName));

    QVERIFY2(windowManager.getFileType() == expectedHandlerKey, "File is routed to the wrong handler");
}

void FileRoutingTest::testRouting_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("expectedHandlerKey");

    //Archives
    QTest::addRow("7z")       << QFINDTESTDATA("samplefiles/archive/test.7z")       << "archive";
    QTest::addRow("tar")      << QFINDTESTDATA("samplefiles/archive/test.tar")      << "archive";
    QTest::addRow("tar.bz2")  << QFINDTESTDATA("samplefiles/archive/test.tar.bz2")  << "archive";
    QTest::addRow("tar.gz")   << QFINDTESTDATA("samplefiles/archive/test.tar.gz")   << "archive";
    QTest::addRow("tar.lz")   << QFINDTESTDATA("samplefiles/archive/test.tar.lz")   << "archive";
    QTest::addRow("tar.xz")   << QFINDTESTDATA("samplefiles/archive/test.tar.xz")   << "archive";
    QTest::addRow("tar.zst")  << QFINDTESTDATA("samplefiles/archive/test.tar.zst")  << "archive";
    QTest::addRow("txt.bz2")  << QFINDTESTDATA("samplefiles/archive/test.txt.bz2")  << "archive";
    QTest::addRow("txt.gz")   << QFINDTESTDATA("samplefiles/archive/test.txt.gz")   << "archive";
    QTest::addRow("txt.lz")   << QFINDTESTDATA("samplefiles/archive/test.txt.lz")   << "archive";
    QTest::addRow("txt.lzma") << QFINDTESTDATA("samplefiles/archive/test.txt.lzma") << "archive";
    QTest::addRow("txt.xz")   << QFINDTESTDATA("samplefiles/archive/test.txt.xz")   << "archive";
    QTest::addRow("txt.zst")  << QFINDTESTDATA("samplefiles/archive/test.txt.zst")  << "archive";
    QTest::addRow("zip")      << QFINDTESTDATA("samplefiles/archive/test.zip")      << "archive";
    
    //Audio Files
    QTest::addRow("aiff")      << QFINDTESTDATA("samplefiles/audio/test.aiff")      << "audio";
    QTest::addRow("ape")       << QFINDTESTDATA("samplefiles/audio/test.ape")       << "audio";
    QTest::addRow("flac")      << QFINDTESTDATA("samplefiles/audio/test.flac")      << "audio";
    QTest::addRow("flac.ogg")  << QFINDTESTDATA("samplefiles/audio/test.flac.ogg")  << "audio";
    QTest::addRow("m4a")       << QFINDTESTDATA("samplefiles/audio/test.m4a")       << "audio";
    QTest::addRow("mp3")       << QFINDTESTDATA("samplefiles/audio/test.mp3")       << "audio";
    QTest::addRow("mpc")       << QFINDTESTDATA("samplefiles/audio/test.mpc")       << "audio";
    QTest::addRow("ogg")       << QFINDTESTDATA("samplefiles/audio/test.ogg")       << "audio";
    QTest::addRow("opus")      << QFINDTESTDATA("samplefiles/audio/test.opus")      << "audio";
    QTest::addRow("opus.ogg")  << QFINDTESTDATA("samplefiles/audio/test.opus.ogg")  << "audio";
    QTest::addRow("speex.ogg") << QFINDTESTDATA("samplefiles/audio/test.speex.ogg") << "audio";
    QTest::addRow("spx")       << QFINDTESTDATA("samplefiles/audio/test.spx")       << "audio";
    QTest::addRow("wav")       << QFINDTESTDATA("samplefiles/audio/test.wav")       << "audio";
    QTest::addRow("wma")       << QFINDTESTDATA("samplefiles/audio/test.wma")       << "audio";
    QTest::addRow("wv")        << QFINDTESTDATA("samplefiles/audio/test.wv")        << "audio";
    
    //Document Files
    QTest::addRow("doc")  << QFINDTESTDATA("samplefiles/document/test.doc")  << "document";
    QTest::addRow("docx") << QFINDTESTDATA("samplefiles/document/test.docx") << "document";
    // QTest::addRow("epub") << QFINDTESTDATA("samplefiles/document/test.epub") << "document"; // EPUB support in planning
    QTest::addRow("odp")  << QFINDTESTDATA("samplefiles/document/test.odp")  << "document";
    QTest::addRow("ods")  << QFINDTESTDATA("samplefiles/document/test.ods")  << "document";
    QTest::addRow("odt")  << QFINDTESTDATA("samplefiles/document/test.odt")  << "document";
    QTest::addRow("pdf")  << QFINDTESTDATA("samplefiles/document/test.pdf")  << "document";
    QTest::addRow("ppt")  << QFINDTESTDATA("samplefiles/document/test.ppt")  << "document";
    QTest::addRow("pptx") << QFINDTESTDATA("samplefiles/document/test.pptx") << "document";
    QTest::addRow("xls")  << QFINDTESTDATA("samplefiles/document/test.xls")  << "document";
    QTest::addRow("xlsx") << QFINDTESTDATA("samplefiles/document/test.xlsx") << "document";

    //Image Files
    QTest::addRow("avif") << QFINDTESTDATA("samplefiles/image/test.avif") << "image";
    QTest::addRow("bmp")  << QFINDTESTDATA("samplefiles/image/test.bmp")  << "image";
    QTest::addRow("gif")  << QFINDTESTDATA("samplefiles/image/test.gif")  << "image";
    QTest::addRow("heic") << QFINDTESTDATA("samplefiles/image/test.heic") << "image";
    QTest::addRow("heif") << QFINDTESTDATA("samplefiles/image/test.heif") << "image";
    QTest::addRow("icns") << QFINDTESTDATA("samplefiles/image/test.icns") << "image";
    QTest::addRow("ico")  << QFINDTESTDATA("samplefiles/image/test.ico")  << "image";
    QTest::addRow("jpeg") << QFINDTESTDATA("samplefiles/image/test.jpeg") << "image";
    QTest::addRow("jpg")  << QFINDTESTDATA("samplefiles/image/test.jpg")  << "image";
    QTest::addRow("png")  << QFINDTESTDATA("samplefiles/image/test.png")  << "image";
    QTest::addRow("svg")  << QFINDTESTDATA("samplefiles/image/test.svg")  << "image";
    QTest::addRow("tiff") << QFINDTESTDATA("samplefiles/image/test.tiff") << "image";
    QTest::addRow("webp") << QFINDTESTDATA("samplefiles/image/test.webp") << "image";
    QTest::addRow("xbm")  << QFINDTESTDATA("samplefiles/image/test.xbm")  << "image";
    QTest::addRow("xpm")  << QFINDTESTDATA("samplefiles/image/test.xpm")  << "image";
    QTest::addRow("kra")  << QFINDTESTDATA("samplefiles/image/test.kra")  << "image";
    QTest::addRow("krz")  << QFINDTESTDATA("samplefiles/image/test.krz")  << "image";

    //Text Files
    QTest::addRow("meson")      << QFINDTESTDATA("samplefiles/text/meson.build")     << "text";
    QTest::addRow("adb")        << QFINDTESTDATA("samplefiles/text/test.adb")        << "text";
    QTest::addRow("asm")        << QFINDTESTDATA("samplefiles/text/test.asm")        << "text";
    QTest::addRow("c")          << QFINDTESTDATA("samplefiles/text/test.c")          << "text";
    QTest::addRow("cfg")        << QFINDTESTDATA("samplefiles/text/test.cfg")        << "text";
    QTest::addRow("clj")        << QFINDTESTDATA("samplefiles/text/test.clj")        << "text";
    QTest::addRow("cmake")      << QFINDTESTDATA("samplefiles/text/test.cmake")      << "text";
    QTest::addRow("cpp")        << QFINDTESTDATA("samplefiles/text/test.cpp")        << "text";
    QTest::addRow("cr")         << QFINDTESTDATA("samplefiles/text/test.cr")         << "text";
    QTest::addRow("cs")         << QFINDTESTDATA("samplefiles/text/test.cs")         << "text";
    QTest::addRow("css")        << QFINDTESTDATA("samplefiles/text/test.css")        << "text";
    QTest::addRow("d")          << QFINDTESTDATA("samplefiles/text/test.d")          << "text";
    QTest::addRow("dart")       << QFINDTESTDATA("samplefiles/text/test.dart")       << "text";
    QTest::addRow("dockerfile") << QFINDTESTDATA("samplefiles/text/test.dockerfile") << "text";
    QTest::addRow("erl")        << QFINDTESTDATA("samplefiles/text/test.erl")        << "text";
    QTest::addRow("ex")         << QFINDTESTDATA("samplefiles/text/test.ex")         << "text";
    QTest::addRow("f")          << QFINDTESTDATA("samplefiles/text/test.f")          << "text";
    QTest::addRow("fish")       << QFINDTESTDATA("samplefiles/text/test.fish")       << "text";
    QTest::addRow("fs")         << QFINDTESTDATA("samplefiles/text/test.fs")         << "text";
    QTest::addRow("go")         << QFINDTESTDATA("samplefiles/text/test.go")         << "text";
    QTest::addRow("groovy")     << QFINDTESTDATA("samplefiles/text/test.groovy")     << "text";
    QTest::addRow("h")          << QFINDTESTDATA("samplefiles/text/test.h")          << "text";
    QTest::addRow("hpp")        << QFINDTESTDATA("samplefiles/text/test.hpp")        << "text";
    QTest::addRow("hs")         << QFINDTESTDATA("samplefiles/text/test.hs")         << "text";
    QTest::addRow("html")       << QFINDTESTDATA("samplefiles/text/test.html")       << "text";
    QTest::addRow("ini")        << QFINDTESTDATA("samplefiles/text/test.ini")        << "text";
    QTest::addRow("java")       << QFINDTESTDATA("samplefiles/text/test.java")       << "text";
    QTest::addRow("jl")         << QFINDTESTDATA("samplefiles/text/test.jl")         << "text";
    QTest::addRow("js")         << QFINDTESTDATA("samplefiles/text/test.js")         << "text";
    QTest::addRow("jsx")        << QFINDTESTDATA("samplefiles/text/test.jsx")        << "text";
    QTest::addRow("kt")         << QFINDTESTDATA("samplefiles/text/test.kt")         << "text";
    QTest::addRow("less")       << QFINDTESTDATA("samplefiles/text/test.less")       << "text";
    QTest::addRow("lisp")       << QFINDTESTDATA("samplefiles/text/test.lisp")       << "text";
    QTest::addRow("lua")        << QFINDTESTDATA("samplefiles/text/test.lua")        << "text";
    QTest::addRow("m")          << QFINDTESTDATA("samplefiles/text/test.m")          << "text";
    QTest::addRow("mk")         << QFINDTESTDATA("samplefiles/text/test.mk")         << "text";
    QTest::addRow("ml")         << QFINDTESTDATA("samplefiles/text/test.ml")         << "text";
    QTest::addRow("mm")         << QFINDTESTDATA("samplefiles/text/test.mm")         << "text";
    QTest::addRow("nim")        << QFINDTESTDATA("samplefiles/text/test.nim")        << "text";
    QTest::addRow("ninja")      << QFINDTESTDATA("samplefiles/text/test.ninja")      << "text";
    QTest::addRow("pas")        << QFINDTESTDATA("samplefiles/text/test.pas")        << "text";
    QTest::addRow("php")        << QFINDTESTDATA("samplefiles/text/test.php")        << "text";
    QTest::addRow("pl")         << QFINDTESTDATA("samplefiles/text/test.pl")         << "text";
    QTest::addRow("ps1")        << QFINDTESTDATA("samplefiles/text/test.ps1")        << "text";
    QTest::addRow("py")         << QFINDTESTDATA("samplefiles/text/test.py")         << "text";
    QTest::addRow("qml")        << QFINDTESTDATA("samplefiles/text/test.qml")        << "text";
    QTest::addRow("r")          << QFINDTESTDATA("samplefiles/text/test.r")          << "text";
    QTest::addRow("rb")         << QFINDTESTDATA("samplefiles/text/test.rb")         << "text";
    QTest::addRow("rs")         << QFINDTESTDATA("samplefiles/text/test.rs")         << "text";
    QTest::addRow("rst")        << QFINDTESTDATA("samplefiles/text/test.rst")        << "text";
    QTest::addRow("rtx")        << QFINDTESTDATA("samplefiles/text/test.rtx")        << "text";
    QTest::addRow("sass")       << QFINDTESTDATA("samplefiles/text/test.sass")       << "text";
    QTest::addRow("scala")      << QFINDTESTDATA("samplefiles/text/test.scala")      << "text";
    QTest::addRow("scm")        << QFINDTESTDATA("samplefiles/text/test.scm")        << "text";
    QTest::addRow("scss")       << QFINDTESTDATA("samplefiles/text/test.scss")       << "text";
    QTest::addRow("sh")         << QFINDTESTDATA("samplefiles/text/test.sh")         << "text";
    QTest::addRow("svelte")     << QFINDTESTDATA("samplefiles/text/test.svelte")     << "text";
    QTest::addRow("swift")      << QFINDTESTDATA("samplefiles/text/test.swift")      << "text";
    QTest::addRow("tcl")        << QFINDTESTDATA("samplefiles/text/test.tcl")        << "text";
    QTest::addRow("tex")        << QFINDTESTDATA("samplefiles/text/test.tex")        << "text";
    QTest::addRow("toml")       << QFINDTESTDATA("samplefiles/text/test.toml")       << "text";
    QTest::addRow("ts")         << QFINDTESTDATA("samplefiles/text/test.ts")         << "text";
    QTest::addRow("tsx")        << QFINDTESTDATA("samplefiles/text/test.tsx")        << "text";
    QTest::addRow("txt")        << QFINDTESTDATA("samplefiles/text/test.txt")        << "text";
    QTest::addRow("vb")         << QFINDTESTDATA("samplefiles/text/test.vb")         << "text";
    QTest::addRow("yaml")       << QFINDTESTDATA("samplefiles/text/test.yaml")       << "text";
    QTest::addRow("zig")        << QFINDTESTDATA("samplefiles/text/test.zig")        << "text";
    QTest::addRow("zsh")        << QFINDTESTDATA("samplefiles/text/test.zsh")        << "text";

    //Video Files
    QTest::addRow("avi")  << QFINDTESTDATA("samplefiles/video/test.avi")  << "video";
    QTest::addRow("mkv")  << QFINDTESTDATA("samplefiles/video/test.mkv")  << "video";
    QTest::addRow("mov")  << QFINDTESTDATA("samplefiles/video/test.mov")  << "video";
    QTest::addRow("mp4")  << QFINDTESTDATA("samplefiles/video/test.mp4")  << "video";
    QTest::addRow("mpg")  << QFINDTESTDATA("samplefiles/video/test.mpg")  << "video";
    QTest::addRow("vob")  << QFINDTESTDATA("samplefiles/video/test.vob")  << "video";
    QTest::addRow("webm") << QFINDTESTDATA("samplefiles/video/test.webm") << "video";
    QTest::addRow("wmv")  << QFINDTESTDATA("samplefiles/video/test.wmv")  << "video";
}

QTEST_GUILESS_MAIN(FileRoutingTest)

#include "FileRoutingTest.moc"
