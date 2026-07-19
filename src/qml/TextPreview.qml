// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami
import org.kde.syntaxhighlighting

Item {
    anchors.fill: parent

    readonly property TextHandler textHandler : WindowManager.activeHandler

    ScrollView {
        id: textAreaScrollView

        anchors.fill: parent

        TextArea {
            id: textArea

            readOnly: true
            text: textHandler ? textHandler.text : ""
            color: Kirigami.Theme.textColor
        }

        SyntaxHighlighter {
            textEdit: textArea
            definition: textHandler ? Repository.definitionForFileName(textHandler.fileName) : "C++"
            theme: {
                const isDarkMode = Kirigami.Theme.backgroundColor.hslLightness < 0.5;
                return Repository.defaultTheme(
                    isDarkMode ? Repository.DarkTheme
                               : Repository.LightTheme
                );
            }
        }
    }

    DefaultAppButton {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing + textAreaScrollView.effectiveScrollBarWidth
        anchors.bottomMargin: Kirigami.Units.largeSpacing
        
        appName: WindowManager.defaultAppName
    }
}