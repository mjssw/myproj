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
        anchors.left: parent.left;
        anchors.bottom : parent.bottom;
        width: parent.width;
        z: 0.9;
        //opacity: 0.5
    }

    GroupBox {
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
                text: qsTr("朋友")
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
                text: qsTr("游戏")
                btnSource: checked ? "../res/game_2.png" : "../res/game_1.png"
                onClicked: clickViewBtn(gameIdx)
            }

            ImageRadioBtn {
                id: findbtn
                exclusiveGroup: group
                implicitHeight:parent.height
                implicitWidth: parent.width/4
                anchors.topMargin:parent.height/10
                text: qsTr("发现")
                btnSource: checked ? "../res/find_2.png" : "../res/find_1.png"
                onClicked: clickViewBtn(findIdx)
            }

            ImageRadioBtn {
                id: morebtn
                exclusiveGroup: group
                implicitHeight:parent.height
                implicitWidth: parent.width/4
                anchors.topMargin:parent.height/10
                text: qsTr("更多")
                btnSource: checked ? "../res/more_2.png" : "../res/more_1.png"
                onClicked: clickViewBtn(moreIdx)
            }
            //*/
        }
    }

    function clickViewBtn(viewidx)
    {
        console.log("clickviewbtn:", viewidx)
        views.showView(viewidx)
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
        anchors.bottom: parent.bottom

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
                parent.labeltext = qsTr("朋友")
            }
            else if(viewidx === gameIdx)
            {
                gameview.visible = true;
                parent.labeltext = qsTr("游戏")
            }
            else if(viewidx === findIdx)
            {
                findview.visible = true;
                parent.labeltext = qsTr("发现")
            }
            else if(viewidx === moreIdx)
            {
                moreview.visible = true;
                parent.labeltext = qsTr("更多")
            }
        }

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
    }
}
