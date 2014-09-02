import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: friendview;
    visible: true
    anchors.fill: parent
    property int groupHeadWidth: 95
    property int groupHeadHeight: 95
    property int groupItemHeight: 100

    ListModel {
        id: groupmodel
    }

    Component {
        id: groupdelegate

        Item {
            id: groupitem
            width: grouplist.width
            height: friendview.groupItemHeight

            //clip: true
            signal clicked

            Rectangle {
                anchors.fill: parent
                color: "lightgray"
                visible: mouse.pressed
            }

            // 群头像
            Image {
                id: headid
                source: head
                width: friendview.groupHeadWidth
                height:friendview.groupHeadHeight
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
            }

            // 群名和群ID
            Text {
                id: nameid
                text: name + "(" + groupid + ")"
                anchors.top: headid.top
                anchors.topMargin: 10
                anchors.left: headid.right
                font.pixelSize: 25
            }

            // 最后一条消息
            Text {
                id: msgid
                text: msgtext
                anchors.bottom: headid.bottom
                anchors.bottomMargin: 10
                anchors.left: headid.right
                font.pixelSize: 16
                color: "gray"
            }

            // 群人数
            /*
            Text {
                id: numlimitid
                text: "(" + Number(curcount) + "/" + Number(maxcount) + ")"
                anchors.top: headid.top
                anchors.topMargin: 10
                anchors.right: parent.right
                font.pixelSize: 25
            }
            //*/

            // 最后一条消息时间
            Text {
                id: msgtimeid
                text: msgtime
                anchors.bottom:  headid.bottom
                anchors.bottomMargin: 10
                anchors.right: parent.right
                font.pixelSize: 16
                color: "gray"
            }

            MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: {
                    groupitem.clicked()
                    console.log("list item clicked")
                }

            }
        }

        /*
        ListView.onAdd: SequentialAnimation {
            PropertyAction {
                target: groupitem
                property: "height"
                value: 0
            }
            NumberAnimation {
                target: groupitem
                property: "height"
                to: friendview.groupItemHeight
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }//*/

        /*
        ListView.onRemove: SequentialAnimation {
            PropertyAction {
                target: groupitem
                property: "ListView.delayRemove"
                value: true
            }
            NumberAnimation {
                target: groupitem
                property: "height"
                to: 0
                duration: 250
                easing.type: Easing.InOutQuad
            }

            // Make sure delayRemove is set back to false so that the item can be destroyed
            PropertyAction {
                target: groupitem
                property: "ListView.delayRemove"
                value: false
            }
        }//*/
    }

    ListView {
        id: grouplist
        anchors.fill: parent
        model: groupmodel
        delegate: groupdelegate
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }

    function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    {
        console.log("friendview addGroup")
        groupmodel.append({
            "head": head,
            "name": name,
            "groupid": groupid,
            "curcount": curcount,
            "maxcount": maxcount,
            "msgtext": msg,
            "msgtime": msgtime
        })
    }

    function delGroup(index)
    {
        groupmodel.remove(index)
    }
}
