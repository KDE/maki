// SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Item {
    id: documentPreview
    anchors.fill: parent

    property int mainViewWidth: 0
    property int separatorWidth: 1
    property int sidebarWidth: 100

    Component.onCompleted: {
        //TODO: Put this into a config
        const maxPageScale = Qt.size(595, 842);
        const pageSize = Qt.size(WindowManager.windowSize.width, WindowManager.windowSize.height);

        const rw = maxPageScale.height * (pageSize.width / pageSize.height);

        const useHeight = rw <= maxPageScale.width;

        let windowHeight = 0;

        if (useHeight) {
            windowHeight = maxPageScale.height;
            documentPreview.mainViewWidth = rw;
        } else {
            windowHeight = maxPageScale.width * (pageSize.height / pageSize.width);
            documentPreview.mainViewWidth = maxPageScale.width;
        }

        root.width = documentPreview.mainViewWidth 
                   + documentPreview.sidebarWidth 
                   + documentPreview.separatorWidth;
        root.height = windowHeight;
    }

    ScrollView {
        id: mainView
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: root.width - documentPreview.sidebarWidth;
        
        ListView {
            id: mainList
            anchors.fill: parent
            anchors.rightMargin: mainView.ScrollBar.vertical.width + Kirigami.Units.smallSpacing
            anchors.leftMargin: Kirigami.Units.smallSpacing
            anchors.topMargin: Kirigami.Units.smallSpacing
            anchors.bottomMargin: Kirigami.Units.smallSpacing
            width: parent.width
            model: WindowManager.pageNum
            spacing: 8

            delegate: Image {
                width: mainList.width
                height: width * (sourceSize.height / sourceSize.width)
                fillMode: Image.PreserveAspectFit
                source: "image://Maki/" + (index + 1)
                mipmap: true
                cache: false
            }
        }
    }

    Kirigami.Separator {
        id: separator
        anchors.left: mainView.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        
        anchors.rightMargin: Kirigami.Units.smallSpacing

        width: documentPreview.separatorWidth
    }
    
    Rectangle {
        id: thumbnailSidebar
        anchors.left: separator.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: documentPreview.sidebarWidth;
        color: Kirigami.Theme.backgroundColor
        
        ScrollView {
            id: thumbnailView
            anchors.fill: parent
        
            ListView {
                id: thumbnailList
                width: parent.width
                anchors.fill: parent
                anchors.rightMargin: thumbnailView.ScrollBar.vertical.width + Kirigami.Units.smallSpacing 
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.topMargin: Kirigami.Units.smallSpacing
                anchors.bottomMargin: Kirigami.Units.smallSpacing
                model: WindowManager.pageNum
                spacing: Kirigami.Units.smallSpacing

                delegate: Item {
                    width: thumbnailList.width
                    height: thumbnailBackground.height
                    
                    Rectangle {
                        id: thumbnailBackground
                        width: parent.width
                        implicitHeight: thumbnailStack.height + Kirigami.Units.smallSpacing * 2
                        color: thumbnailMouseManager.containsMouse ? Kirigami.Theme.activeBackgroundColor : "transparent"
                        border.width: 1
                        border.color: thumbnailMouseManager.containsMouse ? Kirigami.Theme.highlightColor : "transparent"
                        radius: Kirigami.Units.cornerRadius
                        
                        Column {
                            id: thumbnailStack
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.margins: Kirigami.Units.smallSpacing
                            spacing: Kirigami.Units.smallSpacing
                                
                            Image {
                                width: parent.width
                                height: width * (WindowManager.windowSize.height / WindowManager.windowSize.width)
                                fillMode: Image.PreserveAspectFit
                                source: "image://Maki/" + (index + 1)
                                mipmap: true
                                cache: false
                            }

                            Label {
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter
                                text: index + 1
                                color: Kirigami.Theme.textColor
                            }
                        }
                    }

                    MouseArea {
                        id: thumbnailMouseManager
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            mainList.currentIndex = index;
                            mainList.positionViewAtIndex(index, ListView.Beginning);
                        }
                    }
                } 
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