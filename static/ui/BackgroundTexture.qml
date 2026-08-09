import QtQuick 2.0
import QtQuick.Effects

Rectangle {
    id: root

    Image {
        id: backImage
        source: "qrc:/icons/Mastoid.png"

        // I'm ashamed to say I got lost a bit while solving this calculation of the width
        // and height which means I'm not sure anymore how this fully works but IT DOES WORK.
        // If there is an error, please file a bug report. (⌒_⌒;)
        width: {
            var windowRatio = parentWindow.width / parentWindow.height;
            var sourceRatio = sourceSize.width / sourceSize.height;


            if (windowRatio >= sourceRatio) {
                return parent.width;
            }
            else {
                return parent.height * sourceSize.width / sourceSize.height;
            }
        }

        height: {
            var windowRatio = parentWindow.width / parentWindow.height;
            var sourceRatio = sourceSize.width / sourceSize.height;

            if (windowRatio >= sourceRatio) {
                return parent.width * sourceSize.height / sourceSize.width;
            }
            else {
                return parent.height;
            }
        }

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top

        }

        ShaderEffect {
            property variant source: backImage
            property real u_bass: spectrumAnalyzer.bass
            property real u_treble: spectrumAnalyzer.treble
            property real u_beat: spectrumAnalyzer.beat
            property real u_time: player.position
            property bool u_audioEffect: AppConfiguration.shaderEnabled

            width: backImage.width
            height: backImage.height

            vertexShader: "qrc:/shaders/background.vert.qsb"
            fragmentShader: "qrc:/shaders/background.frag.qsb"
        }
    }
    MultiEffect {
        source: backImage
        anchors.fill: backImage
        autoPaddingEnabled: false

        blurEnabled: true
        blur: 1.0
        blurMax: 42
        blurMultiplier: 2
    }

    Connections {
        target: folderView
        function onFolderChanged(name) {
            backImage.source = FileSystemHelper.findCoverUrl(name);
        }
    }
}
