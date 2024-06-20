import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    id: root
    property bool muted: false
    property real volume: volumeSlider.value/100.0

    function play() {
        player.play()
        playIcon.source = "qrc:/icons/pause.svg"
    }

    function pause() {
        player.pause()
        playIcon.source = "qrc:/icons/play.svg"
    }

    Slider {
        id: playbackSlider

        width: parent.width - 10
        anchors {
            top: root.top
            horizontalCenter: parent.horizontalCenter
        }
        enabled: player.seekable

        to: 1.0
        value: player.position / player.duration

        onMoved: player.setPosition(value * player.duration)

        background: Rectangle {
            x: playbackSlider.leftPadding
            y: playbackSlider.topPadding + playbackSlider.availableHeight / 2 - height / 2
            implicitHeight: 4
            implicitWidth: 100
            height: implicitHeight
            width: playbackSlider.availableWidth
            radius: 2
            color: "#505050"

            Rectangle {
                width: playbackSlider.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: "grey"
            }
        }

        handle: Rectangle {
            x: playbackSlider.leftPadding + playbackSlider.visualPosition * (playbackSlider.availableWidth - width)
            y: playbackSlider.topPadding + playbackSlider.availableHeight / 2 - height / 2
            implicitHeight: 14
            implicitWidth: 14
            radius: 8
            color: "grey"
        }
    }

    Text {
        id: currentTime

        anchors {
            top: playbackSlider.bottom
            left: playbackSlider.left
        }

        text: {
            var m  = Math.floor(player.position / 60000)
            var ms = Math.floor(player.position / 1000 - m * 60).toString()
            return `${m}:${ms.padStart(2, 0)}`
        }
        color: "grey"
    }
    Text {
        id: durationTime

        anchors {
            top: playbackSlider.bottom
            right: playbackSlider.right
        }

        text: {
            var m  = Math.floor(player.duration / 60000)
            var ms = Math.floor(player.duration / 1000 - m * 60).toString()
            return `${m}:${ms.padStart(2, 0)}`
        }
        color: "grey"
    }

    Rectangle {
        id: playBtn

        anchors {
            top: durationTime.bottom
            horizontalCenter: parent.horizontalCenter
        }

        Image {
            id: playIcon
            source: "qrc:/icons/play.svg"
            height: parent.height * 0.55
            fillMode: Image.PreserveAspectFit

            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
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
                    pause()
                } else {
                    play()
                }
            }
        }
    }
    RowLayout {
        id: volumeRow
        anchors {
            top: durationTime.bottom
            right: durationTime.right
        }

        Rectangle {
            id: muteBtn

            Image {
                id: muteIcon
                source: "qrc:/icons/speaker.svg"
                height: parent.height * 0.55
                fillMode: Image.PreserveAspectFit

                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }
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
                        muteIcon.source = "qrc:/icons/speaker.svg"
                    } else {
                        root.muted = true
                        muteIcon.source = "qrc:/icons/mute.svg"
                    }
                }
            }
        }

        Slider {
            id: volumeSlider

            to: 100.0
            value: 100.0


            background: Rectangle {
                x: volumeSlider.leftPadding
                y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                implicitHeight: 4
                implicitWidth: 100
                height: implicitHeight
                width: volumeSlider.availableWidth
                radius: 2
                color: "#505050"

                Rectangle {
                    width: volumeSlider.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "grey"
                }
            }

            handle: Rectangle {
                x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                implicitHeight: 14
                implicitWidth: 14
                radius: 8
                color: "grey"
            }
        }
    }


    Connections {
        target: folderView
        function onFilePressed(name) {
            player.source = name;
            play();
        }
    }

    Component.onCompleted: {
        player.source = AppConfiguration.getStartupFile();
        play();
    }
}
