import QtQuick 2.0

Rectangle {
    id: root

    Image {
        id: backImage
        source: "qrc:/icons/defaultCover.png"

        width: parent.width
        fillMode: Image.PreserveAspectFit

        anchors {
//            verticalCenter: parent.verticalCenter
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
            backImage.source = name + "/cover.png"
        }
    }
}
