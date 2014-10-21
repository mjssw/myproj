import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.2
import "qml"

ApplicationWindow {
    id: groupgame
    visible: true
    //width: Qt.platform.os === "android"? Screen.width: 480
    //height: Qt.platform.os === "android"? Screen.height: 640
    width: Qt.platform.os === "android"? Screen.desktopAvailableWidth: 480
    height: Qt.platform.os === "android"? Screen.desktopAvailableHeight: 640

    Component.onCompleted: {
        console.debug("<-------> 5", height, Screen.desktopAvailableHeight, Screen.height)
    }

    Loader {
        id: mainscene
        anchors.fill: parent
        source: "qml/GroupGame.qml"
    }
}
