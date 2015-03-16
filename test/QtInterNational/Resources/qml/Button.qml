import QtQuick 1.0

Rectangle {
    id: btn;

    property string label;
    signal clicked()

    width: 120
    height: 30
    color: mouse.pressed ? "lightgray" : "white"

    radius: 4
    border.width: 1
    border.color: "gray"

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: btn.clicked();
    }

    Text {
        anchors.centerIn: parent
        text: btn.label
    }
}
