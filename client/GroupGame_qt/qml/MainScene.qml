import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.1

Item {
    id: mainscene;
    anchors.fill: parent
    visible: true
    property string labeltext: qsTr("朋友")
    property int friendIdx: 1
    property int gameIdx: 2
    property int findIdx: 3
    property int moreIdx: 4
    property string friendtext: qsTr("朋友")
    property string gametext: qsTr("游戏")
    property string findtext: qsTr("发现")
    property string moretext: qsTr("更多")
    property string creategrouptext: qsTr("新建群")

    Item {
        id: views
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: tailer.top

        property alias headerHeight: header.height
        property alias tailerHeight: tailer.height

        FriendView{
            id: friendview
            //chatHeight: mainscene.height-titlebar.height
        }

        GameView{
            id: gameview
        }

        FindView{
            id: findview
        }

        MoreView{
            id: moreview
        }

        CreateGroup {
            id: creategroupview
            visible: false
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height + tailer.height
        }

        function invisableAll()
        {
            friendview.visible = false;
            gameview.visible = false;
            findview.visible = false;
            moreview.visible = false;
            header.showNewGroupButton(false)
            header.showCancelCreateGroup(false)
        }

        function showView(viewidx)
        {
            console.log("showView:", viewidx)
            invisableAll();
            if( viewidx === friendIdx)
            {
                friendview.visible = true;
                parent.labeltext = mainscene.findtext
                header.showNewGroupButton(true)
            }
            else if(viewidx === gameIdx)
            {
                gameview.visible = true;
                parent.labeltext = mainscene.gametext
            }
            else if(viewidx === findIdx)
            {
                findview.visible = true;
                parent.labeltext = mainscene.findtext
            }
            else if(viewidx === moreIdx)
            {
                moreview.visible = true;
                parent.labeltext = mainscene.moretext
            }
        }

        function showCreateGroupView(isshow)
        {
            creategroupview.visible = isshow
        }

        function showHeaderAndTailer(isshow)
        {
            parent.showHeaderAndTailer(isshow)
        }

        function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        {
            console.debug("mainscene addGroup 2")
            friendview.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        }
    }

    Item {
        id: header
        width: parent.width
        height: headerbg.sourceSize.height
        anchors.top: parent.top
        anchors.left: parent.left

        BorderImage {
            id: headerbg;
            source: "../res/topbar.png";
            border.left: 1; border.right: 1;
            border.top: 1; border.bottom: 2;
            anchors.fill: parent
        }

        Text {
            id: headtext
            text: mainscene.labeltext;
            font.pixelSize: 35;
            color: "white";
            anchors.centerIn: parent;
        }

        Item {
            id: newgroup
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: newgrouptext.width+20
            signal clicked
            onClicked: {
                createNewGroup()
            }
            Text {
                id: newgrouptext
                text: mainscene.creategrouptext
                font.pixelSize: 20
                color: mouse.pressed ? "white" : "lightgray"
                anchors.centerIn: parent
            }

            MouseArea {
                property int off: -20
                id: mouse
                anchors.fill: parent
                onClicked: {
                    newgroup.clicked()
                }
            }
        }

        Item {
            id: backbtn
            visible: false
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: backimg.sourceSize.width*3

            signal clicked
            onClicked: {
                cancelCreateGroup()
            }

            Image {
                id: backimg
                anchors.centerIn: parent
                source: backmouse.pressed ? "../res/back_2.png" : "../res/back_1.png"
            }

            MouseArea {
                id: backmouse
                anchors.fill: parent
                onClicked: {
                    backbtn.clicked()
                }
            }
        }

        function showHeader(isshow)
        {
            visible = isshow
        }

        function showNewGroupButton(isshow)
        {
            newgroup.visible = isshow
        }

        function showCancelCreateGroup(isshow)
        {
            backbtn.visible = isshow
        }
    }

    Item {
        id: tailer
        width: parent.width
        height: tailerbg.sourceSize.height
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        BorderImage  {
            id: tailerbg;
            source: "../res/bottombar.png";
            border.left: 1; border.right: 1;
            border.top: 1; border.bottom: 1;
            anchors.fill: parent
        }

        GroupBox {
            id: gbid
            flat:true
            anchors.fill: parent;
            Row{
                anchors.fill: parent

                ExclusiveGroup { id: group }

                ImageRadioBtn {
                    id: friendbtn
                    exclusiveGroup: group
                    implicitHeight:parent.height
                    implicitWidth: parent.width/4
                    anchors.topMargin:parent.height/10
                    text: mainscene.friendtext
                    checked: true
                    btnSource: checked ? "../res/friend_2.png" : "../res/friend_1.png"
                    onClicked: clickViewBtn(friendIdx)
                }

                ImageRadioBtn {
                    id: gamebtn
                    exclusiveGroup: group
                    implicitHeight:parent.height
                    implicitWidth: parent.width/4
                    anchors.topMargin:parent.height/10
                    text: mainscene.gametext
                    btnSource: checked ? "../res/game_2.png" : "../res/game_1.png"
                    onClicked: clickViewBtn(gameIdx)
                }

                ImageRadioBtn {
                    id: findbtn
                    exclusiveGroup: group
                    implicitHeight:parent.height
                    implicitWidth: parent.width/4
                    anchors.topMargin:parent.height/10
                    text: mainscene.findtext
                    btnSource: checked ? "../res/find_2.png" : "../res/find_1.png"
                    onClicked: clickViewBtn(findIdx)
                }

                ImageRadioBtn {
                    id: morebtn
                    exclusiveGroup: group
                    implicitHeight:parent.height
                    implicitWidth: parent.width/4
                    anchors.topMargin:parent.height/10
                    text: mainscene.moretext
                    btnSource: checked ? "../res/more_2.png" : "../res/more_1.png"
                    onClicked: clickViewBtn(moreIdx)
                }
            }
        }

        function showTailer(isshow)
        {
            visible = isshow
        }
    }


    function clickViewBtn(viewidx)
    {
        console.debug("clickviewbtn:", viewidx)
        views.showView(viewidx)
    }

    function showHeaderAndTailer(isshow)
    {
        header.showHeader(isshow)
        tailer.showTailer(isshow)
    }

    function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    {
        console.debug("mainscene addGroup")
        views.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    }

    function createNewGroup()
    {
        console.debug("mainscene createNewGroup")
        views.invisableAll()
        tailer.showTailer(false)
        views.showCreateGroupView(true)
        header.showCancelCreateGroup(true)
        header.showNewGroupButton(false)
        mainscene.labeltext = mainscene.creategrouptext
        //*/

        /*
        console.debug("parent:", parent.width, parent.height, mainscene.height, friendview.height)
        console.debug("current:", header.height, tailer.height, views.height)
        console.debug("avaiable:", Screen.desktopAvailableHeight, Screen.desktopAvailableWidth)
        //*/
    }

    function cancelCreateGroup()
    {
        console.debug("mainscene cancelCreateGroup")
        views.invisableAll()
        views.showView(friendIdx)
        tailer.showTailer(true)
        views.showCreateGroupView(false)
        header.showCancelCreateGroup(false)
        header.showNewGroupButton(true)
        mainscene.labeltext = mainscene.friendtext
    }
}
