// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigami.platform as Platform

Item {
    height: footerText.implicitHeight + Kirigami.Units.smallSpacing
    width: parent.width

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

        ToolTip {
            id: toolTip

            text: footerText.text
        }

        HoverHandler {
            //Only needs to show tooltip if text is not fully visible
            enabled: footerText.truncated

            onHoveredChanged: {
                toolTip.visible = hovered;
            }
        }
    }
} 