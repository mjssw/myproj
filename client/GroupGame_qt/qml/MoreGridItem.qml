import QtQuick 2.0

Item {
    id: elemitem
    anchors.fill: parent

    property string hinttext: ""
    property string iconNormal: ""
    property string iconPressed: ""

    Image {
        id: elemimg
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        source: clickmouse.pressed ? parent.iconPressed : parent.iconNormal
    }

    Text {
        anchors.top: elemimg.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: parent.hinttext
        font.pixelSize: 20
        font.weight: Font.DemiBold
        color: "gray"
    }

    MouseArea {
        id: clickmouse
        anchors.fill: parent
        onClicked: {
            clickItem()
        }
    }

    function clickItem()
    {
        parent.onClickGridItem()
    }
}
