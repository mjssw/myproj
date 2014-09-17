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

        CommonListView {
            id: findlst
            anchors.fill: parent

            Component.onCompleted: {
            }

            function clickElement(clickkey)
            {
            }
        }

        function addSplite()
        {
            findlst.addSpliteElement(qsTr(""), 30)
        }

        function addResult(icon_, str_, w_, key_)
        {
            findlst.addElement(icon_, str_, w_, key_)
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

                function clickButton()
                {
                    console.debug("clickButton")
                    clickSearch()
                }
            }
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
        findresult.addResult("../res/t1.png", qsTr("g1"), 50, 123)
    }
}
