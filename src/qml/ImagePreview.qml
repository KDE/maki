// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Item {
    anchors.fill: parent

    readonly property ImageHandler imageHandler: WindowManager.activeHandler
    
    AnimatedImage {
        anchors.fill: parent
        source: imageHandler ? imageHandler.imageUrl : ""
        fillMode: Image.PreserveAspectFit
        mipmap: true

        property url currentURL: imageHandler ? imageHandler.imageUrl : ""

        onCurrentURLChanged: {
            if (imageHandler && imageHandler.imageSize) {
                root.width = imageHandler.imageSize.width
                root.height = imageHandler.imageSize.height
            }
        }
    }

    DefaultAppButton {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing
        
        appName: WindowManager.defaultAppName
    }
}