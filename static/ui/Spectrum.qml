import QtQuick 2.0

Item {
    Canvas {
        id: spectrumCanvas

        height: parent.height * 0.8
        anchors {
            verticalCenter: parent.verticalCenter
            // bottom: playerBlock.top
            left: parent.left
            right: parent.right
        }

        // onPaint: {
        //     var ctx = getContext("2d");
        //     ctx.clearRect(0, 0, width, height);
        //     ctx.fillStyle = "#882d9b"
        //     var mags = spectrumAnalyzer.magnitudes;
        //     var barWidth = width / mags.length;
        //     for (var i = 0; i < mags.length; i++) {
        //         var h = mags[i] * height;
        //         ctx.fillRect(i * barWidth, height - h, barWidth - 1, h);
        //     }
        // }

        onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.fillStyle = "#882d9b"

                var left = spectrumAnalyzer.leftMagnitudes;
                var right = spectrumAnalyzer.rightMagnitudes;
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
            function onMagnitudesChanged() { spectrumCanvas.requestPaint() }
        }
    }
}
