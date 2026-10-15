// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Item {
    anchors.fill: parent

    readonly property FallbackHandler fallbackHandler: WindowManager.activeHandler
    
    Kirigami.Icon {
        anchors.fill: parent
        source: fallbackHandler ? fallbackHandler.fallbackIcon : ""
        visible: fallbackHandler ? fallbackHandler.fallbackIcon !== "" : false

        Component.onCompleted: {
            //TODO: Move this to a config
            root.resizeWindow(Qt.size(512, 512));
        }
    }
    Label {
        anchors.centerIn: parent
        text: "No Preview available"
        visible: fallbackHandler ? fallbackHandler.fallbackIcon === "" : true
    }
}