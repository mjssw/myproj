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
            text: gamemenuview.groupname
            font.pixelSize: 35;
            color: "white";
            anchors.centerIn: parent;
        }
    }

    ListModel {
        id: roommodel

        ListElement {
            gameid: "123"
            gameicon: ""
            roomicon: ""
            roomid: "123"
        }
    }

    Component {
        id: roomdelegate

        Item {
            id: roomitem
            width: gamemenuview.width
            height: 100

            //clip: true
            signal clicked
            onClicked: {
                onClickRoom(index)
            }

            Rectangle {
                anchors.fill: parent
                color: "lightgray"
                visible: mouse.pressed
            }

            // TODO
            Text {
                id: nameid
                text: gameid + "(" + roomid + ")"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 25
            }

            MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: {
                    roomitem.clicked()
                    console.log("room item clicked")
                }

            }
        }
    }

    ListView {
        id: roomlist
        anchors.fill: parent
        model: roommodel
        delegate: roomdelegate
    }

    function showGameMenuView(isshow)
    {
        visible = isshow
    }

    function onClickRoom(index, groupname)
    {
        console.debug("onClickRoom index=", index)
    }
}
