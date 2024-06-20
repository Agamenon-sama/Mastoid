import QtQuick 2.0
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
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 15
                    text: {
                        if (fileIsDir) {
                            return fileName + "/"
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

                // use MouseArea to make the rect clickable
                MouseArea {
                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        if (folderModel.isFolder(index)) {
                            changeFolder(fileUrl, fileBaseName)
                        } else {
                            filePressed(fileUrl)
                        }
                    }

                    hoverEnabled: true
                    onEntered: parent.color = "#80808080"
                    onExited: parent.color = "transparent"
                }
            }
        }

        focus: true
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
                    // This way that I am using to determine the name of the folder
                    // may not be portable because it relies on the fact that the
                    // file separator is the '/' character which is not the case on windows
                    // todo: test it once I decide to care about windows
                    var parent = folderModel.parentFolder;
                    var subStrings = parent.toString().split('/');
                    changeFolder(parent, subStrings[subStrings.length-1]);
                }
            }
        }
    }

    function changeFolder(folderPath, baseName) {
        folderModel.folder = folderPath
        titleTxt.text = baseName
        folderChanged(folderModel.folder)
    }

    function getInitFolder() {
        var x = FileSystemHelper.findMusicDirectory();
        console.log(x);
        return x;
    }
}
