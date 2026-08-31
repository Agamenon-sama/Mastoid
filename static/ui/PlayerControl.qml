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
    property color themeColor: Theme.accentColor
    property color backgroundColor: "#000"
    property int endPolicy: PlayerControl.EndPolicy.PlayNext

    Behavior on themeColor {
        ColorAnimation { duration: 400; easing.type: Easing.OutCubic }
    }

    color: backgroundColor

    function play() {
        // attempt to fix the stutter when playing after a pause in some Qt Multimedia backends
        player.setPosition(player.position > 50 ? player.position - 50 : 0);
        player.play()
        playIcon.source = "qrc:/icons/pause.svg"
    }

    function pause() {
        player.pause()
        playIcon.source = "qrc:/icons/play.svg"
    }

    function seek(v) {
        player.setPosition(v);
        Mpris.notifySeeked(v * 1000);
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

                    onMoved: seek(value * player.duration)

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
                            seek(player.duration);
                        }
                        else {
                            seek(playbackSlider.value * player.duration + seekTime);
                        }
                    }
                }
                Shortcut {
                    sequences: ["j", "left"]
                    onActivated: {
                        // the if statement might be useless from what I test
                        if (playbackSlider.value * player.duration - seekTime < 0) {
                            seek(0);
                        }
                        else {
                            seek(playbackSlider.value * player.duration - seekTime);
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
                                seek((key * 0.1) * player.duration);
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

                text: {
                    const path = decodeURIComponent(player.source.toString());
                    if (path == "file:")
                        return "";

                    return path.slice(path.lastIndexOf("/")+1);
                }
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

        function onSeekRequest(offsetUs) {
            // player.position += offsetUs / 1000; // offsetUs is in microseconds
            seek(player.position + offsetUs / 1000);
        }

        function onOpenUriRequest(uri) {
            player.source = uri;
            play();
        }

        function onSetPositionRequest(positionUs) {
            seek(positionUs / 1000);
        }

        function onVolumeChangeRequest(v) {
            volumeSlider.value = v * 100.0;
        }

        function onLoopStatusChangeRequest(status) {
            switch (status) {
            case "None":
                root.endPolicy = PlayerControl.EndPolicy.End;
                break;
            case "Track":
                root.endPolicy = PlayerControl.EndPolicy.Loop;
                break;
            case "Playlist":
                root.endPolicy = PlayerControl.EndPolicy.PlayNext;
                break;
            }
        }
    }

    Connections {
        target: player

        function onMetaDataChanged() {

            var meta = {};

            for (var key of player.metaData.keys()) {
                meta[player.metaData.metaDataKeyToString(key)] = player.metaData.stringValue(key);
            }

            var artists = [];
            if (meta["Album artist"] !== undefined) artists.push(meta["Album artist"])
            if (meta["Contributing artist"] !== undefined) artists.push(meta["Contributing artist"])

            Mpris.updateMetadata(
                meta["Track number"] || 0,
                meta["Title"] || currentSongName.text,
                artists.length === 0 ? "Unknown Artist" : artists,
                meta["Album title"],
                player.duration * 1000 // microseconds
            );
        }

        function onPositionChanged() {
            Mpris.updatePosition(player.position * 1000) // ms -> µs
        }

        // function onSeeked(newPositionMs) {
        //     // user or app jumped the position (not natural playback progress)
        //     Mpris.notifySeeked(newPositionMs * 1000)
        // }
    }

    Component.onCompleted: {
        player.source = AppConfiguration.getStartupFile();
        if (player.source) {
            play();
        }
    }
}
