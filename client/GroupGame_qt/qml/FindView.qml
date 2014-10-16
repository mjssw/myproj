import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.2

Item {
    id: findview;
    visible: false
    anchors.fill: parent

    property int headerHeight: parent.headerHeight
    property int tailerHeight: parent.tailerHeight

    property int elemH: headerHeight
    property int spliteH: headerHeight * 0.3

    Loader {
        id: findloader
        source: ""

        anchors.top: parent.top
        anchors.topMargin: -parent.headerHeight
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height + parent.headerHeight + parent.tailerHeight

        function backFindView()
        {
            parent.backFindView()
        }

        onLoaded: {
            console.debug("@@@@@@@@=1", Screen.desktopAvailableHeight, Screen.height)
        }
    }

    Rectangle{
        id: findlist
        anchors.fill: parent
        color: "white"

        CommonListView {
            id: findlst
            anchors.fill: parent

            property int keySearchGroup: 101
            property int keyNearGroup: 102

            Component.onCompleted: {
                addElement("../res/t2.png", qsTr("搜索群"), findview.elemH, keySearchGroup)
                addSpliteElement(qsTr(""), findview.spliteH)
                addElement("../res/t1.png", qsTr("附近的群"), findview.elemH, keyNearGroup)

                console.debug("@@@@@@@@=2", Screen.desktopAvailableHeight, Screen.height)
            }

            function clickElement(clickkey)
            {
                switch(clickkey)
                {
                case keySearchGroup:
                    clickFindGroups()
                    break
                case keyNearGroup:
                    clickJoinGroup()
                    break;
                }
            }
        }
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }

    function clickJoinGroup()
    {
        console.debug("clickJoinGroup")
    }

    function clickFindGroups()
    {
        console.debug("clickFindGroups")
        findloader.source = "SearchGroup.qml"
        findlist.visible = false
        parent.showHeaderAndTailer(false)
    }

    function backFindView()
    {
        findloader.source = ""
        findlist.visible = true
        parent.showHeaderAndTailer(true)
    }

    function testFunc()
    {
        console.debug("findview::testFunc")
        clickFindGroups()
    }
}
