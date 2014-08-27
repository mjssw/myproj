import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    id: wnd;
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640

    Rectangle {
        //color: "#FFFFFF";
        anchors.fill: parent;
    }

    Image {
        id: titlebar;
        source: "res/topbar.png";
        anchors.top : parent.top;
        width: parent.width;
    }

    Image  {
        id: toolbar;
        source: "res/bottombar.png";
        anchors.left: parent.left;
        anchors.bottom : parent.bottom;
        width: parent.width;
    }

    ListModel {
        id: grouplist;
    }

    function pall()
    {
        console.log("wnd.w=", wnd.width, "wnd.h=", wnd.height);
        console.log("toolbar.hcenter=", toolbar.anchors.horizontalCenter, "toolbar.vcenter=", toolbar.anchors.verticalCenter)
        console.log("toolbar.w=", toolbar.width, "wnd.h=", toolbar.height);
        console.log("toolbar.x=", toolbar.x, "wnd.y=", toolbar.y);
    }

    Row{
        anchors.centerIn: toolbar

        Button {
            id: friendbtn
            style: ButtonStyle {
                panel: Item {
                    implicitHeight: toolbar.height
                    implicitWidth: toolbar.width/4
                    BorderImage {
                        anchors.top : parent.top;
                        anchors.topMargin: toolbar.height/6;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        source: friendbtn.pressed ? "res/friend_2.png" : "res/friend_1.png"
                        Text {
                            text: "朋友"
                            anchors.top: parent.bottom;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            color: control.pressed ? "white" : "#929292"
                            font.pixelSize: 20
                            renderType: Text.NativeRendering
                        }
                    }
                }
            }
            onClicked: pall();
        }

        Button {
            id: gamebtn
            style: ButtonStyle {
                panel: Item {
                    implicitHeight: toolbar.height
                    implicitWidth: toolbar.width/4
                    BorderImage {
                        anchors.top : parent.top;
                        anchors.topMargin: toolbar.height/6;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        source: control.pressed ? "res/game_2.png" : "res/game_1.png"
                        Text {
                            text: "游戏"
                            anchors.top: parent.bottom;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            color: control.pressed ? "white" : "#929292"
                            font.pixelSize: 20
                            renderType: Text.NativeRendering
                        }
                    }
                }
            }
            onClicked: pall();
        }

        Button {
            id: findbtn
            style: ButtonStyle {
                panel: Item {
                    implicitHeight: 100
                    implicitWidth: toolbar.width/4
                    BorderImage {
                        anchors.top : parent.top;
                        anchors.topMargin: toolbar.height/6;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        source: control.pressed ? "res/find_2.png" : "res/find_1.png"
                        Text {
                            text: "发现"
                            anchors.top: parent.bottom;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            color: control.pressed ? "white" : "#929292"
                            font.pixelSize: 20
                            renderType: Text.NativeRendering
                        }
                    }
                }
            }
            onClicked: pall();
        }

        Button {
            id: morebtn
            style: ButtonStyle {
                panel: Item {
                    implicitHeight: 100
                    implicitWidth: toolbar.width/4
                    BorderImage {
                        anchors.top : parent.top;
                        anchors.topMargin: toolbar.height/6;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        source: control.pressed ? "res/more_2.png" : "res/more_1.png"
                        Text {
                            text: "更多"
                            anchors.top: parent.bottom;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            color: control.pressed ? "white" : "#929292"
                            font.pixelSize: 20
                            renderType: Text.NativeRendering
                        }
                    }
                }
            }
            onClicked: pall();
        }//*/
    }
}
