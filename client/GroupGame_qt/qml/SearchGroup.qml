import QtQuick 2.0

Rectangle {
    id: searchgroup
    anchors.fill: parent
    color: "white"

    property int off: 5

    Item{
        id: findresult
        anchors.top: searchitem.bottom
        anchors.topMargin: parent.off
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        property int spliteH: 30
        property int elemH: 50

        CommonListView {
            id: findlst
            anchors.fill: parent
            hoverenable: true

            property int joinkey: 0

            Component.onCompleted: {
            }

            function clickElement(clickkey)
            {
            }

            function enterElement(clickkey, index)
            {
                joinbtn.visible = true
                var pos = parent.spliteH + parent.elemH*(index-1) + (parent.elemH-joinbtn.height)/2
                joinbtn.anchors.topMargin = pos
                joinkey = clickkey
            }

            function leaveElement(clickkey, index)
            {
                joinbtn.visible = false
            }

            Item {
                id: joinbtn
                visible: false
                anchors.right: parent.right
                anchors.rightMargin: searchgroup.off
                anchors.top: parent.top
                height: findresult.elemH * 0.5
                width: height * 1.5
                CommColorButton {
                    id: colorbtn
                    color: "red"
                    btnText: qsTr("加入")

                    function clickButton()
                    {
                        console.debug("click join btn", findlst.joinkey)
                        showHint(qsTr("已发送"))
                    }
                }
            }
        }

        function addSplite()
        {
            findlst.addSpliteElement(qsTr(""), spliteH)
        }

        function addResult(icon_, str_, key_)
        {
            findlst.addElement(icon_, str_, elemH, key_)
        }

        function clearAll()
        {
            findlst.clearAll()
        }
    }

    CommHeader {
        id: header
        headtext: qsTr("发现")

        CommBackbtn {
            id: backbtn
            backStr: qsTr("返回")

            function clickBackButton()
            {
                onclickBackButton()
            }
        }
    }

    Rectangle {
        id: searchitem
        anchors.left: parent.left
        anchors.top: header.bottom
        height: 60
        width: parent.width

        Item {
            id: searchedit
            anchors.left: parent.left
            anchors.leftMargin: off
            height: parent.height - 2*off
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.6

            CommEdit {
                id: edit
                defaultText: qsTr("查找群号")
            }

            function getEditText()
            {
                return edit.getEditText()
            }
        }

        Item {
            id: searchbtn
            anchors.right: parent.right
            anchors.rightMargin: off
            anchors.top: parent.top
            anchors.topMargin: off
            anchors.left: searchedit.right
            anchors.leftMargin: off
            height: searchedit.height

            CommColorButton {
                btnText: qsTr("查找")
                letterspacing: 10

                function clickButton()
                {
                    console.debug("clickButton")
                    clickSearch()
                }
            }
        }
    }

    Item {
        id: hintitem
        width: parent.width * 0.9
        height: parent.height / 14
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        CommHint {
            id: hint
        }

        function showHint(text)
        {
            hintitem.visible = true
            hint.showHint(text)
        }
    }

    function onclickBackButton()
    {
        console.debug("backfindview")
        parent.backFindView()
    }

    function clickSearch()
    {
        console.debug("clickSearch", searchedit.getEditText())
        findresult.clearAll()
        findresult.addSplite()
        findresult.addResult("../res/t1.png", qsTr("g1"), 123)
        findresult.addResult("../res/t1.png", qsTr("g2"), 125)
    }

    function showHint(text)
    {
        hintitem.showHint(text)
    }
}
