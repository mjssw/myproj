import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

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

    Image {
        id: titlebar;
        source: "../res/topbar.png";
        anchors.top : parent.top;
        width: parent.width;
        z: 0.9;
    }

    Image  {
        id: toolbar;
        source: "../res/bottombar.png";
        anchors.bottom : parent.bottom;
        width: parent.width;
        z: 0.9;
    }

    Image {
        id: backbtn
        visible: false
        anchors.verticalCenter: titlebar.verticalCenter
        anchors.left: titlebar.left
        anchors.leftMargin: 30
        source: mouse.pressed ? "../res/back_2.png" : "../res/back_1.png"
        z: 1

        signal clicked
        onClicked: {
            backToGroupList()
        }
        MouseArea {
            id: mouse
            anchors.fill: parent
            onClicked: {
                backbtn.clicked()
            }
        }
    }

    GroupBox {
        id: gbid
        flat:true
        anchors.fill: toolbar;
        z: 1.0;
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
            //*/
        }
    }

    Text {
        id: viewtext
        text: parent.labeltext;
        font.pixelSize: 35;
        //font.weight: Font.Black;
        color: "white";
        anchors.centerIn: titlebar;
        z: 1.0;
    }

    Item{
        id: views
        anchors.top: titlebar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top

        FriendView{
            id: friendview
            chatHeight: mainscene.height-titlebar.height
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

        function invisableAll()
        {
            friendview.visible = false;
            gameview.visible = false;
            findview.visible = false;
            moreview.visible = false;
        }

        function showView(viewidx)
        {
            console.log("showView:", viewidx)
            invisableAll();
            if( viewidx === friendIdx)
            {
                friendview.visible = true;
                parent.labeltext = mainscene.findtext
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

        function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        {
            console.debug("mainscene addGroup 2")
            friendview.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        }

        function clickGroup(index, groupname)
        {
            console.debug("mainscene views clickGroup idx=", index, "groupname=", groupname)
            parent.clickGroup(index, groupname)
        }

        function backToGroupList()
        {
            console.debug("views backToGroupList")
            friendview.backToGroupList()
        }
    }

    function clickViewBtn(viewidx)
    {
        console.debug("clickviewbtn:", viewidx)
        views.showView(viewidx)
    }

    function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    {
        console.debug("mainscene addGroup")
        views.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
    }

    function showGroupComponts(isshow)
    {
        toolbar.visible=isshow
        //views.visible=isshow
        gbid.visible=isshow
        backbtn.visible=(!isshow)
    }

    function clickGroup(index, groupname)
    {
        console.debug("mainscene clickGroup idx=", index, " groupname=", groupname)
        showGroupComponts(false)
        labeltext=groupname
    }

    function backToGroupList()
    {
        console.debug("backToGroupList")
        showGroupComponts(true)
        labeltext=mainscene.friendtext
        views.backToGroupList()
    }
}
