import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: moreview;
    visible: false
    anchors.fill: parent

    property int headerH: parent.headerHeight
    property int selfH: headerH * 1.5
    property int elemH: headerH
    property int spliteH: headerH * 0.3

    Rectangle{
        anchors.fill: parent
        color: "white"
    }

    CommonListView {
        id: morelst
        anchors.fill: parent

        property int keyUserManager: 201
        property int keyAbout: 202
        property int keyHelp: 203

        Component.onCompleted: {
            console.debug("<-------> 0", parent.headerH)
            addElement("../res/boy.png", qsTr("帐号管理"), moreview.selfH, keyUserManager)
            addSpliteElement(qsTr(""), moreview.spliteH)
            addElement("../res/t2.png", qsTr("关于"), moreview.elemH, keyAbout)
            addSpliteElement(qsTr(""), moreview.spliteH)
            addElement("../res/t3.png", qsTr("帮助"), moreview.elemH, keyHelp)
        }

        function clickElement(clickkey)
        {
            console.debug(clickkey, parent.headerH)
            morelst.debugInfo()
        }

        function testFunc()
        {
            //addElement("../res/t3.png", qsTr("帮助"), moreview.elemH, keyHelp)
            delElement(keyAbout)
        }
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }

    function testFunc()
    {
        //morelst.testFunc()
        console.debug("headerH=", headerH)
    }

    Component.onCompleted: {
        var h = Qt.platform.os === "android"? Screen.desktopAvailableHeight: 640
        console.debug("<-------> 1", headerH, h)
    }
}
