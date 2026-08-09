import QtQuick 2.0

Item {
    id: root

    property color color: Theme.accentColor
    property bool enabled: AppConfiguration.spectrumEnabled

    function requestPaint() { spectrumCanvas.requestPaint(); }

    Behavior on color {
        ColorAnimation { duration: 400; easing.type: Easing.OutCubic }
    }

    Canvas {
        id: spectrumCanvas

        height: parent.height * 0.8
        anchors {
            verticalCenter: parent.verticalCenter
            // bottom: playerBlock.top
            left: parent.left
            right: parent.right
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            ctx.fillStyle = root.color

            var left = [];
            var right = [];
            if (root.enabled) {
                left = spectrumAnalyzer.leftMagnitudes;
                right = spectrumAnalyzer.rightMagnitudes;
            }
            var barWidth = width / left.length;
            var midY = height / 2;

            for (var i = 0; i < left.length; i++) {
                var lh = left[i] * midY;
                var rh = right[i] * midY;
                ctx.fillRect(i * barWidth, midY - lh, barWidth - 1, lh);       // up
                ctx.fillRect(i * barWidth, midY, barWidth - 1, rh);            // down
            }
        }

        Connections {
            target: spectrumAnalyzer
            function onMagnitudesChanged() { spectrumCanvas.requestPaint(); }
        }
    }
}
