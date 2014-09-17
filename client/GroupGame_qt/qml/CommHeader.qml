import QtQuick 2.0

Rectangle {
    id: header
    width: parent.width
    height: 88
    anchors.top: parent.top
    anchors.left: parent.left

    property string headtext: ""

    Rectangle {
        id: headerbg
        anchors.fill: parent
        color: "steelblue"
    }

    Text {
        text: headtext
        font.pixelSize: parent.height * 0.4
        color: "white"
        anchors.centerIn: parent
    }
}
