import QtQuick 2.0

Item {
    id: membersmenuview
    visible: false
    anchors.fill: parent

    property string groupname: parent.groupname
    property int headerHeight: parent.headerHeight
    property int menuHeight: parent.menuHeight

    Item {
        id: membermenuheader
        z: 0.1
        height: headerbg.sourceSize.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: -menuHeight-headerHeight

        BorderImage {
            id: headerbg;
            source: "../res/topbar.png";
            border.left: 1; border.right: 1;
            border.top: 1; border.bottom: 2;
            anchors.fill: parent
        }

        Text {
            id: headtext
            text: membersmenuview.groupname
            font.pixelSize: 35;
            color: "white";
            anchors.centerIn: parent;
        }
    }

    function showMembersMenuView(isshow)
    {
        visible = isshow
    }
}
