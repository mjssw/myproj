import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: friendview;
    visible: true
    anchors.fill: parent
    property int chatHeight: height

    GroupView {
        id: groupview
    }

    ChatView {
        id: chatview
        visible: false
        chatHeight: parent.chatHeight
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
        console.debug("friendview clickGroup idx=", index, "groupname=", groupname)
        groupview.visible=false
        chatview.visible=true
        parent.clickGroup(index, groupname)
    }

    function backToGroupList()
    {
        console.debug("friendview backToGroupList")
        groupview.visible=true
        chatview.visible=false
    }
}
