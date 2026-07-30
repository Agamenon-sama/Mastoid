import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia

import Mastoid 1.0

ApplicationWindow {
    id: parentWindow

    width: AppConfiguration.width
    height: AppConfiguration.height
    visible: true
    title: qsTr("Mastoid")

    color: "#1e1e1e"

    QtObject {
        id: internal
        property variant settingsMenu: null
    }

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

        SpectrumAnalyzer {
            id: spectrumAnalyzer
        }

        MediaPlayer {
            id: player

            audioOutput: AudioOutput {
                id: audio
                muted: playerBlock.muted
                volume: playerBlock.volume
            }
            audioBufferOutput: spectrumAnalyzer.bufferOutput

            onMediaStatusChanged: () => {
                if (mediaStatus === MediaPlayer.EndOfMedia) { // ended playing
                    switch(playerBlock.endPolicy) {
                    case PlayerControl.EndPolicy.Loop:
                        player.stop();
                        player.position = 0;
                        play();
                        break;
                    case PlayerControl.EndPolicy.PlayNext:
                        var songPath = folderView.nextSong();
                        if (songPath) {
                            player.stop();
                            player.position = 0;
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

    Canvas {
        id: spectrumCanvas

        height: parent.height * 0.1
        anchors {
            bottom: playerBlock.top
            left: parent.left
            right: parent.right
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            ctx.fillStyle = "#882d9b"
            var mags = spectrumAnalyzer.magnitudes;
            var barWidth = width / mags.length;
            for (var i = 0; i < mags.length; i++) {
                var h = mags[i] * height;
                ctx.fillRect(i * barWidth, height - h, barWidth - 1, h);
            }
        }

        Connections {
            target: spectrumAnalyzer
            function onMagnitudesChanged() { spectrumCanvas.requestPaint() }
        }
    }

    function openSettingsMenu() {
        var settingsComp = Qt.createComponent("qrc:/ui/SettingsMenu.qml");
        if (settingsComp.status === Component.Ready) {
            internal.settingsMenu = settingsComp.createObject(parentWindow);
        }
        else if (settingsComp.status === Component.Error) {
            console.error("Error creating the settings menu", settingsComp.errorString());
        }
    }

    Connections {
        target: SystemTrayMenu
        function onRestoreWindow() {
            showNormal();
        }
    }
}
