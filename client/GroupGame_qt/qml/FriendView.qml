import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: friendview;
    visible: true
    anchors.fill: parent

    Rectangle{
        anchors.fill: parent
        color:"green"
    }

    Label{
        anchors.fill: parent
        text: qsTr("你好哈哈哈")
    }

    function showView(isshow)
    {
        visible = isshow ? true : false;
    }
}
