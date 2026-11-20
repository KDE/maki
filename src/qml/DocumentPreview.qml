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

        onPasswordRequired: {
            if (passwordPrompt.visible) {
                passwordPrompt.showWrongPassword();
            } else {
                passwordPrompt.open();
            }
        }

        onStatusChanged: {
            if (status === PdfDocument.Ready) {
                if (passwordPrompt.visible) {
                    passwordPrompt.wrongPassword = false;
                    passwordPrompt.close();
                }
            }
        }
    }
    
    Kirigami.Dialog {
        id: passwordPrompt

        title: i18nc("@title", "Password required")
        standardButtons: Dialog.NoButton
        modal: true
        closePolicy: Popup.NoAutoClose
        showCloseButton: false

        property bool wrongPassword: false

        function showWrongPassword() {
            passwordPrompt.wrongPassword = true
            shakeAnimation.restart();
        }

        function submitPassword() {
            if (passwordInput.text === doc.password) {
                shakeAnimation.restart();
                return;
            }
            doc.password = passwordInput.text;
            passwordInput.selectAll();
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: Kirigami.Units.smallSpacing

            Label {
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.mediumSpacing
                Layout.rightMargin: Kirigami.Units.mediumSpacing
                text: i18nc("@info", "This PDF is password protected. Enter the password to view it.")
                wrapMode: TextInput.Wrap
                visible: !passwordPrompt.wrongPassword
            }
            
            Label {
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.mediumSpacing
                Layout.rightMargin: Kirigami.Units.mediumSpacing
                text: i18nc("@info:status", "Wrong Password, try again.")
                color: Kirigami.Theme.negativeTextColor
                wrapMode: TextInput.Wrap
                visible: passwordPrompt.wrongPassword
            }

            Kirigami.PasswordField {
                id: passwordInput

                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.mediumSpacing
                Layout.rightMargin: Kirigami.Units.mediumSpacing
                placeholderText: i18nc("@password", "Password")

                onAccepted: {
                    passwordPrompt.submitPassword();
                }

                transform: Translate {
                    id: shakeOffset
                }
            }

            DialogButtonBox {
                Layout.alignment: Qt.AlignRight

                standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

                onRejected: {
                    passwordPrompt.close()
                    root.close();
                }

                onAccepted: {
                    passwordPrompt.submitPassword();
                }
            }
        }
        
        SequentialAnimation {
            id: shakeAnimation
            NumberAnimation { target: shakeOffset; property: "x"; to: -Kirigami.Units.largeSpacing;     duration: Kirigami.Units.veryShortDuration;     easing.type: Easing.InOutQuad }
            NumberAnimation { target: shakeOffset; property: "x"; to:  Kirigami.Units.largeSpacing;     duration: Kirigami.Units.veryShortDuration * 2; easing.type: Easing.InOutQuad }
            NumberAnimation { target: shakeOffset; property: "x"; to: -Kirigami.Units.largeSpacing / 2; duration: Kirigami.Units.veryShortDuration * 2; easing.type: Easing.InOutQuad }
            NumberAnimation { target: shakeOffset; property: "x"; to: 0;                                duration: Kirigami.Units.veryShortDuration;     easing.type: Easing.InOutQuad }
        }
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

        const thumbnailViewSize = doc.pageCount > 1 ? 100 : 0;

        root.resizeWindow(Qt.size(windowWidth + thumbnailViewSize, windowHeight));
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

            visible: doc.pageCount > 1
        }

        ListView {
            id: thumbnailList

            Layout.preferredWidth: 100
            Layout.fillHeight: true

            model: doc.status === PdfDocument.Ready ? doc.pageCount : 0

            clip: true

            visible: doc.pageCount > 1

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