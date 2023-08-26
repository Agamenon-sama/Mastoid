import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia

ApplicationWindow {
    width: 1080
    height: 600
    visible: true
    title: qsTr("Mastoid")

    color: "#1e1e1e"

    BackgroundTexture {
        id: backTexture

        width: parent.width;
        height: parent.height;
    }

    FolderView {
        id: folderView

        width: parent.width * 0.6
        anchors {
            top: parent.top
            bottom: playerBlock.top
            right: parent.right
        }
    }

    CoverArt {
        id: coverArt
        width: parent.width * 0.4
        anchors {
            top: parent.top
            bottom: playerBlock.top
            left: parent.left
        }

        color: "transparent"
    }

    PlayerControl {
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

            audioOutput: AudioOutput {
                id: audio
                muted: playerBlock.muted
                volume: playerBlock.volume
            }
        }
    }
}
