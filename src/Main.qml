// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-FileCopyrightText: 2026 Kristen McWilliam <kristen@kde.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigami.platform as Platform

Kirigami.ApplicationWindow {
    id: root

    width: 640
    height: 360

    title: WindowManager.fileName

    visible: WindowManager.visible

    header: HeaderBar {}

    //Show the file path in the footer
    footer: FooterBar {}

    Shortcut {
        sequence: "Space"
        context: Qt.ApplicationShortcut
        onActivated: WindowManager.visible = false
    }

    onClosing: (close) => {
        close.accepted = false
        WindowManager.visible = false
        WindowManager.releaseResources()
    }

    Loader {
        anchors.fill: parent

        active: WindowManager.fileType !== "" 

        source: {
            switch(WindowManager.fileType) {
            case "image"    : return "ImagePreview.qml"
            case "text"     : return "TextPreview.qml"
            case "audio"    : return "AudioPreview.qml"
            case "video"    : return "VideoPreview.qml"
            case "document" : return "DocumentPreview.qml"
            case "archive"  : return "ArchivePreview.qml"

            default         : return "FallbackPreview.qml"
            }
        }
    }
}