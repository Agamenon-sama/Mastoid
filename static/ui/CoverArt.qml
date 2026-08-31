import QtQuick 2.0

Rectangle {
    id: root

    Image {
        id: coverImage
        source: "qrc:/icons/Mastoid.png"

        width: parent.width * 0.85
        fillMode: Image.PreserveAspectFit

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        SequentialAnimation {
            id: coverTransition
            NumberAnimation {
                target: coverImage
                property: "opacity"
                easing.type: Easing.Linear
                duration: 350
                to: 0
            }

            PropertyAction {
                target: coverImage
                property: "source"
                value: internal.sourceTransition
            }

            NumberAnimation {
                target: coverImage
                property: "opacity"
                easing.type: Easing.OutCubic
                duration: 550
                to: 1
            }
        }

        onStatusChanged: if (status === Image.Ready) themeCanvas.requestPaint();
        onSourceChanged: Mpris.updateCoverArt(source);
    }

    QtObject {
        id: internal
        property string sourceTransition
    }

    Connections {
        target: folderView
        function onFolderChanged(name) {
            coverTransition.stop(); // if another animation is playing, interrupt it and start a new one
            internal.sourceTransition = FileSystemHelper.findCoverUrl(name);
            coverTransition.start();
        }
    }

    Canvas {
        id: themeCanvas
        width: 64; height: 64 // downscale target
        visible: false

        onPaint: {
            var ctx = getContext("2d")
            ctx.drawImage(coverImage, 0, 0, width, height)

            var imageData = ctx.getImageData(0, 0, width, height)
            var data = imageData.data

            var buckets = {}
            var bucketSize = 24 // quantization step; smaller = more precise but more buckets

            for (var i = 0; i < data.length; i += 4) {
                var r = data[i], g = data[i + 1], b = data[i + 2], a = data[i + 3]

                if (a < 125) continue // skip transparent pixels

                // skip near-white / near-black / low-saturation pixels
                var max = Math.max(r, g, b)
                var min = Math.min(r, g, b)
                var lightness = (max + min) / 2
                var saturation = max === min ? 0 : (max - min) / (255 - Math.abs(2 * lightness - 255))
                if (lightness < 20 || lightness > 235) continue
                if (saturation < 0.15) continue

                var qr = Math.round(r / bucketSize) * bucketSize
                var qg = Math.round(g / bucketSize) * bucketSize
                var qb = Math.round(b / bucketSize) * bucketSize
                var key = qr + "," + qg + "," + qb

                buckets[key] = (buckets[key] || 0) + 1
            }

            var bestKey = null
            var bestCount = 0
            for (var k in buckets) {
                if (buckets[k] > bestCount) {
                    bestCount = buckets[k]
                    bestKey = k
                }
            }

            if (bestKey) {
                var parts = bestKey.split(",")
                var dominant = Qt.rgba(parts[0] / 255, parts[1] / 255, parts[2] / 255, 1)
                Theme.accentColor = dominant
            }
        }
    }
}
