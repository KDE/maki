// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-FileCopyrightText: 2026 Kristen McWilliam <kristen@kde.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

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

        source: {
            switch(WindowManager.fileKey) {
            case HandlerUtils.HandlerKey.Image    : return "ImagePreview.qml"
            case HandlerUtils.HandlerKey.Text     : return "TextPreview.qml"
            case HandlerUtils.HandlerKey.Audio    : return "AudioPreview.qml"
            case HandlerUtils.HandlerKey.Video    : return "VideoPreview.qml"
            case HandlerUtils.HandlerKey.Document : return "DocumentPreview.qml"
            case HandlerUtils.HandlerKey.Archive  : return "ArchivePreview.qml"

            default         : return "FallbackPreview.qml"
            }
        }
    }
}