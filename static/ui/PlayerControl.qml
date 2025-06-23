import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    id: root

    enum EndPolicy {
        Loop,
        End,
        PlayNext
    }

    property bool muted: false
    property real volume: volumeSlider.value/100.0
    property int seekTime: 5000
    property real volumeModifier: 5.0
    property color themeColor: "#4c0080"
    property color backgroundColor: "#000"
    property int endPolicy: PlayerControl.EndPolicy.PlayNext

    color: backgroundColor

    function play() {
        player.play()
        playIcon.source = "qrc:/icons/pause.svg"
    }

    function pause() {
        player.pause()
        playIcon.source = "qrc:/icons/play.svg"
    }

    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            RowLayout {
                width: parent.width - 50
                anchors {
                    top: parent.top
                    topMargin: 5
                    horizontalCenter: parent.horizontalCenter
                }
                Text {
                    id: currentTime

                    text: {
                        var m  = Math.floor(player.position / 60000)
                        var ms = Math.floor(player.position / 1000 - m * 60).toString()
                        return `${m}:${ms.padStart(2, 0)}`
                    }
                    color: "#ddd"
                }
                Slider {
                    id: playbackSlider

                    Layout.fillWidth: true

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
                        color: "#888"

                        Rectangle {
                            width: playbackSlider.visualPosition * parent.width
                            height: parent.height
                            radius: 2
                            color: themeColor
                        }
                    }

                    handle: Rectangle {
                        x: playbackSlider.leftPadding + playbackSlider.visualPosition * (playbackSlider.availableWidth - width)
                        y: playbackSlider.topPadding + playbackSlider.availableHeight / 2 - height / 2
                        implicitHeight: 14
                        implicitWidth: 14
                        radius: 8
                        color: themeColor
                    }
                }


                Text {
                    id: durationTime

                    text: {
                        var m  = Math.floor(player.duration / 60000)
                        var ms = Math.floor(player.duration / 1000 - m * 60).toString()
                        return `${m}:${ms.padStart(2, 0)}`
                    }
                    color: "#ddd"
                }

            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: settingsBtn

                width: 45
                height: 45
                radius: 45
                color: "transparent"
                border.width: 2
                border.color: "#888"

                anchors {
                    left: parent.left
                    leftMargin: 25
                    verticalCenter: parent.verticalCenter
                }

                Text {
                    text: qsTr("S")
                    color: "white"

                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: parentWindow.openSettingsMenu();
                }
            }

            Rectangle {
                id: playBtn

                anchors {
                     horizontalCenter: parent.horizontalCenter
                     verticalCenter: parent.verticalCenter
                }

                Image {
                    id: playIcon
                    source: "qrc:/icons/play.svg"
                    height: parent.height * 0.55
                    fillMode: Image.PreserveAspectFit
                    mipmap: true

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
                border.color: "#888"

                MouseArea {
                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        if (player.playbackState == MediaPlayer.PlayingState) {
                            pause()
                        } else {
                            play()
                        }
                    }
                }

                Shortcut {
                    autoRepeat: false
                    sequences: ["space", "k"]
                    onActivated: {
                        if (player.playbackState == MediaPlayer.PlayingState) {
                            pause()
                        } else {
                            play()
                        }
                    }
                }
                Shortcut {
                    sequences: ["l", "right"]
                    onActivated: {
                        // todo: maybe test if I need to check if the player is on or not
                        if (playbackSlider.value * player.duration + seekTime > player.duration) {
                            player.setPosition(player.duration);
                        }
                        else {
                            player.setPosition(playbackSlider.value * player.duration + seekTime)
                        }
                    }
                }
                Shortcut {
                    sequences: ["j", "left"]
                    onActivated: {
                        // the if statement might be useless from what I test
                        if (playbackSlider.value * player.duration - seekTime < 0) {
                            player.setPosition(player.duration);
                        }
                        else {
                            player.setPosition(playbackSlider.value * player.duration - seekTime)
                        }
                    }
                }
                // This Repeater is used to create 10 new shorcuts for each of the number
                // keys that seek through the song at different percentages
                // for example: Key_3 -> 30% of the song
                Repeater {
                    model: 10
                    ShortcutComponent {
                        required property int index // index should be from 0 to model - 1
                        value: index
                        Connections {
                            function onPressed(key) {
                                player.setPosition((key * 0.1) * player.duration);
                            }
                        }
                    }
                }
            }
            RowLayout {
                id: volumeRow
                anchors {
                    right: parent.right
                    rightMargin: 25
                    verticalCenter: parent.verticalCenter
                }

                Rectangle {
                    id: muteBtn

                    Image {
                        id: muteIcon
                        source: "qrc:/icons/speaker.svg"
                        height: parent.height * 0.55
                        fillMode: Image.PreserveAspectFit
                        mipmap: true

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
                    border.color: "#888"

                    MouseArea {
                        anchors.fill: parent

                        cursorShape: Qt.PointingHandCursor

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
                    Shortcut {
                        autoRepeat: false
                        sequence: "m"
                        onActivated: {
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
                        color: "#888"

                        Rectangle {
                            width: volumeSlider.visualPosition * parent.width
                            height: parent.height
                            radius: 2
                            color: themeColor
                        }
                    }

                    handle: Rectangle {
                        x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                        y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                        implicitHeight: 14
                        implicitWidth: 14
                        radius: 8
                        color: themeColor
                    }
                    Shortcut {
                        sequence: "+"
                        onActivated: {
                            volumeSlider.value += volumeModifier;
                        }
                    }
                    Shortcut {
                        sequence: "-"
                        onActivated: {
                            volumeSlider.value -= volumeModifier;
                        }
                    }
                }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: currentSongName

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                color: "#ddd"

                text: ""
            }
        }
    }


    Connections {
        target: folderView
        function onFilePressed(name) {
            player.source = name;
            play();
            currentSongName.text = name.slice(name.lastIndexOf("/")+1);
        }
    }

    Connections {
        target: SystemTrayMenu
        function onPlay() {
            play();
        }
        function onPause() {
            pause();
        }
        function onSoundToggle() {
            if (root.muted) {
                root.muted = false
                muteIcon.source = "qrc:/icons/speaker.svg"
            } else {
                root.muted = true
                muteIcon.source = "qrc:/icons/mute.svg"
            }
        }
    }

    Connections {
        target: Mpris
        function onPlayRequest() {
            play();
        }

        function onPauseRequest() {
            pause();
        }

        function onPlayPauseRequest() {
            if (player.playing) {
                pause();
            } else {
                play();
            }
        }

        function onStopRequest() {
            player.stop();
        }

        function onNextRequest() {
            var songPath = folderView.nextSong();
            if (songPath) {
                player.source = songPath;
                play();
            }
        }

        function onPreviousRequest() {
            var songPath = folderView.previousSong();
            if (songPath) {
                player.source = songPath;
                play();
            }
        }

        function onSeekRequest(x) {
            player.position += x/1000; // x is in microseconds
        }

        function onOpenUriRequest(uri) {
            player.source = uri;
            play();
        }
    }

    Component.onCompleted: {
        player.source = AppConfiguration.getStartupFile();
        if (player.source) {
            play();
        }
    }
}
