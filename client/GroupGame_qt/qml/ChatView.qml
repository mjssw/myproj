import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: chatscene;
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640
    color:"green"

    Label {
        text: "朋友";
        font.pixelSize: 35;
        font.weight: Font.Black;
        color: "white";
        anchors.centerIn: titlebar;
        z: 1.0;
    }

    Image {
        id: titlebar;
        source: "../res/topbar.png";
        anchors.top : parent.top;
        width: parent.width;
        z: 0.9;
    }
}

