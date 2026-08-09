import QtQuick
import QtQuick.Controls

Switch {
    id: root

    property color activeColor: "#4c0080"

    indicator: Rectangle {
        width: 60
        height: 26
        radius: 13
        color: root.checked ? root.activeColor : "#444";

        Rectangle {
            width: 20
            height: 20
            radius: 10
            x: root.checked ? parent.width - width - 3 : 3
            y: (parent.height - height) / 2
            color: "#999"
        }
    }
}
