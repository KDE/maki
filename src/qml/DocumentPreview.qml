// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Item {
    id: documentPreview
    anchors.fill: parent

    readonly property DocumentHandler documentHandler: WindowManager.activeHandler

    property size firstPageSize: documentHandler ? documentHandler.pageSize : Qt.size(100, 100)

    onFirstPageSizeChanged: {
        //TODO: Put this into a config
        const maxPageScale = Qt.size(595, 842);
        const pageSize = firstPageSize

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

        root.resizeWindow(Qt.size(windowWidth + 100, windowHeight));
    }

    RowLayout {
        id: documentPreviewLayout
    
        spacing: Kirigami.Units.smallSpacing

        anchors.fill: parent

        anchors.margins: Kirigami.Units.smallSpacing

        Item {
            id: mainListWrapper

            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: mainList

                anchors.fill: parent

                model: documentHandler ? documentHandler.pageNum : 0
                spacing: Kirigami.Units.smallSpacing

                delegate: Image {
                    width: mainList.width
                    height: width * (sourceSize.height / sourceSize.width)
                    fillMode: Image.PreserveAspectFit
                    source: "image://Maki/" + (index + 1)
                    cache: false
                }

                Kirigami.WheelHandler {
                    id: mainListWheelHandler
                    target: mainList
                    keyNavigationEnabled: true
                }
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

            model: documentHandler ? documentHandler.pageNum : 0

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
                        cache: false
                        
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

    Rectangle {
        anchors.fill: parent

        color: Kirigami.Theme.backgroundColor;

        opacity: documentHandler.isLoading ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: Kirigami.Units.veryLongDuration
            }
        }

        Label {
            anchors.fill: parent

            text: i18n("Loading...")

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}