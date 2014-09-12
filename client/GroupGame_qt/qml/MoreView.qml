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
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }
}
