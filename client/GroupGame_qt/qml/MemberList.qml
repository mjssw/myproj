import QtQuick 2.0

Rectangle {
    id: memberlst
    anchors.fill: parent
    color: "white"

    property int headH: 88
    property int bottomH: 100

    Loader {
        id: memberinfoloader
        anchors.fill: parent
        source: ""

        function onclickBackButton()
        {
            backtoMemberList()
        }
    }

    Item {
        id: memberlstmain
        anchors.fill: parent

        CommHeader {
            id: header
            height: parent.height / 10
            headtext: qsTr("成员列表")
            z: 0.1

            CommBackbtn {
                id: backbtn

                function clickBackButton()
                {
                    onclickBackButton()
                }
            }

            Component.onCompleted: {
                console.debug("complete1")
            }
        }

        Item {
            id: lst
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            // TODO
            CommonListView {
                id: lstinfo
                anchors.fill: parent

                Component.onCompleted: {
                    addElement("../res/boy.png", qsTr("test1"), 80, 1)
                    addElement("../res/boy.png", qsTr("test2"), 80, 1)
                    addElement("../res/boy.png", qsTr("test3"), 80, 1)
                    addElement("../res/boy.png", qsTr("test4"), 80, 1)
                    addElement("../res/boy.png", qsTr("test5"), 80, 1)
                    setMoveable(true)
                }

                function clickElement(clickkey)
                {
                    showMemberInfo()
                }
            }
        }
    }

    function showMemberInfo()
    {
        memberinfoloader.source = "MemberInfo.qml"
        memberlstmain.visible = false
    }

    function backtoMemberList()
    {
        memberinfoloader.source = ""
        memberlstmain.visible = true
    }
}
