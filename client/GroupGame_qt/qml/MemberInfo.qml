import QtQuick 2.0

Rectangle {
    id: memberinfo
    anchors.fill: parent
    color: "white"

    property int headH: 88
    property int bottomH: 100

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
            headtext: qsTr("成员信息")
            z: 0.1

            CommBackbtn {
                id: backbtn
                backStr: qsTr("返回")

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
                    addElement("../res/boy.png", qsTr("帐号管理"), 100, 1)
                    addSpliteElement(qsTr(""), 40)
                    addElement("../res/t2.png", qsTr("等级"), 80, 1)
                    addElement("../res/t2.png", qsTr("签名"), 80, 1)
                    setMoveable(true)
                }

                function clickElement(clickkey)
                {
                }
            }
        }
    }
}
