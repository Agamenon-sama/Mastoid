import QtQuick 2.9

/*
  The only purpose of this component is to allow me to use a Shortcut
  as a delegate for a Repeater in PlayerControl.qml in order to seek by
  percentage through the media player using the number keys.

  I could have have implemented the same functionality in PlayerControl.qml
  without all this but the only way I see to manually type 10 Shortcut component
  in PlayerControl.qml but I would have felt really bad about it.
*/
Item {
    property int value

    signal pressed(key: int)

    Shortcut {
        sequence: "" + value // cast int to string
        onActivated: {
            pressed(value)
        }
    }
}
