import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: moreview;
    visible: false
    anchors.fill: parent

    property int selfH: 100
    property int elemH: 80
    property int spliteH: 40

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
            addElement("../res/boy.png", qsTr("帐号管理"), moreview.selfH, keyUserManager)
            addSpliteElement(qsTr(""), moreview.spliteH)
            addElement("../res/t2.png", qsTr("关于"), moreview.elemH, keyAbout)
            addSpliteElement(qsTr(""), moreview.spliteH)
            addElement("../res/t3.png", qsTr("帮助"), moreview.elemH, keyHelp)
        }

        function clickElement(clickkey)
        {
            console.debug(clickkey)
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
        morelst.testFunc()
    }
}
