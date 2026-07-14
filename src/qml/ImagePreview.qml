// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

Item {
    anchors.fill: parent

    readonly property ImageHandler imageHandler: WindowManager.activeHandler
    
    Image {
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

    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 8

        text: i18n("open")

        opacity: buttonHover.hovered ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }

        onClicked: {
            console.log("clicked");
            WindowManager.openInDefaultApp();
        }

        HoverHandler {
            id: buttonHover

            margin: 64
        }
    }
}