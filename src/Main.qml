// SPDX-FileCopyrightText: 2026 Alex Huber <alex.huber743@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.ApplicationWindow {
    id: root

    width: 640
    height: 360

    title: i18nc("@title:window", "Maki Preview")

    visible: WindowManager.visible

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
            case "image"   : return "ImagePreview.qml"
            case "text"    : return "TextPreview.qml"
            case "audio"   : return "AudioPreview.qml"
            case "video"   : return "VideoPreview.qml"
            case "pdf"     : return "DocumentPreview.qml"
            case "archive" : return "ArchivePreview.qml"
            default        : return "FallbackPreview.qml"
            }
        }
    }
}