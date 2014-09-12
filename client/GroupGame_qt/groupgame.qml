import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.1
import "qml"

ApplicationWindow {
    id: groupgame
    visible: true
    //width: Qt.platform.os === "android"? Screen.width: 480
    //height: Qt.platform.os === "android"? Screen.height: 640
    width: Qt.platform.os === "android"? Screen.desktopAvailableWidth: 480
    height: Qt.platform.os === "android"? Screen.desktopAvailableHeight: 640

    // set debugButton=1 to enable debug buttons, other whise set 0 to disable
    property int debugButton: 1
    property int btmMargin: 60 * debugButton
    Item {
        id: itemModel
        anchors { fill: parent; bottomMargin: groupgame.btmMargin }

        MainScene {
            id: mainscene
            anchors { fill: parent;}
            visible:false
        }

        LoginScene {
            id: loginscene
            anchors { fill: parent;}
            visible:true
        }

        StartScene{
            id: startscene
            anchors { fill: parent;}
            visible:false
        }

        function showMainScene(isshow)
        {
            mainscene.visible = isshow
        }

        function showLoginScene(isshow)
        {
            loginscene.visible = isshow
        }

        function showStartScene(isshow)
        {
            startscene.visible = isshow
        }

        function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        {
            console.debug("groupgame addGroup")
            mainscene.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        }

        function userLogin(user, pwd)
        {
            console.debug("[groupgame][itemmodel] >> userlogin", user, pwd)
            return groupgame.userLogin(user, pwd)
        }

        function userLoginFailed(param)
        {
            loginscene.userLoginFailed(param)
        }

        function userLoginSuccess()
        {
            loginscene.userLoginSuccess()
        }

        function registerResult(result)
        {
            loginscene.registerResult(result)
        }
    }

    function hideAllScene()
    {
        itemModel.showLoginScene(false)
        itemModel.showMainScene(false)
        itemModel.showStartScene(false)
    }

    function showStartScene()
    {
        hideAllScene()
        itemModel.showStartScene(true)
    }

    function showLoginScene()
    {
        hideAllScene()
        itemModel.showLoginScene(true)
    }

    function showMainScene()
    {
        hideAllScene()
        itemModel.showMainScene(true)
    }

    Connections {
        target: wrapper
        onUserLoginFailed: {
            itemModel.userLoginFailed(errmsg)
        }
        onUserLoginSuccess: {
            itemModel.userLoginSuccess()
        }
        onUserLoginGroupDone: {
            showMainScene()
        }
        onAddGroup: {
            itemModel.addGroup(
                qsTr("../res/groupdefault.png"),
                name, id,
                1, 50,
                qsTr(""), qsTr(""))
        }
        onRegisterResult: {
            itemModel.registerResult(result)
        }
    }

    // add test button
    Rectangle {
        id: testbtns
        width: groupgame.width; height: groupgame.btmMargin
        anchors { top: itemModel.bottom; bottom: parent.bottom }
        color: "red"
        visible: groupgame.debugButton==1 ? true : false

        Row {
            anchors.centerIn: parent
            spacing: 20

            Repeater {
                model: 5

                Rectangle {
                    width: 40; height: 40
                    //radius: 3
                    color: "white"

                    MouseArea {
                        width: 40; height: 40
                        anchors.centerIn: parent
                        onClicked: {
                            console.debug("indx=", index)
                            var id = index + 1
                            if(id === 1)
                            {
                                showMainScene()
                            }
                            else if(id === 2)
                            {
                                showLoginScene()
                            }
                            else if( id === 3)
                            {
                                showStartScene()
                            }
                            else if( id === 4 )
                            {
                                itemModel.addGroup(
                                    qsTr("../res/groupdefault.png"),
                                    qsTr("群1"), qsTr("1110001"),
                                    12, 50,
                                    qsTr("昨天赢了100"), qsTr("2014-09-02 11:25"))
                            }
                            else if( id === 5 )
                            {
                                //console.debug(groupgame.height, itemModel.height, testbtns.height)
                                //console.log(mgr.testmethod());
                                //wrapper.UserLogin("aaa", "bbb")
                            }
                        }
                    }
                }
            }
        }
    }//*/

}
