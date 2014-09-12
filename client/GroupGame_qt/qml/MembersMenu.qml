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
        height: headerHeight
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: -menuHeight-headerHeight

        Rectangle {
            id: headerbg;
            anchors.fill: parent
            color: "steelblue"
        }

        Text {
            id: headtext
            text: membersmenuview.groupname
            font.pixelSize: parent.height * 0.5
            color: "white";
            anchors.centerIn: parent;
        }
    }

    function showMembersMenuView(isshow)
    {
        visible = isshow
    }
}
