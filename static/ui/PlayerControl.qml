import QtQuick 2.0
import QtQuick.Controls
// import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    id: root
    property bool muted: false
    property real volume: volumeSlider.value/100.0

    Slider {
        id: playbackSlider

        width: parent.width - 10
        anchors.margins: 5
        enabled: player.seekable

        to: 1.0
        value: player.position / player.duration

        // onValueChanged: player.setPosition(value * player.duration)
    }

    RowLayout {
        anchors.top: playbackSlider.bottom

        Rectangle {
            id: playBtn

            Image {
                id: playIcon
                source: "qrc:/icons/play.png"
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
                        player.pause()
                        playIcon.source = "qrc:/icons/play.png"
                    } else {
                        player.play()
                        playIcon.source = "qrc:/icons/pause.png"
                    }
                }
            }
        }
        Rectangle {
            id: muteBtn

            Image {
                id: muteIcon
                source: "qrc:/icons/mute.png"
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
                    } else {
                        root.muted = true
                    }
                }
            }
        }

        Slider {
            id: volumeSlider

            to: 100.0
            value: 100.0

            /*handle: Rectangle {
                // anchors.centerIn: parent
                color: "grey"
                implicitWidth: 12
                implicitHeight: 12
                radius: 12
                x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
            }*/
            /*panel: Rectangle {
                color: "grey"
            }*/

            /*style: SliderStyle {
                groove: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 8
                    color: "gray"
                    radius: 8
                }
            }*/
        }
    }

}
