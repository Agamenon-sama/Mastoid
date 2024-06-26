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
    }

    Connections {
        target: folderView
        function onFolderChanged(name) {
            coverImage.source = FileSystemHelper.findCoverUrl(name);
        }
    }
}
