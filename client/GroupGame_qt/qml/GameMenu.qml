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
                font.pixelSize: parent.height * 0.3
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
