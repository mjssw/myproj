import QtQuick 2.0

Item {
    id: gamemenuview
    visible: false
    anchors.fill: parent

    property string groupname: parent.groupname
    property int headerHeight: parent.headerHeight
    property int menuHeight: parent.menuHeight

    Item {
        id: gamemenuheader
        z: 0.1
        height: parent.headerHeight
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
            text: gamemenuview.groupname
            font.pixelSize: parent.height * 0.5
            color: "white";
            anchors.centerIn: parent;
        }
    }

    Item{
        id: roomlist
        anchors.fill: parent
        property int spliteH: 30
        property int elemH: 50

        CommonListView {
            id: lst
            anchors.fill: parent

            function clickElement(clickkey)
            {
                console.debug("roomlist::clickElement")
            }
        }

        function addRoom(gameicon, gamename, gameid, roomid)
        {
            // TODO
            lst.addElement(gameicon, gamename, elemH, roomid)
        }

        function addSplite(game)
        {
            lst.addSpliteElement(game, spliteH)
        }
    }


    function showGameMenuView(isshow)
    {
        visible = isshow
    }

    function onClickRoom(index, groupname)
    {
        console.debug("onClickRoom index=", index)
    }

    function addGameRoom(gameicon, gamename, gameid, roomid)
    {
        roomlist.addRoom(gameicon, gamename, gameid, roomid)
    }

    function addRoomSplite(game)
    {
        roomlist.addSplite(game)
    }
}
