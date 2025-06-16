import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia

ApplicationWindow {
    id: parentWindow

    width: AppConfiguration.width
    height: AppConfiguration.height
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
        height: parent.height * 0.18
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        MediaPlayer {
            id: player

            audioOutput: AudioOutput {
                id: audio
                muted: playerBlock.muted
                volume: playerBlock.volume
            }

            onPlayingChanged: () => {
                if (player.position === player.duration) { // ended playing
                    switch(playerBlock.endPolicy) {
                    case PlayerControl.EndPolicy.Loop:
                        player.position = 0;
                        play();
                        break;
                    case PlayerControl.EndPolicy.PlayNext:
                        var songPath = folderView.nextSong();
                        if (songPath) {
                            player.source = songPath;
                            play();
                        }
                        break;
                    case PlayerControl.EndPolicy.End:
                    default:
                        player.stop();
                    }
                }
            }

            onErrorOccurred: (error, errorString) => {
                switch (error) {
                case 1:
                    error = "A media resource couldn't be resolved.";
                    break;
                case 2:
                    error = "Unsupported file format or corrupted file. Playback may still be possible, but with issues.";
                    break;
                case 4:
                    error = "You don't have the permission to open this file";
                    break;
                default:
                    error = "Unknown error";
                    break;
                }
                console.error(error, errorString);
            }
        }
    }

    Connections {
        target: SystemTrayMenu
        function onRestoreWindow() {
            showNormal();
        }
    }
}
