import QtQuick 2.0

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

            width: backImage.width
            height: backImage.height

            vertexShader: "qrc:/shaders/background.vert.qsb"
            fragmentShader: "qrc:/shaders/background.frag.qsb"
        }
    }

    Connections {
        target: folderView
        function onFolderChanged(name) {
            backImage.source = FileSystemHelper.findCoverUrl(name);
        }
    }
}
