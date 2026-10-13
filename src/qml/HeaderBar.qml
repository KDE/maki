// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

ToolBar {
    height: defaultAppButton.implicitHeight + 2 * Kirigami.Units.mediumSpacing

    Button {
        id: defaultAppButton
        anchors.right: aboutAppButton.left
        anchors.rightMargin: Kirigami.Units.smallSpacing

        implicitWidth: contentItem.implicitWidth + 2 * Kirigami.Units.mediumSpacing
        
        contentItem: RowLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.mediumSpacing
            spacing: Kirigami.Units.smallSpacing

            Kirigami.Icon {
                Layout.preferredWidth: parent.height
                Layout.fillHeight: true
                source: WindowManager.defaultAppIcon
                roundToIconSize: false
            }
            
            Text {
                id: buttonText
                Layout.fillWidth: true
                Layout.fillHeight: true

                text: i18n("Open in %1", WindowManager.defaultAppName)
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                color: Kirigami.Theme.textColor
            }
        }

        onClicked: {
            WindowManager.openInDefaultApp();
        }
    }

    Button {
        id: aboutAppButton
        height: defaultAppButton.height
        width: height
        anchors.right: parent.right
        icon.name: "kt-info-widget"
        icon.width: width - 2 * Kirigami.Units.mediumSpacing
        icon.height: height - 2 * Kirigami.Units.mediumSpacing
        display: AbstractButton.IconOnly

        onClicked: {
            root.pageStack.pushDialogLayer(aboutAppPage, 
                                          { 
                                            width: root.width 
                                          }, 
                                          {
                                            width: Kirigami.Units.gridUnit * 30,
                                            height: Kirigami.Units.gridUnit * 30,
                                            modality: Qt.NonModal
                                          });
        }
    }

    Component {
        id: aboutAppPage
        FormCard.AboutPage {}
    }
}