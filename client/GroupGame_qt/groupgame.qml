import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQml.Models 2.1
import "qml"

ApplicationWindow {
    id: groupgame
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640

//! [0]
    Item {
        id: itemModel
        //anchors { fill: parent; bottomMargin: 30 }
        anchors { fill: parent;}

        function showpage(id)
        {
            if(id === 1)
            {
                page1.visible=true;
                page2.visible=false;
                page3.visible=false;
            }
            else if(id === 2)
            {
                page1.visible=false;
                page2.visible=true;
                page3.visible=false;
            }
            else
            {
                page1.visible=false;
                page2.visible=false;
                page3.visible=true;
            }
        }

        MainScene {
            id: page1
            anchors { fill: parent;}
            visible:true
            opacity: 1
        }

        LoginScene {
            id: page2
            anchors { fill: parent;}
            visible:false
            opacity: 1
        }

        StartScene{
            id: page3
            anchors { fill: parent;}
            visible:false
            opacity: 1
        }
    }

//! [0]
    /*
    Rectangle {
        width: groupgame.width; height: 30
        anchors { top: itemModel.bottom; bottom: parent.bottom }
        color: "red"

        Row {
            anchors.centerIn: parent
            spacing: 20

            Repeater {
                model: 3

                Rectangle {
                    width: 5; height: 5
                    radius: 3
                    color: "white"

                    MouseArea {
                        width: 20; height: 20
                        anchors.centerIn: parent
                        onClicked: {
                            console.log("indx=", index)
                            itemModel.showpage(index+1)
                        }
                    }
                }
            }
        }
    }//*/

}
