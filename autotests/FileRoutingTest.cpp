// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>

#include "../src/backend/WindowManager.h"

class FileRoutingTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRouting();
    void testRouting_data();
};

void FileRoutingTest::testRouting()
{
    QFETCH(QString, fileName);
    QFETCH(HandlerUtils::HandlerKey, expectedHandlerKey);

    WindowManager windowManager;
    // Necessary right now to construct all handlers.
    // Image Provider will be removed at some point
    MakiImageProvider imageProvider;
    windowManager.setImageProvider(&imageProvider);
    windowManager.setURL(QUrl::fromLocalFile(fileName));

    QVERIFY2(windowManager.fileKey() == expectedHandlerKey, "File is routed to the wrong handler");
}

void FileRoutingTest::testRouting_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<HandlerUtils::HandlerKey>("expectedHandlerKey");

    // clang-format off
    // Archives
    QTest::addRow("7z")       << QFINDTESTDATA("samplefiles/archive/test.7z")       << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("tar")      << QFINDTESTDATA("samplefiles/archive/test.tar")      << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("tar.bz2")  << QFINDTESTDATA("samplefiles/archive/test.tar.bz2")  << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("tar.gz")   << QFINDTESTDATA("samplefiles/archive/test.tar.gz")   << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("tar.lz")   << QFINDTESTDATA("samplefiles/archive/test.tar.lz")   << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("tar.xz")   << QFINDTESTDATA("samplefiles/archive/test.tar.xz")   << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("tar.zst")  << QFINDTESTDATA("samplefiles/archive/test.tar.zst")  << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("txt.bz2")  << QFINDTESTDATA("samplefiles/archive/test.txt.bz2")  << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("txt.gz")   << QFINDTESTDATA("samplefiles/archive/test.txt.gz")   << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("txt.lz")   << QFINDTESTDATA("samplefiles/archive/test.txt.lz")   << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("txt.lzma") << QFINDTESTDATA("samplefiles/archive/test.txt.lzma") << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("txt.xz")   << QFINDTESTDATA("samplefiles/archive/test.txt.xz")   << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("txt.zst")  << QFINDTESTDATA("samplefiles/archive/test.txt.zst")  << HandlerUtils::HandlerKey::Archive;
    QTest::addRow("zip")      << QFINDTESTDATA("samplefiles/archive/test.zip")      << HandlerUtils::HandlerKey::Archive;
    
    // Audio Files
    QTest::addRow("aiff")      << QFINDTESTDATA("samplefiles/audio/test.aiff")      << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("ape")       << QFINDTESTDATA("samplefiles/audio/test.ape")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("flac")      << QFINDTESTDATA("samplefiles/audio/test.flac")      << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("flac.ogg")  << QFINDTESTDATA("samplefiles/audio/test.flac.ogg")  << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("m4a")       << QFINDTESTDATA("samplefiles/audio/test.m4a")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("mp3")       << QFINDTESTDATA("samplefiles/audio/test.mp3")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("mpc")       << QFINDTESTDATA("samplefiles/audio/test.mpc")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("ogg")       << QFINDTESTDATA("samplefiles/audio/test.ogg")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("opus")      << QFINDTESTDATA("samplefiles/audio/test.opus")      << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("opus.ogg")  << QFINDTESTDATA("samplefiles/audio/test.opus.ogg")  << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("speex.ogg") << QFINDTESTDATA("samplefiles/audio/test.speex.ogg") << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("spx")       << QFINDTESTDATA("samplefiles/audio/test.spx")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("wav")       << QFINDTESTDATA("samplefiles/audio/test.wav")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("wma")       << QFINDTESTDATA("samplefiles/audio/test.wma")       << HandlerUtils::HandlerKey::Audio;
    QTest::addRow("wv")        << QFINDTESTDATA("samplefiles/audio/test.wv")        << HandlerUtils::HandlerKey::Audio;
    
    // Document Files
    QTest::addRow("doc")  << QFINDTESTDATA("samplefiles/document/test.doc")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("docx") << QFINDTESTDATA("samplefiles/document/test.docx") << HandlerUtils::HandlerKey::Document;
    // QTest::addRow("epub") << QFINDTESTDATA("samplefiles/document/test.epub") << HandlerUtils::HandlerKey::Document; // EPUB support in planning
    QTest::addRow("odp")  << QFINDTESTDATA("samplefiles/document/test.odp")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("ods")  << QFINDTESTDATA("samplefiles/document/test.ods")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("odt")  << QFINDTESTDATA("samplefiles/document/test.odt")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("pdf")  << QFINDTESTDATA("samplefiles/document/test.pdf")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("ppt")  << QFINDTESTDATA("samplefiles/document/test.ppt")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("pptx") << QFINDTESTDATA("samplefiles/document/test.pptx") << HandlerUtils::HandlerKey::Document;
    QTest::addRow("xls")  << QFINDTESTDATA("samplefiles/document/test.xls")  << HandlerUtils::HandlerKey::Document;
    QTest::addRow("xlsx") << QFINDTESTDATA("samplefiles/document/test.xlsx") << HandlerUtils::HandlerKey::Document;

    // Image Files
    QTest::addRow("avif") << QFINDTESTDATA("samplefiles/image/test.avif") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("bmp")  << QFINDTESTDATA("samplefiles/image/test.bmp")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("gif")  << QFINDTESTDATA("samplefiles/image/test.gif")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("heic") << QFINDTESTDATA("samplefiles/image/test.heic") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("heif") << QFINDTESTDATA("samplefiles/image/test.heif") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("icns") << QFINDTESTDATA("samplefiles/image/test.icns") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("ico")  << QFINDTESTDATA("samplefiles/image/test.ico")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("jpeg") << QFINDTESTDATA("samplefiles/image/test.jpeg") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("jpg")  << QFINDTESTDATA("samplefiles/image/test.jpg")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("png")  << QFINDTESTDATA("samplefiles/image/test.png")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("svg")  << QFINDTESTDATA("samplefiles/image/test.svg")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("tiff") << QFINDTESTDATA("samplefiles/image/test.tiff") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("webp") << QFINDTESTDATA("samplefiles/image/test.webp") << HandlerUtils::HandlerKey::Image;
    QTest::addRow("xbm")  << QFINDTESTDATA("samplefiles/image/test.xbm")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("xpm")  << QFINDTESTDATA("samplefiles/image/test.xpm")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("kra")  << QFINDTESTDATA("samplefiles/image/test.kra")  << HandlerUtils::HandlerKey::Image;
    QTest::addRow("krz")  << QFINDTESTDATA("samplefiles/image/test.krz")  << HandlerUtils::HandlerKey::Image;

    // Text Files
    QTest::addRow("meson")      << QFINDTESTDATA("samplefiles/text/meson.build")     << HandlerUtils::HandlerKey::Text;
    QTest::addRow("adb")        << QFINDTESTDATA("samplefiles/text/test.adb")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("asm")        << QFINDTESTDATA("samplefiles/text/test.asm")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("c")          << QFINDTESTDATA("samplefiles/text/test.c")          << HandlerUtils::HandlerKey::Text;
    QTest::addRow("cfg")        << QFINDTESTDATA("samplefiles/text/test.cfg")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("clj")        << QFINDTESTDATA("samplefiles/text/test.clj")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("cmake")      << QFINDTESTDATA("samplefiles/text/test.cmake")      << HandlerUtils::HandlerKey::Text;
    QTest::addRow("cpp")        << QFINDTESTDATA("samplefiles/text/test.cpp")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("cr")         << QFINDTESTDATA("samplefiles/text/test.cr")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("cs")         << QFINDTESTDATA("samplefiles/text/test.cs")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("css")        << QFINDTESTDATA("samplefiles/text/test.css")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("d")          << QFINDTESTDATA("samplefiles/text/test.d")          << HandlerUtils::HandlerKey::Text;
    QTest::addRow("dart")       << QFINDTESTDATA("samplefiles/text/test.dart")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("dockerfile") << QFINDTESTDATA("samplefiles/text/test.dockerfile") << HandlerUtils::HandlerKey::Text;
    QTest::addRow("erl")        << QFINDTESTDATA("samplefiles/text/test.erl")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("ex")         << QFINDTESTDATA("samplefiles/text/test.ex")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("f")          << QFINDTESTDATA("samplefiles/text/test.f")          << HandlerUtils::HandlerKey::Text;
    QTest::addRow("fish")       << QFINDTESTDATA("samplefiles/text/test.fish")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("fs")         << QFINDTESTDATA("samplefiles/text/test.fs")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("go")         << QFINDTESTDATA("samplefiles/text/test.go")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("groovy")     << QFINDTESTDATA("samplefiles/text/test.groovy")     << HandlerUtils::HandlerKey::Text;
    QTest::addRow("h")          << QFINDTESTDATA("samplefiles/text/test.h")          << HandlerUtils::HandlerKey::Text;
    QTest::addRow("hpp")        << QFINDTESTDATA("samplefiles/text/test.hpp")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("hs")         << QFINDTESTDATA("samplefiles/text/test.hs")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("html")       << QFINDTESTDATA("samplefiles/text/test.html")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("ini")        << QFINDTESTDATA("samplefiles/text/test.ini")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("java")       << QFINDTESTDATA("samplefiles/text/test.java")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("jl")         << QFINDTESTDATA("samplefiles/text/test.jl")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("js")         << QFINDTESTDATA("samplefiles/text/test.js")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("jsx")        << QFINDTESTDATA("samplefiles/text/test.jsx")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("kt")         << QFINDTESTDATA("samplefiles/text/test.kt")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("less")       << QFINDTESTDATA("samplefiles/text/test.less")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("lisp")       << QFINDTESTDATA("samplefiles/text/test.lisp")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("lua")        << QFINDTESTDATA("samplefiles/text/test.lua")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("m")          << QFINDTESTDATA("samplefiles/text/test.m")          << HandlerUtils::HandlerKey::Text;
    QTest::addRow("mk")         << QFINDTESTDATA("samplefiles/text/test.mk")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("ml")         << QFINDTESTDATA("samplefiles/text/test.ml")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("mm")         << QFINDTESTDATA("samplefiles/text/test.mm")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("nim")        << QFINDTESTDATA("samplefiles/text/test.nim")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("ninja")      << QFINDTESTDATA("samplefiles/text/test.ninja")      << HandlerUtils::HandlerKey::Text;
    QTest::addRow("pas")        << QFINDTESTDATA("samplefiles/text/test.pas")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("php")        << QFINDTESTDATA("samplefiles/text/test.php")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("pl")         << QFINDTESTDATA("samplefiles/text/test.pl")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("ps1")        << QFINDTESTDATA("samplefiles/text/test.ps1")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("py")         << QFINDTESTDATA("samplefiles/text/test.py")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("qml")        << QFINDTESTDATA("samplefiles/text/test.qml")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("r")          << QFINDTESTDATA("samplefiles/text/test.r")          << HandlerUtils::HandlerKey::Text;
    QTest::addRow("rb")         << QFINDTESTDATA("samplefiles/text/test.rb")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("rs")         << QFINDTESTDATA("samplefiles/text/test.rs")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("rst")        << QFINDTESTDATA("samplefiles/text/test.rst")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("rtx")        << QFINDTESTDATA("samplefiles/text/test.rtx")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("sass")       << QFINDTESTDATA("samplefiles/text/test.sass")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("scala")      << QFINDTESTDATA("samplefiles/text/test.scala")      << HandlerUtils::HandlerKey::Text;
    QTest::addRow("scm")        << QFINDTESTDATA("samplefiles/text/test.scm")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("scss")       << QFINDTESTDATA("samplefiles/text/test.scss")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("sh")         << QFINDTESTDATA("samplefiles/text/test.sh")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("svelte")     << QFINDTESTDATA("samplefiles/text/test.svelte")     << HandlerUtils::HandlerKey::Text;
    QTest::addRow("swift")      << QFINDTESTDATA("samplefiles/text/test.swift")      << HandlerUtils::HandlerKey::Text;
    QTest::addRow("tcl")        << QFINDTESTDATA("samplefiles/text/test.tcl")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("tex")        << QFINDTESTDATA("samplefiles/text/test.tex")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("toml")       << QFINDTESTDATA("samplefiles/text/test.toml")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("ts")         << QFINDTESTDATA("samplefiles/text/test.ts")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("tsx")        << QFINDTESTDATA("samplefiles/text/test.tsx")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("txt")        << QFINDTESTDATA("samplefiles/text/test.txt")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("vb")         << QFINDTESTDATA("samplefiles/text/test.vb")         << HandlerUtils::HandlerKey::Text;
    QTest::addRow("yaml")       << QFINDTESTDATA("samplefiles/text/test.yaml")       << HandlerUtils::HandlerKey::Text;
    QTest::addRow("zig")        << QFINDTESTDATA("samplefiles/text/test.zig")        << HandlerUtils::HandlerKey::Text;
    QTest::addRow("zsh")        << QFINDTESTDATA("samplefiles/text/test.zsh")        << HandlerUtils::HandlerKey::Text;

    // Video Files
    QTest::addRow("avi")  << QFINDTESTDATA("samplefiles/video/test.avi")  << HandlerUtils::HandlerKey::Video;
    QTest::addRow("mkv")  << QFINDTESTDATA("samplefiles/video/test.mkv")  << HandlerUtils::HandlerKey::Video;
    QTest::addRow("mov")  << QFINDTESTDATA("samplefiles/video/test.mov")  << HandlerUtils::HandlerKey::Video;
    QTest::addRow("mp4")  << QFINDTESTDATA("samplefiles/video/test.mp4")  << HandlerUtils::HandlerKey::Video;
    QTest::addRow("mpg")  << QFINDTESTDATA("samplefiles/video/test.mpg")  << HandlerUtils::HandlerKey::Video;
    QTest::addRow("vob")  << QFINDTESTDATA("samplefiles/video/test.vob")  << HandlerUtils::HandlerKey::Video;
    QTest::addRow("webm") << QFINDTESTDATA("samplefiles/video/test.webm") << HandlerUtils::HandlerKey::Video;
    QTest::addRow("wmv")  << QFINDTESTDATA("samplefiles/video/test.wmv")  << HandlerUtils::HandlerKey::Video;
    // clang-format on
}

QTEST_GUILESS_MAIN(FileRoutingTest)

#include "FileRoutingTest.moc"
