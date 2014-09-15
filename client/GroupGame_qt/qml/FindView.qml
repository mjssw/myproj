import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: findview;
    visible: false
    anchors.fill: parent

    Rectangle{
        anchors.fill: parent
        color: "white"
    }

    CommonListView {
        id: findlst
        anchors.fill: parent



        Component.onCompleted: {
            addElement("../res/t2.png", qsTr("加入群"), 60, clickJoinGroup)
            addSpliteElement(qsTr(""), 30)
            addElement("../res/t1.png", qsTr("附近的群"), 60, clickFindGroups)
        }

        function clickJoinGroup()
        {
            console.debug("clickJoinGroup")
        }

        function clickFindGroups()
        {
            console.debug("clickFindGroups")
        }
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }
}
