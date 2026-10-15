// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Pdf
import org.kde.kirigami as Kirigami

Item {
    id: documentPreview
    anchors.fill: parent

    readonly property DocumentHandler documentHandler: WindowManager.activeHandler

    readonly property url documentUrl: documentHandler ? documentHandler.documentUrl : ""

    PdfDocument {
        id: doc
        source: documentUrl
    }

    readonly property size firstPageSize: doc.status === PdfDocument.Ready ? doc.pagePointSize(0) : Qt.size(1, 1)

    onFirstPageSizeChanged: {
        //TODO: Put this into a config
        const maxPageScale = Qt.size(root.screen.width * 0.8, root.screen.height * 0.8);
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

                model: doc.status === PdfDocument.Ready ? doc.pageCount : 0
                spacing: Kirigami.Units.smallSpacing

                clip: true

                delegate: Rectangle {
                    width: mainList.width
                    height: width * (documentPreview.firstPageSize.height / documentPreview.firstPageSize.width)
                    color: "white"

                    PdfPageImage {
                        anchors.fill: parent
                        document: doc
                        currentFrame: index
                        fillMode: Image.PreserveAspectFit
                        sourceSize.width: width * Screen.devicePixelRatio
                        sourceSize.height: height * Screen.devicePixelRatio
                        cache: false
                    }
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

            model: doc.status === PdfDocument.Ready ? doc.pageCount : 0

            clip: true

            delegate: ItemDelegate {
                id: delegate

                width: ListView.view.width

                contentItem: ColumnLayout {
                    id: thumbnailBackground

                    width: delegate.width

                    spacing: Kirigami.Units.smallSpacing

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: width * (documentPreview.firstPageSize.height / documentPreview.firstPageSize.width)

                        color: "white"

                        PdfPageImage {
                            anchors.fill: parent
                            document: doc
                            currentFrame: index
                            fillMode: Image.PreserveAspectFit
                            cache: false
                        }
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

    //Loading Screen
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