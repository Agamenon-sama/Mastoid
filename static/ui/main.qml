import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia

// import "FolderView.qml"

ApplicationWindow {
    width: 1080
    height: 600
    visible: true
    title: qsTr("Mastoid")

    color: "#1e1e1e"

    FolderView {
        id: folderView

        width: parent.width * 0.6
        anchors {
            top: parent.top
            bottom: playerBlock.top
            right: parent.right
        }
    }

    Rectangle {
        width: parent.width * 0.4
        anchors {
            top: parent.top
            bottom: playerBlock.top
            left: parent.left
        }

        color: "#777"
    }

    AudioControl {
        id: playerBlock
        height: parent.height * 0.15
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        color: "#000"

        MediaPlayer {
            id: player
            // todo: change the source to something more dynamic
            source: "file:/home/agamenon/Music/Metal/Symphony X - Inferno (Unleash The Fire).mp3"
            audioOutput: AudioOutput {
                id: audio
                muted: playerBlock.muted
            }
        }
    }

    /*Rectangle {
        id: playerBlock
        height: parent.height * 0.15
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        color: "#000"

        MediaPlayer {
            id: player
            // todo: change the source to something more dynamic
            source: "file:/home/agamenon/Music/Metal/Symphony X - Inferno (Unleash The Fire).mp3"
            audioOutput: AudioOutput {
                id: audio
            }
        }

        Button {
            id: playBtn
            text: "▶"
            x: 0

            onClicked: player.play()
        }
        Button {
            id: pauseBtn
            text: "‖"
            x: 100

            onClicked: player.pause()
        }
        Button {
            id: stopBtn
            text: "■"
            x: 200

            onClicked: player.stop()
        }

        /*Button {
            id: "muteBtn"
            text: "mute"

            // onClicked:
        }*/

        /*Component.onCompleted: {
            player.source = "file:/home/agamenon/Music/Metal/Symphony X - Inferno (Unleash The Fire).mp3"
            player.play();
            console.log("playerBlock loaded")
        }*/

        /*FileDialog {
            id: fileDialog
            onAccepted: {
                player.stop();
                player.source = fileDialog.currentFile;
                player.play();
            }
        }

        Button {
            text: "file"
            onClicked: fileDialog.open()
        }*/

    //}
}
