// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

RowLayout {

    anchors.fill: parent

    Component.onCompleted: {
        root.width = 600;
        root.height = 320;
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 8

        Image {
            source: WindowManager.infoPanelURL
            fillMode: Image.PreserveAspectFit
        }

        Text {
            text: WindowManager.fileName
            color: Kirigami.Theme.textColor
        }
    }

    Kirigami.Separator {
        Layout.preferredWidth: implicitWidth
        Layout.fillHeight: true
    }

    TreeView {
        Layout.fillHeight: true
        Layout.fillWidth: true

        Layout.minimumWidth: root.width * (2/5)

        contentWidth: width

        model: WindowManager.archiveModel

        delegate: TreeViewDelegate {
            implicitWidth: parent.width
            implicitHeight: 32

            contentItem: Item {
                Row {
                    spacing: 8

                    Kirigami.Icon {
                        height: 16
                        width: 16
                        source: model.toolTip
                    }

                    Text {
                        text: model.display
                        color: Kirigami.Theme.textColor
                        wrapMode: Text.Wrap
                    }
                }
            }
        }
    }
}