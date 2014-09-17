import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: moreview;
    visible: false
    anchors.fill: parent

    Rectangle{
        anchors.fill: parent
        color: "white"
    }

    CommonListView {
        id: findlst
        anchors.fill: parent

        property int keyUserManager: 201
        property int keyAbout: 202
        property int keyHelp: 203

        Component.onCompleted: {
            addElement("../res/boy.png", qsTr("帐号管理"), 100, keyUserManager)
            addSpliteElement(qsTr(""), 30)
            addElement("../res/t2.png", qsTr("关于"), 60, keyAbout)
            addSpliteElement(qsTr(""), 30)
            addElement("../res/t3.png", qsTr("帮助"), 60, keyHelp)
        }

        function clickElement(clickkey)
        {
        }
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }
}
