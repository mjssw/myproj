import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQml.Models 2.1
import "qml"

ApplicationWindow {
    id: root
    color: "lightgray"
    width: 480
    height: 640
    visible: true
    property bool printDestruction: false


//! [0]
    Item {
        id: itemModel
        anchors { fill: parent; bottomMargin: 30 }

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

        Rectangle {
            id: page1
            //width: view.width; height: view.height
            anchors { fill: parent;}
            color: "#FFFEF0"
            Text { text: "Page 1"; font.bold: true; anchors.centerIn: parent }
            visible:true
            opacity: 1
            //Component.onDestruction: if (printDestruction) print("destroyed 1")
        }
        Rectangle {
            id: page2
            //width: view.width; height: view.height
            anchors { fill: parent;}
            color: "#F0FFF7"
            Text { text: "Page 2"; font.bold: true; anchors.centerIn: parent }
            visible:false
            opacity: 1
            //Component.onDestruction: if (printDestruction) print("destroyed 2")
        }
        Rectangle {
            id: page3
            //width: view.width; height: view.height
            anchors { fill: parent;}
            color: "#F4F0FF"
            Text { text: "Page 3"; font.bold: true; anchors.centerIn: parent }
            visible:false
            opacity: 1
            //Component.onDestruction: if (printDestruction) print("destroyed 3")
        }
    }

//! [0]
    Rectangle {
        width: root.width; height: 30
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
