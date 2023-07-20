import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    id: root
    property bool muted: false

    RowLayout {
        Rectangle {
            id: playBtn

            Text {
                text: "▶"
                color: "grey"
            }
            width: 50
            height: 50
            radius: 50
            color: "transparent"
            border.width: 2
            border.color: "grey"
            // x: 0

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (player.playbackState == MediaPlayer.PlayingState) {
                        player.pause()
                    } else {
                        player.play()
                    }
                }
            }
        }
        Rectangle {
            id: muteBtn

            Text {
                text: "mute"
                color: "grey"
            }
            width: 45
            height: 45
            radius: 45
            color: "transparent"
            border.width: 2
            border.color: "grey"

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (root.muted) {
                        root.muted = false
                    } else {
                        root.muted = true
                    }
                }
            }
        }
        Button {
            id: stopBtn
            text: "■"
            // x: 200

            onClicked: player.stop()
        }
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

}
