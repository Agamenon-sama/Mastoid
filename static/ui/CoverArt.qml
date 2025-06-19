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
}
