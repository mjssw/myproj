import QtQuick 2.0

Item {
    id: backbtn
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: backimg.sourceSize.width * 2

    property int off: 10
    property int margin: 3

    signal clicked
    onClicked: {
        clickBackButton()
    }

    Image {
        id: backimg
        anchors.centerIn: parent
        height: parent.height * 0.5
        width: sourceSize.width * height / sourceSize.height
        source: backmouse.pressed ? "../res/back_1.png" : "../res/back_2.png"
    }

    MouseArea {
        id: backmouse
        anchors.fill: parent
        onClicked: {
            backbtn.clicked()
        }
    }
}
