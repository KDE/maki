// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

RowLayout {
    anchors.fill: parent

    spacing: 0

    Component.onCompleted: {
        root.width = 600;
        root.height = 320;
    }

    Rectangle {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.fillHeight: true
        Layout.preferredWidth: 150

        color: Qt.lighter(Kirigami.Theme.backgroundColor, 1.1)

        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width

            Image {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                source: WindowManager.infoPanelURL
                fillMode: Image.PreserveAspectFit
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: WindowManager.fileName
                color: Kirigami.Theme.textColor
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
            }
            
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: WindowManager.fileSize
                color: Kirigami.Theme.textColor
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Kirigami.Separator {
        Layout.fillHeight: true
    }

    ScrollView {
        id: archiveTreeScrollView

        Layout.fillHeight: true
        Layout.fillWidth: true

        TreeView {
            id: archiveTree

            contentWidth: width

            model: WindowManager.archiveModel

            delegate: TreeViewDelegate {
                implicitWidth: parent.width
                implicitHeight: contentItem.height

                clip: true
                contentItem: RowLayout {
                    spacing: Kirigami.Units.largeSpacing
                    width: parent.width
                    height: fileIcon.height + Kirigami.Units.largeSpacing

                    Kirigami.Icon {
                        id: fileIcon

                        //TODO: Icon Size should go into a config.
                        Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                        Layout.preferredWidth: Kirigami.Units.iconSizes.medium
                        source: model.toolTip
                    }

                    Text {
                        id: fileName

                        Layout.fillWidth: true
                        text: model.display
                        color: Kirigami.Theme.textColor
                        elide: Text.ElideRight
                        maximumLineCount: 2
                    }
                    
                    Text {
                        id: fileSize

                        Layout.preferredWidth: contentWidth
                        Layout.rightMargin: Kirigami.Units.largeSpacing
                        text: model.statusTip
                        color: Kirigami.Theme.textColor
                    }
                }
            }
        }
    }
}