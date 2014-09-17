import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: friendview;
    visible: true
    anchors.fill: parent
    color: "white"
    property int headerHeight: parent.headerHeight
    property int tailerHeight: parent.tailerHeight

    GroupView {
        id: groupview
    }

    ChatView {
        // 使用整个屏幕
        id: chatview
        visible: false
        height: parent.height+parent.headerHeight+parent.tailerHeight
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: -parent.headerHeight
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }

    function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    {
        console.debug("friendview addGroup")
        groupview.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    }

    function delGroup(index)
    {
        console.debug("friendview addGroup")
        groupview.delGroup(index)
    }

    function clickGroup(index, groupname)
    {
        console.debug("headerHeight=", headerHeight, "tailerHeight=", tailerHeight)
        console.debug("friendview clickGroup idx=", index, "groupname=", groupname)
        groupview.visible = false
        chatview.visible = true
        chatview.headerText = groupname
        parent.showHeaderAndTailer(false)
    }

    function backToGroupList()
    {
        console.debug("friendview backToGroupList")
        groupview.visible=true
        chatview.visible=false
        parent.showHeaderAndTailer(true)
    }
}
