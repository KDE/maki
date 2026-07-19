// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Button {
    property string appName: ""
    
    property bool hide: true
    property int fadeDuration: Kirigami.Units.longDuration
    property int hoverMargin: Kirigami.Units.largeSpacing * 2

    implicitWidth: contentItem.implicitWidth + 2 * Kirigami.Units.largeSpacing
    
    contentItem: Text {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing

        text: i18n("Open in %1", appName)
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: Kirigami.Theme.textColor
    }


    opacity: {
        if (hide) {
            return buttonHover.hovered ? 1.0 : 0.0;
        } else {
            return 1.0
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: fadeDuration }
    }

    onClicked: {
        WindowManager.openInDefaultApp();
    }

    HoverHandler {
        id: buttonHover

        margin: hoverMargin
    }
}