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
        z: 0.1
        width: parent.width
        height: 88
        anchors.top: parent.top
        anchors.left: parent.left

        property alias backStrText: backbtn.backStrText

        Rectangle {
            id: chatheaderbg;
            anchors.fill: parent
            color: "steelblue"
        }

        Text {
            id: chatheadtext
            text: chatview.headerText
            font.pixelSize: parent.height * 0.4
            color: "white";
            anchors.centerIn: parent;
        }

        Image {
            id: backbtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 30
            source: mouse.pressed ? "../res/back_1.png" : "../res/back_2.png"

            property string backStrText: qsTr("群列表")

            signal clicked
            onClicked: {
                parent.backBtnClicked()
            }

            Text {
                id: backtext
                text: parent.backStrText
                font.pixelSize: parent.height*0.7
                color: mouse.pressed ? "#969696" : "white"
                anchors.left: parent.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                property int off: -20
                id: mouse
                anchors.top: parent.top
                anchors.topMargin: off
                anchors.bottom: parent.bottom
                anchors.bottomMargin: off
                anchors.left: backbtn.left
                anchors.right: backtext.right
                onClicked: {
                    backbtn.clicked()
                }
            }
        }

        function showHeader(isshow)
        {
            visible = isshow
        }

        function backBtnClicked()
        {
            parent.backBtnClicked()
        }
    }

    Rectangle {
        id: chatmenu
        z: 0.1
        anchors.top: chatheader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: chatheader.height/2+10
        color: "white"
        opacity: 0.9

        Rectangle {
            height: 1
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "lightgray"
        }

        GroupBox {
            id: menu
            flat:true
            anchors.fill: parent;
            Row{
                anchors.fill: parent
                ExclusiveGroup { id: exmenu }

                RadioButton {
                    id: chatbtn
                    exclusiveGroup: exmenu
                    height:parent.height
                    width: parent.width/3
                    checked: true
                    onClicked: {
                        clickMenuChat()
                    }
                    style: RadioButtonStyle {
                        panel: Item {
                            anchors.fill: parent;
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter;
                                source: chatbtn.checked? "../res/menuchat_2.png" : "../res/menuchat_1.png"
                            }
                        }
                    }
                }

                RadioButton {
                    id: gamebtn
                    exclusiveGroup: exmenu
                    height:parent.height
                    width: parent.width/3
                    checked: false
                    onClicked: {
                        clickMenuGame()
                    }
                    style: RadioButtonStyle {
                        panel: Item {
                            anchors.fill: parent;
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter;
                                source: gamebtn.checked? "../res/menugame_2.png" : "../res/menugame_1.png"
                            }
                        }
                    }
                }

                RadioButton {
                    id: membersbtn
                    exclusiveGroup: exmenu
                    height:parent.height
                    width: parent.width/3
                    checked: false
                    onClicked: {
                        clickMenuMembers()
                    }

                    style: RadioButtonStyle {
                        panel: Item {
                            anchors.fill: parent;
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter;
                                source: membersbtn.checked? "../res/menumembers_2.png" : "../res/menumembers_1.png"
                            }
                        }
                    }
                }
            }
        }
    }

    Item {
        id: chatcontent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: chatmenu.bottom
        anchors.bottom: parent.bottom

        property string groupname: parent.headerText
        property int headerHeight: chatheader.height
        property int menuHeight: chatmenu.height

        ChatMenu {
            id: chatmenuview
            visible: true
        }

        GameMenu {
            id: gamemenuview
            visible: false
        }

        MembersMenu {
            id: membersmenuview
            visible: false
        }

        function showChatMenu()
        {
            chatmenuview.visible = true
            gamemenuview.visible = false
            membersmenuview.visible = false
            showChatviewHeader(true)
        }

        function showGameMenu()
        {
            chatmenuview.visible = false
            gamemenuview.visible = true
            membersmenuview.visible = false
            showChatviewHeader(true)

            console.debug(groupname, headerHeight, menuHeight)
        }

        function showMembersMenu()
        {
            chatmenuview.visible = false
            gamemenuview.visible = false
            membersmenuview.visible = true
            showChatviewHeader(true)
        }
    }

    function showChatviewHeader(isshow)
    {
        chatheader.visible = isshow
    }

    function backBtnClicked()
    {
        console.debug("chatview::backToGroupList")
        parent.backToGroupList()
    }

    function clickMenuChat()
    {
        console.debug("clickMenuChat")
        chatcontent.showChatMenu()
    }

    function clickMenuGame()
    {
        console.debug("clickMenuGame")
        chatcontent.showGameMenu()
    }

    function clickMenuMembers()
    {
        console.debug("clickMenuMembers")
        chatcontent.showMembersMenu()
    }
}

