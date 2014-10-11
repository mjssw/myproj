import QtQuick 2.0

Rectangle {
    id: searchgroup
    anchors.fill: parent
    color: "white"

    property int off: 5
    property int headerH: height / 10

    Loader {
        id: groupinfoloader
        source: ""
        anchors.fill: parent

        function clickBackButton()
        {
            source = ""
            searchview.visible = true
        }
    }

    Item {
        id: searchview
        anchors.fill: parent

        Item{
            id: findresult
            anchors.top: searchitem.bottom
            anchors.topMargin: parent.off
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            property int elemH: searchgroup.headerH
            property int spliteH: elemH * 0.3

            CommonListView {
                id: findlst
                anchors.fill: parent
                hoverenable: true

                property int joinkey: 0

                Component.onCompleted: {
                }

                function clickElement(clickkey)
                {
                    showGroupInfo()
                }

                function enterElement(clickkey, index)
                {
                }

                function leaveElement(clickkey, index)
                {
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
            height: searchgroup.headerH
            headtext: qsTr("发现")

            CommBackbtn {
                id: backbtn

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
            height: searchgroup.headerH * 1.2
            width: parent.width

            Item {
                id: searchedit
                anchors.left: parent.left
                anchors.leftMargin: off
                height: parent.height - 2*off
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width * 0.7

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

    function showGroupInfo()
    {
        groupinfoloader.source = "GroupInfo.qml"
        searchview.visible = false

        console.debug("groupinfoloader:", groupinfoloader.width, groupinfoloader.height)
    }
}
