import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: chatview;
    visible: false
    property int chatHeight: 0
    anchors.fill: parent

    Item {
        id: cview
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.chatHeight

        Image {
            id: inputbg
            source: "../res/input.png"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Image {
            id: addbtn
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: inputbg.left
            anchors.leftMargin: 20
            source: addma.pressed ? "../res/add_2.png" : "../res/add_1.png"

            signal clicked
            onClicked: {
            }
            MouseArea {
                id: addma
                anchors.fill: parent
                onClicked: {
                    addbtn.clicked()
                }
            }
        }

        Image {
            id: splite1
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: addbtn.right
            anchors.leftMargin: 20
            source: "../res/split.png"
        }

        Image {
            id: sendbtn
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.right: inputbg.right
            anchors.rightMargin: 20
            source: sendma.pressed ? "../res/send_2.png" : "../res/send_1.png"

            signal clicked
            onClicked: {
            }
            MouseArea {
                id: sendma
                anchors.fill: parent
                onClicked: {
                    addbtn.clicked()
                }
            }
        }

        Image {
            id: splite2
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.right: sendbtn.left
            anchors.rightMargin: 20
            source: "../res/split.png"
        }

        Image {
            id: smilebtn
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.right: splite2.left
            anchors.rightMargin: 10
            source: smilema.pressed ? "../res/smile_2.png" : "../res/smile_1.png"

            signal clicked
            onClicked: {
            }
            MouseArea {
                id: smilema
                anchors.fill: parent
                onClicked: {
                    addbtn.clicked()
                }
            }
        }

        TextArea {
            id: inputtext
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: splite1.right
            anchors.leftMargin: 10
            anchors.right: smilebtn.left
            anchors.rightMargin: 10
            height: inputbg.height-10

        }


//        Component {
//                id: btnStyle
//                ButtonStyle {
//                    panel: Item {
//                        implicitHeight: 50
//                        implicitWidth: 320
//                        BorderImage {
//                            anchors.fill: parent
//                            antialiasing: true
//                            border.bottom: 8
//                            border.top: 8
//                            border.left: 8
//                            border.right: 8
//                            anchors.margins: control.pressed ? -4 : 0
//                            source: control.pressed ? "../images/button_pressed.png" : "../images/button_default.png"
//                            Text {
//                                text: control.text
//                                anchors.centerIn: parent
//                                color: "white"
//                                font.pixelSize: 23
//                                renderType: Text.NativeRendering
//                            }
//                        }
//                    }
//                }
//            }

        BorderImage {
            id: chatbg
            source: "../res/chatbg.png"
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: inputbg.top
        }
    }
}

