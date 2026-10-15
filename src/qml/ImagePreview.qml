// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Item {
    anchors.fill: parent

    readonly property ImageHandler imageHandler: WindowManager.activeHandler
    property url currentURL: imageHandler ? imageHandler.imageUrl : ""
    
    onCurrentURLChanged: {
        if (imageHandler && imageHandler.imageSize) {
            root.resizeWindow(imageHandler.imageSize)
        }
    }

    Image {
        visible: !imageHandler.isAnimated
        anchors.fill: parent
        source: parent.currentURL
        fillMode: Image.PreserveAspectFit
        mipmap: true
        autoTransform: true
    }
    
    AnimatedImage {
        visible: imageHandler.isAnimated
        anchors.fill: parent
        source: parent.currentURL
        fillMode: Image.PreserveAspectFit
        mipmap: true
    }
}