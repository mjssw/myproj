import QtQuick 2.0

Item {
    id: backbtn
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: backimg.sourceSize.width + backtext.width + off + margin

    property int off: 10
    property int margin: 3
    property string backStr: ""

    signal clicked
    onClicked: {
        clickBackButton()
    }

    Image {
        id: backimg
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: off
        source: backmouse.pressed ? "../res/back_1.png" : "../res/back_2.png"
    }

    Text {
        id: backtext
        anchors.left: backimg.right
        anchors.leftMargin: margin
        anchors.verticalCenter: parent.verticalCenter
        text: parent.backStr
        font.pixelSize: backimg.height * 0.8
        font.weight: Font.DemiBold
        color: backmouse.pressed ? "gray" : "white"
    }

    MouseArea {
        id: backmouse
        anchors.fill: parent
        onClicked: {
            backbtn.clicked()
        }
    }
}
