import QtQuick 2.0

Rectangle {
    id: colorbtn
    anchors.fill: parent
    color: "#C96A6A"
    radius: 8

    property string btnText: ""
    property int letterspacing: 0
    property alias pressed: clickarea.pressed
    opacity: pressed ? 0.7 : 0.95

    MouseArea{
        id: clickarea
        anchors.fill: parent
        onClicked: {
            clickButton()
        }
    }

    Text {
        anchors.centerIn: parent
        text: btnText
        font.pixelSize: parent.height * 0.4
        font.letterSpacing: parent.letterspacing
        font.weight: Font.DemiBold
        color: "white"
    }
}
