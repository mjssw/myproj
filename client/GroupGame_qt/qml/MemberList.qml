import QtQuick 2.0

Rectangle {
    id: memberlst
    anchors.fill: parent
    color: "white"

    property int headH: height / 10
    property int bottomH: headH * 1.2
    property int elemH: headH
    property int spliteH: elemH * 0.3

    signal myItemMessage(string msg)

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
            height: memberlst.headH
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
                    addElement("../res/boy.png", qsTr("test1"), memberlst.elemH, 1)
                    addElement("../res/boy.png", qsTr("test2"), memberlst.elemH, 1)
                    addElement("../res/boy.png", qsTr("test3"), memberlst.elemH, 1)
                    addElement("../res/boy.png", qsTr("test4"), memberlst.elemH, 1)
                    addElement("../res/boy.png", qsTr("test5"), memberlst.elemH, 1)
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
        // can directly call function defined in Loader{}
        // see GroupInfo.qml
        //loadertest1()

        memberinfoloader.source = "MemberInfo.qml"
        memberlstmain.visible = false
    }

    function backtoMemberList()
    {
        memberinfoloader.source = ""
        memberlstmain.visible = true
    }

    function memberlistdebug()
    {
        console.debug("memberlistdebug")

        myItemMessage("myItemMessage in memberlistdebug")
    }
}
