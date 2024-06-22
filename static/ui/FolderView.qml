import QtQuick 2.9
import QtQuick.Dialogs
import Qt.labs.folderlistmodel

Rectangle {
    color: "transparent"

    signal folderChanged(newFolder: string)
    signal filePressed(fileName: string)

    ListView {
        id: fileList

        anchors {
            bottom: parent.bottom
            top: topBar.bottom
            left: parent.left
        }
        width: parent.width * 0.97
        height: parent.height - topBar.height
        clip: true

        model: FolderListModel {
            id: folderModel
            showDirs: true
            showDirsFirst: true
            nameFilters: ["*.mp3", "*.ogg", "*.flac", "*.wav"]
            folder: AppConfiguration.baseDirectory
        }

        delegate: Component {
            id: fileDelegate

            Rectangle {
                Rectangle {
                    height: parent.height * 0.8
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.height * 0.1
                    color: "transparent"

                    Image {
                        anchors.centerIn: parent
                        height: parent.height * 0.7
                        fillMode: Image.PreserveAspectFit

                        source: {
                            if (fileIsDir) {
                                return "qrc:/icons/folder.svg"
                            }

                            return "qrc:/icons/music_note.svg"
                        }
                    }
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5 + parent.height
                    text: {
                        if (fileIsDir) {
                            return fileName
                        }

                        return fileName
                    }

                    font {
                        weight: 600
                    }

                    color: "#ddd"
                }
                height: 50
                width: fileList.width
                color: "transparent"
                radius: 10

                MouseArea {
                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        if (folderModel.isFolder(index)) {
                            changeFolder(fileUrl, fileBaseName);
                            fileList.currentIndex = -1; // reset highlight to none
                        } else {
                            filePressed(fileUrl);
                        }
                    }

                    hoverEnabled: true
                    onEntered: {
                        // the if statement is to help prevent the highlight
                        // and the hover from overlapping
                        if (index !== fileList.currentIndex) {
                            parent.color = "#80808080";
                        }
                    }
                    onExited: {
                        parent.color = "transparent";
                    }
                }
                Keys.onPressed: (event) => {
                    if (event.isAutoRepeat) {
                        return;
                    }

                    switch (event.key) {
                    case Qt.Key_Return:
                        if (folderModel.isFolder(index)) {
                            changeFolder(fileUrl, fileBaseName);
                            // don't reset currentIndex
                        } else {
                            filePressed(fileUrl);
                        }
                        event.accepted = true;
                        break;

                    case Qt.Key_PageUp:
                        fileList.currentIndex = 0;
                        event.accepted = true;
                        break;

                    case Qt.Key_PageDown:
                        fileList.currentIndex = fileList.count - 1
                        event.accepted = true;
                        break;
                    }
                }
            }
        }

        highlight: Rectangle {
            id: fileDelegateHighlight

            color: "#80808080"
            radius: 10
        }

        focus: true
        Shortcut {
            autoRepeat: false
            sequence: "f"
            onActivated: {
                // help reset focus on the ListView if the user
                // pressed for example on a slider
                fileList.focus = true;
            }
        }

        Component.onCompleted: {
            // this is to (visually) disable the highlight of the ListView by default
            fileList.currentIndex = -1;
        }
    }

    Rectangle {
        id: topBar

        anchors {
            top: parent.top
        }
        width: parent.width
        height: 50
        color: "transparent"

        Text {
            id: titleTxt
            text: "Music"
            font {
                weight: 600
            }

            color: "#ddd"

            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle {
            id: backButton

            Image {
                id: backIcon
                source: "qrc:/icons/back.svg"
                height: parent.height * 0.5
                fillMode: Image.PreserveAspectFit

                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }
            }

            anchors {
                top: parent.top
                left: parent.left
            }

            height: parent.height
            width: parent.height
            color: "transparent"

            MouseArea {
                anchors.fill: parent

                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    // This way of determining the name of the folder assumes that the file
                    // separator is the '/' character which is a reasonable assumption given:
                    // https://doc.qt.io/qt-6/qml-qt-labs-folderlistmodel-folderlistmodel.html#path-separators
                    // todo: test it once I decide to care about windows
                    var parent = folderModel.parentFolder;
                    var subStrings = parent.toString().split('/');
                    changeFolder(parent, subStrings[subStrings.length-1]);
                    fileList.currentIndex = -1; // reset highlight to none
                }
            }
            Shortcut {
                autoRepeat: false
                sequences: [StandardKey.Back, "Ctrl+left"]
                onActivated: {
                    var parent = folderModel.parentFolder;
                    var subStrings = parent.toString().split('/');
                    changeFolder(parent, subStrings[subStrings.length-1]);
                    // I explicitly kept the currentIndex where it as I think that's a better UX
                }
            }
        }
    }

    function changeFolder(folderPath, baseName) {
        folderModel.folder = folderPath;
        titleTxt.text = baseName;
        folderChanged(folderModel.folder);
    }
}
