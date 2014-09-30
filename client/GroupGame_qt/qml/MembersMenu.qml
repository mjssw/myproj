import QtQuick 2.0

Item {
    id: membersmenuview
    visible: false
    anchors.fill: parent

    property string groupname: parent.groupname
    property int headerHeight: parent.headerHeight
    property int menuHeight: parent.menuHeight

    Rectangle {
        anchors.fill: parent
        color: "green"
    }

    function showMembersMenuView(isshow)
    {
        visible = isshow
    }
}
