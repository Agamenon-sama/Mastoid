import QtQuick 2.0
import QtQuick.Dialogs
import Qt.labs.folderlistmodel

Rectangle {
    color: "transparent"

    signal folderChanged(newFolder: string)
    signal filePressed(fileName: string)

    ListView {
        id: list
        anchors {
            bottom: parent.bottom
            top: topBar.bottom
        }
        width: parent.width
        height: parent.height - topBar.height


        model: FolderListModel {
            id: folderModel
            showDirs: true
            showDirsFirst: true
            // todo: change the folder to something more dynamic
            folder: "file:/home/agamenon/Music"
        }

        delegate: Component {
            id: fileDelegate

            Rectangle {
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    text: {
                        if (fileIsDir) {
                            return fileName + "/"
                        }

                        return fileName
                    }

                    color: "black"
                }
                height: 50
                width: list.width
                color: "transparent"
                radius: 10

                // use MouseArea to make the rect clickable
                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        console.log("Clicked " + fileName)
                        if (folderModel.isFolder(index)) {
                            changeFolder(fileUrl, fileBaseName)
                        } else {
                            filePressed(fileUrl)
                        }
                    }

                    hoverEnabled: true
                    onEntered: parent.color = "#fff"
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
            text: "title"
            color: "black"

            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle {
            id: backButton

            Text {
                 text: "◁"
                 color: "black"

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

                onClicked: {
                    // fixme: the title must be set to the base name after we press on the btn
                    // for now the title is set to "title"
                    changeFolder(folderModel.parentFolder, "title")
                }
            }
        }
    }

    function changeFolder(folderPath, baseName) {
        folderModel.folder = folderPath
        titleTxt.text = baseName
        folderChanged(folderModel.folder)
    }
}
