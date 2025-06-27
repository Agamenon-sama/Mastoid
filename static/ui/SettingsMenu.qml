import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: root

    property color labelColor: "#ddd"
    property color themeColor: "#4c0080"

    Rectangle {
        id: settingsOverlay

        width: parentWindow.width
        height: parentWindow.height
        x: 0
        y: 0

        color: "#70000000"

        ColorAnimation on color {
            from: "#20000000"
            to: "#70000000"
            duration: 300
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: root.destroy();
        }

        Rectangle {
            id: settingsDialog
            width: parentWindow.width * 0.7
            height: parentWindow.height * 0.8
            color: "#111"
            y: (parentWindow.height - height) /  2;
            border.width: 1
            border.color: "#888"
            clip: true

            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            NumberAnimation on y {
                from: (parentWindow.height - settingsDialog.height) / 2 - 50;
                to: (parentWindow.height - settingsDialog.height) /  2;
                duration: 300
                easing.type: Easing.OutQuad
            }

            MouseArea { // this only exists to shadow the other MouseArea
                anchors.fill: parent
            }

            Text {
                id: title

                text: "Settings"
                color: labelColor
                font.pixelSize: 28

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 40
                }

            }

            Rectangle {
                id: underline

                width: parent.width * 0.7
                height: 2

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: title.bottom
                    topMargin: 20
                }

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: Qt.rgba(themeColor.r, themeColor.g, themeColor.b, 0) }
                    GradientStop { position: 0.5; color: themeColor }
                    GradientStop { position: 1.0; color: Qt.rgba(themeColor.r, themeColor.g, themeColor.b, 0) }
                }
            }

            GridLayout {
                id: controlsLayout

                columns: 2
                rowSpacing: 30
                width: parent.width * 0.7
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: underline.bottom
                    topMargin: 40
                }

                // row 1
                Label {
                    text: "Window Width"
                    color: labelColor
                }

                SpinBox {
                    id: widthSpinBox
                    from: 0
                    to: 9999
                    value: AppConfiguration.width
                    editable: true

                    Layout.alignment: Qt.AlignRight

                    onValueModified: { AppConfiguration.width = value; }
                }

                // row 2
                Label {
                    text: "Window Height"
                    color: labelColor
                }

                SpinBox {
                    from: 0
                    to: 9999
                    value: AppConfiguration.height
                    editable: true

                    Layout.alignment: Qt.AlignRight

                    onValueModified: { AppConfiguration.height = value; }
                }

                // row 3
                Label {
                    text: "Base Directory"
                    color: labelColor

                    ToolTip {
                        id: baseDirToolTip
                        text: "The directory you see when you first lanch the app"
                        visible: bsTooltipMouseArea.containsMouse
                        delay: 600

                        contentItem: Text {
                            text: baseDirToolTip.text
                            color: "#ddd"
                        }

                        background: Rectangle {
                            color: "#222"
                        }
                    }

                    MouseArea {
                        id: bsTooltipMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }

                Button {
                    id: selectDirBtn
                    text: "Select Directory"

                    Layout.alignment: Qt.AlignRight

                    contentItem: Text {
                        text: selectDirBtn.text
                        color: "#ddd"
                        padding: 10
                    }

                    background: Rectangle {
                        color: "transparent"
                        border {
                            width: 2
                            color: bdBtnMA.containsMouse ? themeColor : "#222"
                        }
                    }

                    MouseArea {
                        id: bdBtnMA
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: baseDirSelect.open();
                    }
                }

                FolderDialog {
                    id: baseDirSelect
                    currentFolder: AppConfiguration.baseDirectory
                    onAccepted: {
                        AppConfiguration.baseDirectory = currentFolder;
                    }
                }

                // row 4
                Label {
                    text: "Run In System Tray"
                    color: labelColor

                    ToolTip {
                        id: sysTrayToolTip
                        text: "When this is active, you should see Mastoid's icon in your system tray.\n" +
                              "When you close the app, it will continue running until you close it from the system tray."
                        visible: stTooltipMouseArea.containsMouse
                        delay: 600

                        contentItem: Text {
                            text: sysTrayToolTip.text
                            color: "#ddd"
                        }

                        background: Rectangle {
                            color: "#222"
                        }
                    }

                    MouseArea {
                        id: stTooltipMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }

                Switch {
                    id: sysTraySwitch
                    checked: AppConfiguration.runInTray

                    Layout.alignment: Qt.AlignRight

                    indicator: Rectangle {
                        width: 60
                        height: 26
                        radius: 13
                        color: sysTraySwitch.checked ? themeColor : "#444";

                        Rectangle {
                            width: 20
                            height: 20
                            radius: 10
                            x: sysTraySwitch.checked ? parent.width - width - 3 : 3
                            y: (parent.height - height) / 2
                            color: "#999"
                        }
                    }

                    onClicked: {
                        AppConfiguration.runInTray = checked;
                    }
                }
            }
            RowLayout {
                id: finalizeBtnsLayout

                anchors {
                    right: controlsLayout.right
                    bottom: parent.bottom
                    bottomMargin: 40
                }

                Button {
                    id: cancelBtn

                    text: "Cancel"

                    contentItem: Text {
                        text: cancelBtn.text
                        color: "#ddd"
                        padding: 10
                    }

                    background: Rectangle {
                        color: "transparent"
                        border {
                            width: 2
                            color: cancelBtnMA.containsMouse ? themeColor : "#222"
                        }
                    }

                    MouseArea {
                        id: cancelBtnMA
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: root.destroy();
                    }
                }

                Button {
                    id: saveBtn

                    text: "Save"

                    contentItem: Text {
                        text: saveBtn.text
                        color: "#ddd"
                        padding: 10
                    }

                    background: Rectangle {
                        color: "transparent"
                        border {
                            width: 2
                            color: saveBtnMA.containsMouse ? themeColor : "#222"
                        }
                    }

                    MouseArea {
                        id: saveBtnMA
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            AppConfiguration.save();
                            root.destroy();
                        }
                    }
                }
            }
        }
    }
}
