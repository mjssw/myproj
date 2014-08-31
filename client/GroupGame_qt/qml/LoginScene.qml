import QtQuick 2.0

Rectangle {
    id: loginscene;
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640
    color: "red"
}
