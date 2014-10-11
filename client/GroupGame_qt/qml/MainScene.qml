import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.1

Item {
    id: mainscene
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

    property int headerH: height / 10
    property int tailerH: height / 10 * 1.2

    Component.onCompleted: {
        console.debug("<-------> 3", height, headerH)
    }

    Item {
        id: views
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: tailer.top
        //z: 0.3

        // careful here
        property int appH: Qt.platform.os === "android"? Screen.desktopAvailableHeight: 640
        property int headerHeight: appH / 10
        property int tailerHeight: appH / 10 * 1.2

        FriendView{
            id: friendview
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

        Component.onCompleted: {
            console.debug("<-------> 2", headerHeight)
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
                parent.labeltext = mainscene.friendtext
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

        function testFunc()
        {
            console.debug("testfunc")
            moreview.testFunc()
        }
    }

    Item {
        id: header
        width: parent.width
        height: parent.headerH
        anchors.top: parent.top
        anchors.left: parent.left

        Rectangle {
            id: headerbg
            anchors.fill: parent
            color: "steelblue"
        }

        Text {
            id: headtext
            text: mainscene.labeltext
            font.pixelSize: parent.height * 0.4
            color: "white"
            anchors.centerIn: parent
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
                font.pixelSize: parent.height/2 * 0.5
                color: mouse.pressed ? "#DEDE6F" : "white"
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
            width: parent.width / 10

            signal clicked
            onClicked: {
                console.debug(backimg.width, backimg.sourceSize.width, backbtn.width)
                cancelCreateGroup()
            }

            Image {
                id: backimg
                anchors.centerIn: parent
                height: parent.height * 0.5
                width: sourceSize.width * height / sourceSize.height
                source: backmouse.pressed ? "../res/back_1.png" : "../res/back_2.png"
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
        height: parent.tailerH
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Rectangle {
            id: tailerbg
            anchors.fill: parent
            border.width: 1
            border.color: "lightgray"
            color: "#F5FAF5"
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
                    text: mainscene.gametext
                    btnSource: checked ? "../res/game_2.png" : "../res/game_1.png"
                    onClicked: clickViewBtn(gameIdx)
                }

                ImageRadioBtn {
                    id: findbtn
                    exclusiveGroup: group
                    implicitHeight:parent.height
                    implicitWidth: parent.width/4
                    text: mainscene.findtext
                    btnSource: checked ? "../res/find_2.png" : "../res/find_1.png"
                    onClicked: clickViewBtn(findIdx)
                }

                ImageRadioBtn {
                    id: morebtn
                    exclusiveGroup: group
                    implicitHeight:parent.height
                    implicitWidth: parent.width/4
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

    function testFunc()
    {
        console.debug("testfunc1")
        views.testFunc()
    }
}
