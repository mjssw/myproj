import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQml.Models 2.1
import QtQuick.Window 2.1
import "qml"

ApplicationWindow {
    id: groupgame
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640

//    Rectangle{
//        anchors.fill: parent
//        color: "white"
//    }

    Item {
        id: itemModel
        //anchors { fill: parent;}

        // test code
        anchors { fill: parent; bottomMargin: 60 }
        function testfunc(id)
        {
            if(id === 1)
            {
                mainscene.visible=true;
                loginscene.visible=false;
                startscene.visible=false;
            }
            else if(id === 2)
            {
                mainscene.visible=false;
                loginscene.visible=true;
                startscene.visible=false;
            }
            else if( id === 3)
            {
                mainscene.visible=false;
                loginscene.visible=false;
                startscene.visible=true;
            }
            else if( id === 4 )
            {
                mainscene.addGroup(
                    qsTr("../res/groupdefault.png"),
                    qsTr("群1"), qsTr("1110001"),
                    12, 50,
                    qsTr("昨天赢了100"), qsTr("2014-09-02 11:25"))
            }
            else if( id === 5 )
            {
            }
        }

        MainScene {
            id: mainscene
            anchors { fill: parent;}
            visible:true
        }

        LoginScene {
            id: loginscene
            anchors { fill: parent;}
            visible:false
        }

        StartScene{
            id: startscene
            anchors { fill: parent;}
            visible:false
        }
    }

    // add test button
    Rectangle {
        width: groupgame.width; height: 30
        anchors { top: itemModel.bottom; bottom: parent.bottom }
        color: "red"
        visible: true

        Row {
            anchors.centerIn: parent
            spacing: 20

            Repeater {
                model: 5

                Rectangle {
                    width: 40; height: 40
                    //radius: 3
                    color: "white"

                    MouseArea {
                        width: 40; height: 40
                        anchors.centerIn: parent
                        onClicked: {
                            console.debug("indx=", index)
                            itemModel.testfunc(index+1)
                        }
                    }
                }
            }
        }
    }//*/

}
