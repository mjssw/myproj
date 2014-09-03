import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: chatview;
    visible: false
    property int chatHeight: 0
    property int minMsgHeight: 40
    property string realmsg: ""
    property int useRealMsg: 0
    property int lastLineCount: 1
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
            z: 0.5
        }

        Image {
            id: addbtn
            z: 0.5
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: inputbg.left
            anchors.leftMargin: 20
            source: addma.pressed ? "../res/add_2.png" : "../res/add_1.png"

            signal clicked
            onClicked: {
                parent.clickAddMoreBtn()
            }
            MouseArea {
                id: addma
                anchors.fill: parent
                anchors.topMargin: -(inputbg.height-parent.height)/2
                anchors.bottomMargin: -(inputbg.height-parent.height)/2
                onClicked: {
                    addbtn.clicked()
                }
            }
        }

        Image {
            id: splite1
            z: 0.5
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: addbtn.right
            anchors.leftMargin: 20
            source: "../res/split.png"
        }

        Image {
            id: sendbtn
            z: 0.5
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.right: inputbg.right
            anchors.rightMargin: 20
            source: sendma.pressed ? "../res/send_2.png" : "../res/send_1.png"

            signal clicked
            onClicked: {
                parent.clickSendMsg()
            }
            MouseArea {
                id: sendma
                anchors.fill: parent
                anchors.topMargin: -(inputbg.height-parent.height)/2
                anchors.bottomMargin: -(inputbg.height-parent.height)/2
                onClicked: {
                    sendbtn.clicked()
                }
            }
        }

        Image {
            id: splite2
            z: 0.5
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.right: sendbtn.left
            anchors.rightMargin: 20
            source: "../res/split.png"
        }

        Image {
            id: smilebtn
            z: 0.5
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.right: splite2.left
            anchors.rightMargin: 10
            source: smilema.pressed ? "../res/smile_2.png" : "../res/smile_1.png"

            signal clicked
            onClicked: {
                parent.clickSmileBtn()
            }
            MouseArea {
                id: smilema
                anchors.fill: parent
                anchors.topMargin: -(inputbg.height-parent.height)/2
                anchors.bottomMargin: -(inputbg.height-parent.height)/2
                onClicked: {
                    smilebtn.clicked()
                }
            }
        }

        TextArea {
            id: inputtext
            z: 0.5
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: splite1.right
            anchors.leftMargin: 15
            anchors.right: smilebtn.left
            anchors.rightMargin: 15
            height: inputbg.height-10
            //length: 10
            //platformMaxImplicitWidth: 20
            verticalAlignment: TextEdit.AlignVCenter
            font.pixelSize: 30

//            onCursorPositionChanged: {
//                console.debug("onCursorPositionChanged")
//            }
            onLineCountChanged: {
                chatview.useRealMsg = 1
                chatview.realmsg = text;
                if(chatview.lastLineCount < lineCount)
                {
                    chatview.realmsg += "\n"
                }
                console.debug("onLineCountChanged line=", lineCount, "chatview.realmsg=", chatview.realmsg)
            }
            onContentItemChanged: {
                console.debug("onContentItemChanged")
            }
            onDataChanged: {

            }
        }

        Rectangle {
            id: chatbg
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: inputbg.top
            color: "#E8E8E8"
        }

        ListModel {
            id: chatmsgmodel
        }
        Component {
            id: chatmsgdelegate

            Item {
                width: chatbg.width
                height: msgbg.height+20>chatview.minMsgHeight?msgbg.height+20:chatview.minMsgHeight
                BorderImage {
                    id: msgbg
                    // TODO
                    source: isself===1?"../res/chat_me.png":"../res/chat_friend.png"
                    border.left: isself===1?13:25; border.top: 13
                    border.right: isself===1?25:13; border.bottom: 13
                    anchors.centerIn: msgid
                    anchors.horizontalCenterOffset: isself===1?5:-5
                    height: msgid.height + 30
                    width: msgid.width + 40
                }

                Text {
                    id: msgid
                    text: msgcontent
                    font.pixelSize: 25
                    anchors.right: isself===1?msgsender.left:0
                    anchors.rightMargin: isself===1?30:0
                    anchors.left: isself===0?msgsender.right:0
                    anchors.leftMargin: isself===0?30:0
                    anchors.verticalCenter: parent.verticalCenter
                }

                Image{
                    id: msgsender
                    width: chatview.minMsgHeight
                    height: chatview.minMsgHeight
                    // TODO
                    source: isself===1?"../res/boy.png":"../res/girl.png"
                    anchors.right: isself===1?parent.right:0
                    anchors.rightMargin: isself===1?5:0
                    anchors.left: isself===0?parent.left:0
                    anchors.leftMargin: isself===0?5:0
                    anchors.verticalCenter: parent.verticalCenter
                }

            }
        }
        ListView {
            id: chatmsglist
            anchors.fill: chatbg
            anchors.topMargin: 10
            model: chatmsgmodel
            delegate: chatmsgdelegate
        }


        function clickSendMsg()
        {
            console.debug("clickSendMsg useRealMsg=",useRealMsg,"text=",inputtext.text,"realmsg=",realmsg )
            chatmsgmodel.append({
                                    msgcontent: useRealMsg==0?inputtext.text:realmsg,
                                    isself: 1,
                                    sender: "",
                                })
            chatmsglist.positionViewAtEnd()
        }

        function clickAddMoreBtn()
        {
            console.debug("clickAddMoreBtn useRealMsg=",useRealMsg,"text=",inputtext.text,"realmsg=",realmsg )
            chatmsgmodel.append({
                                    msgcontent: useRealMsg==0?inputtext.text:realmsg,
                                    isself: 0,
                                    sender: "",
                                })
            chatmsglist.positionViewAtEnd()
        }

        function clickSmileBtn()
        {
            console.debug("clickSmileBtn")
        }
    }
}

