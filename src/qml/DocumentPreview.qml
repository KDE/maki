// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Item {
    id: documentPreview
    anchors.fill: parent

    Component.onCompleted: {
        //TODO: Put this into a config
        const maxPageScale = Qt.size(595, 842);
        const pageSize = Qt.size(WindowManager.windowSize.width, WindowManager.windowSize.height);

        const rw = maxPageScale.height * (pageSize.width / pageSize.height);

        const useHeight = rw <= maxPageScale.width;

        let windowHeight = 0;
        let windowWidth = 0;

        if (useHeight) {
            windowHeight = maxPageScale.height;
            windowWidth = rw;
        } else {
            windowHeight = maxPageScale.width * (pageSize.height / pageSize.width);
            windowWidth = maxPageScale.width;
        }

        root.width = windowWidth + 100;
        root.height = windowHeight;
    }

    RowLayout {
        spacing: Kirigami.Units.smallSpacing

        anchors.fill: parent

        anchors.margins: Kirigami.Units.smallSpacing

        ListView {
            id: mainList

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: WindowManager.pageNum
            spacing: Kirigami.Units.smallSpacing

            delegate: Image {
                width: mainList.width
                height: width * (sourceSize.height / sourceSize.width)
                fillMode: Image.PreserveAspectFit
                source: "image://Maki/" + (index + 1)
            }

            Kirigami.WheelHandler {
                id: mainListWheelHandler
                target: mainList
                keyNavigationEnabled: true
            }
        }

        Kirigami.Separator {
            id: separator

            Layout.fillHeight: true
        }

        ListView {
            id: thumbnailList

            Layout.preferredWidth: 100
            Layout.fillHeight: true

            model: WindowManager.pageNum

            delegate: ItemDelegate {
                id: delegate

                width: ListView.view.width

                contentItem: ColumnLayout {
                    id: thumbnailBackground

                    width: delegate.width

                    spacing: Kirigami.Units.smallSpacing

                    Image {
                        fillMode: Image.PreserveAspectFit
                        source: "image://Maki/" + (index + 1)
                        
                        Layout.fillWidth: true
                        Layout.preferredHeight: width * (sourceSize.height / sourceSize.width)
                    }

                    Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: index + 1
                        color: Kirigami.Theme.textColor

                        Layout.fillWidth: true
                    }
                }

                onClicked: {
                    mainList.currentIndex = index;
                    mainList.positionViewAtIndex(index, ListView.Beginning);
                }
            }

            Kirigami.WheelHandler {
                id: thumbnailListWheelHandler
                target: mainList
                keyNavigationEnabled: true
            }
        }
    }


    Button {
        anchors.top: parent.top
        anchors.left: parent.left
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