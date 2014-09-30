import QtQuick 2.0

Item {
    id: chatmenuview
    anchors.fill: parent
    Item {
        id: chatbar
        z: 0.1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: inputbg.sourceSize.height

        property int moreItembasicHeight: height * 1.5
        property int moreItemHeight: moreItembasicHeight+50

        Rectangle {
            id: moreitem
            visible: true
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 0
            color: "whitesmoke"

            Grid {
                id: itemgrid
                property int bordermargin: 10
                property int elemW: chatbar.moreItembasicHeight
                property int elemH: chatbar.moreItembasicHeight + 30
                property int space: 20
                anchors.fill: parent
                anchors.leftMargin: bordermargin
                anchors.rightMargin: bordermargin
                anchors.topMargin: bordermargin
                anchors.bottomMargin: bordermargin
                columns: ((parent.width-2*bordermargin) + space)/(elemW+space)
                spacing: space

                Item {
                    width: parent.elemW
                    height: parent.elemH
                    MoreGridItem {
                        hinttext: qsTr("添加游戏")
                        iconNormal: "../res/game.png"
                        iconPressed: "../res/game.png"
                    }
                    function onClickGridItem()
                    {
                        console.debug("click grid item add game")
                    }
                }

                Item {
                    width: parent.elemW
                    height: parent.elemH
                    MoreGridItem {
                        hinttext: qsTr("我的位置")
                        iconNormal: "../res/position.png"
                        iconPressed: "../res/position.png"
                    }
                    function onClickGridItem()
                    {
                        console.debug("click grid item add position")
                    }
                }
            }
        }

        property int inputbgDefaultHeight: 90
        BorderImage {
            id: inputbg
            source: "../res/input.png"
            border.bottom: 1; border.top:1;
            border.left: 1; border.right: 1;
            anchors.bottom: moreitem.top
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
                }
            }
        }

        function showBar(isshow)
        {
            visible = isshow
        }

        function clickAddMoreBtn()
        {
            //parent.clickAddMoreBtn()
            moreitem.height = moreItemHeight
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
                if( inputtext.text.charAt(to-1) == '\n')
                {
                    continue
                }

                if( i+1 != bkPos.length-1)
                {
                    str += "\n"
                }
            }
            inputtext.text = ""
            return str
        }

        function hideMoreItemContent()
        {
            moreitem.height = 0
        }
    }

    Item {
        id: chatmsgview
        anchors.top: parent.top
        anchors.bottom: chatbar.top
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle {
            id: chatbg
            anchors.fill: parent
            //color: "#E8E8E8"
            color: "white"
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

        MouseArea {
            // TODO bug here, can click old msgview area
            anchors.fill: parent
            onClicked: {
                console.debug("click blank", parent.height)
                hideMoreItemContent()
            }
        }//*/

        function showMsgView(isshow)
        {
            visible = isshow
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

    function showChatMenuView(isshow)
    {
        visible = isshow
    }

    function clickSendMsg()
    {
        var formatstr = chatbar.formatInputMsg()
        chatmsgview.sendMsg("", 1, formatstr)
        InputMethod.hide()
    }

    function clickAddMoreBtn()
    {
        var formatstr = chatbar.formatInputMsg()
        chatmsgview.sendMsg("", 0, formatstr)
    }

    function clickSmileBtn()
    {
        console.debug("clickSmileBtn chatmsgview->height", chatmsgview.height)
    }

    function hideMoreItemContent()
    {
        chatbar.hideMoreItemContent()
    }
}
