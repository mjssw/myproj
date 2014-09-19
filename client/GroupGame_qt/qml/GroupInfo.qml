import QtQuick 2.0

Rectangle {
    id: groupinfo
    anchors.fill: parent
    color: "white"

    property int headH: 88
    property int bottomH: 100
    property bool isInGroup: false

    Loader {
        id: memberlistloader
        anchors.fill: parent
        source: ""

        function onclickBackButton()
        {
            backtoGroupInfo()
        }
    }

    Item {
        id: groupinfomain
        anchors.fill: parent

        CommHeader {
            id: header
            headtext: qsTr("详细信息")
            z: 0.1

            CommBackbtn {
                id: backbtn
                backStr: qsTr("返回")

                function clickBackButton()
                {
                    onclickBackButton()
                }
            }

            Component.onCompleted: {
                console.debug("complete1")
            }
        }

        Rectangle {
            id: bottomer
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: groupinfo.bottomH
            border.width: 1
            border.color: "lightgray"
            color: "#F5FAF5"
            z: 0.1

            Component.onCompleted: {
                console.debug("complete2")
            }

            Item {
                width: parent.width * 0.6
                height: parent.height* 0.6
                anchors.centerIn: parent
                CommColorButton {
                    id: btn
                    btnText: groupinfo.isInGroup ? qsTr("开始聊天") : qsTr("申请加入")
                    letterspacing: 6

                    function clickButton()
                    {
                        onclickFuncButton()
                    }
                }
            }
        }

        Item {
            id: maininfo
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bottomer.top

            // TODO
            CommonListView {
                id: lstinfo
                anchors.fill: parent

                property int keyMemberList: 501
                Component.onCompleted: {
                    console.debug("-- complete --")
                    addElement("../res/groupdefault.png", qsTr("群名字"), 100, 1)
                    addSpliteElement(qsTr(""), 40)
                    addElement("../res/t2.png", qsTr("成员列表"), 80, keyMemberList)
                    addSpliteElement(qsTr(""), 40)
                    addElement("../res/t2.png", qsTr("等级"), 80, 2)
                    addElement("../res/t2.png", qsTr("人数上限"), 80, 2)
                    addElement("../res/t2.png", qsTr("介绍"), 80, 2)
                    setMoveable(true)
                }

                function clickElement(clickkey)
                {
                    console.debug("click main info")
                    lstinfo.debugInfo()
                    switch(clickkey)
                    {
                    case keyMemberList:
                        clickMemberList()
                        break;
                    }
                }
            }

            Component.onCompleted: {
                console.debug("complete3")
            }
        }

        Item {
            id: hintitem
            width: parent.width * 0.9
            height: parent.height / 14
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            z: 0.1

            CommHint {
                id: hint
            }

            function showHint(text)
            {
                hintitem.visible = true
                hint.showHint(text)
            }
        }
    }

    function onclickBackButton()
    {
        parent.clickBackButton()
    }

    function onclickFuncButton()
    {
        if( isInGroup )
        {
            startChatView()
        }
        else
        {
            applyJoinGroup()
        }
    }

    function startChatView()
    {
        console.debug("startChatView")
    }

    function applyJoinGroup()
    {
        console.debug("applyJoinGroup")
        hintitem.showHint(qsTr("已发送"))
    }

    function clickMemberList()
    {
        memberlistloader.source = "MemberList.qml"
        groupinfomain.visible = false
    }

    function backtoGroupInfo()
    {
        memberlistloader.source = ""
        groupinfomain.visible = true
    }

    Component.onCompleted: {
        console.debug("complete4")
        // TODO set isInGroup property
        // by call cpp interface
    }
}
