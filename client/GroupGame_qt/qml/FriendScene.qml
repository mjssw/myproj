import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: friendscene;
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640


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

    Image  {
        id: toolbar;
        source: "../res/bottombar.png";
        anchors.left: parent.left;
        anchors.bottom : parent.bottom;
        width: parent.width;
        z: 0.9;
    }

    Rectangle{
        id: rectlist;
        anchors.top: titlebar.bottom;
        anchors.bottom: toolbar.bottom;
        anchors.left: parent.left;
        anchors.right:parent.right;
        //color: "#787878"
        color: "#FFFFFF"
        z: 0.5;

        ListModel {
            id: grouplist;
            ListElement{
                name:"abc";
                phone:"123"
            }
            ListElement{
                name:"xyz";
                phone:"456"
            }
            ListElement{
                name:"xyz";
                phone:"456"
            }
            ListElement{
                name:"xyz";
                phone:"456"
            }
            ListElement{
                name:"xyz";
                phone:"456"
            }
            ListElement{
                name:"xyz";
                phone:"456"
            }
            ListElement{
                name:"xyz";
                phone:"456"
            }
        }

        Component {
            id: groupdelegate
            Item {
                width: friendscene.width;
                height: 90
                Image {
                    id: icon
                    source: "../res/groupdefault.png";
                }
                Text {
                    id: nameField;
                    text: name
                    anchors.left: icon.right
                }
                Text {
                    text: '\t PHONE:' + phone;
                    anchors.left: nameField.right
                }
            }
        }

        ListView {
            anchors.fill: parent
            model: grouplist
            delegate: groupdelegate
        }
    }

    Row{
        anchors.centerIn: toolbar;
        z: 1.0;

        ImageBtn {
            implicitHeight:toolbar.height
            implicitWidth: toolbar.width/4
            anchors.topMargin:toolbar.height/6
            text: "朋友"
            btnSource: pressed ? "../res/friend_2.png" : "../res/friend_1.png"
            onClicked: {}
        }

        ImageBtn {
            implicitHeight:toolbar.height
            implicitWidth: toolbar.width/4
            anchors.topMargin:toolbar.height/6
            text: "游戏"
            btnSource: pressed ? "../res/game_2.png" : "../res/game_1.png"
            onClicked: {}
        }

        ImageBtn {
            implicitHeight:toolbar.height
            implicitWidth: toolbar.width/4
            anchors.topMargin:toolbar.height/6
            text: "发现"
            btnSource: pressed ? "../res/find_2.png" : "../res/find_1.png"
            onClicked: {}
        }

        ImageBtn {
            implicitHeight:toolbar.height
            implicitWidth: toolbar.width/4
            anchors.topMargin:toolbar.height/6
            text: "更多"
            btnSource: pressed ? "../res/more_2.png" : "../res/more_1.png"
            onClicked: {}
        }
    }
}
