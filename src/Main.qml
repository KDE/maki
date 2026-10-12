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

    //Show the file path in the footer
    footer: Item {
        height: footerText.implicitHeight + Kirigami.Units.smallSpacing
        width: root.width

        Rectangle {
            width: parent.width
            height: 1
            color: Platform.ColorUtils.linearInterpolation(
                    Platform.Theme.backgroundColor,
                    Platform.Theme.textColor,
                    Platform.Theme.frameContrast)
        }

        Text {
            id: footerText
            anchors.fill: parent
            anchors.margins: Kirigami.Units.mediumSpacing
            text: stripURL(WindowManager.url)
            color: Kirigami.Theme.textColor
            elide: Text.ElideLeft
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter

            //Directly assigning to text throws an error, because WindowManager.url is empty at first, so that needs to be checked.
            function stripURL(fileUrl) {
                if (fileUrl === "" || fileUrl === Qt.url("") || fileUrl === undefined || fileUrl === null) {
                    return "";
                }

                return decodeURIComponent(new URL(fileUrl).pathname);
            }
        }
    } 

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