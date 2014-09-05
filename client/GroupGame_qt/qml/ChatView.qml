import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    id: chatview;
    width: parent.width

    property int minMsgHeight: 40
    property string headerText: ""

    Item {
        id: chatheader
        width: parent.width
        height: chatheaderbg.sourceSize.height
        anchors.top: parent.top
        anchors.left: parent.left

        BorderImage {
            id: chatheaderbg;
            source: "../res/topbar.png";
            border.left: 1; border.right: 1;
            border.top: 1; border.bottom: 2;
            anchors.fill: parent
        }

        Text {
            id: chatheadtext
            text: chatview.headerText
            font.pixelSize: 35;
            color: "white";
            anchors.centerIn: parent;
        }

        Image {
            id: backbtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 30
            source: mouse.pressed ? "../res/back_2.png" : "../res/back_1.png"

            signal clicked
            onClicked: {
                parent.backToGroupList()
            }

            MouseArea {
                property int off: -20
                id: mouse
                anchors.fill: parent
                anchors.leftMargin: off
                anchors.rightMargin: off
                anchors.topMargin: off
                anchors.bottomMargin: off
                onClicked: {
                    backbtn.clicked()
                }
            }
        }

        function showHeader(isshow)
        {
            visible = isshow
        }

        function backToGroupList()
        {
            parent.backToGroupList()
        }
    }

    Item {
        id: chatbar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: inputbg.sourceSize.height

        property int inputbgDefaultHeight: 90
        BorderImage {
            id: inputbg
            source: "../res/input.png"
            border.bottom: 1; border.top:1;
            border.left: 1; border.right: 1;
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


        TextEdit{
            property int signleHeight: 40

            id: inputtext
            height: signleHeight
            anchors.verticalCenter: inputbg.verticalCenter
            anchors.left: splite1.right
            anchors.leftMargin: 15
            anchors.right: smilebtn.left
            anchors.rightMargin: 15

            color: "black"
            wrapMode: TextEdit.Wrap
            font.pixelSize: 30

            property int lastline: 1
            onCursorRectangleChanged:{
                if( lastline < lineCount || lastline > lineCount )
                {
                    height = signleHeight * lineCount
                    lastline = lineCount

                    inputbg.height = parent.inputbgDefaultHeight + (lineCount-1)*signleHeight
                    chatbar.height = inputbg.height
                    //chatmsglist.positionViewAtEnd()
                }
            }
        }

        function clickAddMoreBtn()
        {
            parent.clickAddMoreBtn()
        }

        function clickSendMsg()
        {
            parent.clickSendMsg()
        }

        function clickSmileBtn()
        {
            parent.clickSmileBtn()
        }

        function formatInputMsg()
        {
            var bkPos=[]
            var singlelineh = inputtext.contentHeight / inputtext.lineCount
            for(var i=0; i<inputtext.lineCount; ++i)
            {
                var pos = inputtext.positionAt(0, (singlelineh/2)+(i*singlelineh))
                bkPos.push(pos)
            }
            bkPos.push(inputtext.text.length)
            var str = ""
            for (var i=0; i<bkPos.length-1; i++)
            {
                var from, to
                from=bkPos[i]; to=bkPos[i+1];
                var substr=inputtext.text.substring(from, to)
                str += substr
                if( i+1 != bkPos.length-1)
                {
                    str += "\n"
                }
            }
            inputtext.text = ""
            return str
        }
    }

    Item {
        id: chatmsgview
        anchors.top: chatheader.bottom
        anchors.bottom: chatbar.top
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle {
            id: chatbg
            anchors.fill: parent
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

        function sendMsg(sender_, isself_, msg)
        {
            chatmsgmodel.append({
                                    msgcontent: msg,
                                    isself: isself_,
                                    sender: sender_,
                                })
            chatmsglist.positionViewAtEnd()
        }
    }

    function backToGroupList()
    {
        console.debug("chatview::backToGroupList")
        parent.backToGroupList()
    }

    function clickSendMsg()
    {
        var formatstr = chatbar.formatInputMsg()
        chatmsgview.sendMsg("", 1, formatstr)
    }

    function clickAddMoreBtn()
    {
        var formatstr = chatbar.formatInputMsg()
        chatmsgview.sendMsg("", 0, formatstr)
    }

    function clickSmileBtn()
    {
        console.debug("clickSmileBtn")
    }
}

