import QtQuick 2.0

Rectangle {
    id: memberinfo
    anchors.fill: parent
    color: "white"

    property int headH: height / 10
    property int bottomH: headH * 1.2
    property int elemH: headH
    property int spliteH: elemH * 0.3

    Loader {
        id: loader
        anchors.fill: parent
        source: ""

        function onclickBackButton()
        {
        }
    }

    Item {
        id: memberinfomain
        anchors.fill: parent

        CommHeader {
            id: header
            height: memberinfo.headH
            headtext: qsTr("成员信息")
            z: 0.1

            CommBackbtn {
                id: backbtn

                function clickBackButton()
                {
                    onclickBackButton()
                }
            }
        }

        Item {
            id: infoitem
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            // TODO
            CommonListView {
                id: lstinfo
                anchors.fill: parent

                Component.onCompleted: {
                    addElement("../res/boy.png", qsTr("帐号管理"), memberinfo.elemH*1.2, 1)
                    addSpliteElement(qsTr(""), memberinfo.spliteH)
                    addElement("../res/t2.png", qsTr("等级"), memberinfo.elemH, 1)
                    addElement("../res/t2.png", qsTr("签名"), memberinfo.elemH, 1)
                    setMoveable(true)
                }

                function clickElement(clickkey)
                {
                }
            }
        }
    }
}
