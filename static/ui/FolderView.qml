import QtQuick 2.0
import QtQuick.Dialogs
import Qt.labs.folderlistmodel

Rectangle {
    color: "#eee"

    ListView {
        id: list
        anchors.fill: parent
        // anchors.margins: 5

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
                        // list.currentIndex = index
                        if (folderModel.isFolder(index)) {
                            console.log("fileUrl = " + fileUrl + "; filePath = " + filePath)
                            folderModel.folder = fileUrl
                            // list.currentIndex = 0
                        } else {
                            popup.text = fileName
                            popup.open()
                            player.source = fileUrl
                            player.play()
                        }
                    }

                    hoverEnabled: true
                    onEntered: parent.color = "#fff"
                    onExited: parent.color = "transparent"
                }
            }
        }

        /*highlight: Rectangle {
            /*Text {
                text: {
                    if (folderModel.get(list.currentIndex, "fileIsDir")) {
                        return folderModel.get(list.currentIndex, "fileName") + "/"
                    }

                    return folderModel.get(list.currentIndex, "fileName")
                }
                color: "#000"
            }
            height: 50
            width: list.width
            color: "#555"
            radius: 5
            // y: list.currentItem.y
            // Behavior on y { SpringAnimation { spring: 2; damping: 0.2 } }
        }*/

        focus: true
    }
    MessageDialog {
        id: popup
        title: "you clicked on a file"
    }
}
