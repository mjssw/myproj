import QtQuick 2.0
import "JsListIndex.js" as JsIndex

Item {
    id: gpgame
    anchors.fill: parent

    property int debugButton: 1
    property int btmMargin: 60 * debugButton
    Item {
        id: itemModel
        anchors { fill: parent; bottomMargin: gpgame.btmMargin }
        focus: true
        height: parent.height - gpgame.btmMargin

        Item {
            id: mainitem
            anchors.fill: parent

            Keys.onReleased: {
                console.log("Key pressed: ",event.key)
                if (event.key === Qt.Key_Back) {
                    console.log("Back button pressed.  Stack depth ")
                    //event.accepted = true
                    //wrapper.SendHomeSignal()

                    //event.key = Qt.Key_Home
                    event.accepted = false

                    //emit
                }
                else if(event.key === Qt.Key_Home) {
                    console.debug("android home pressed")
                    event.accetped = false
                }
            }

            Component.onCompleted: {
                console.debug("<-------> 4", height)
            }

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
                return parent.userLogin(user, pwd)
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

            function processGameMessage(msgid, msgdata)
            {
                console.debug(">>> processGameMessage", msgid, msgdata)
                wrapper.TestCppCall2(msgdata)
            }

            function testFunc()
            {
                console.debug("testfunc1")
                mainscene.testFunc()
                //Qt.quit()
            }
        }

        function showMainScene(isshow)
        {
            mainitem.showMainScene(isshow)
        }

        function showLoginScene(isshow)
        {
            mainitem.showLoginScene(isshow)
        }

        function showStartScene(isshow)
        {
            mainitem.showStartScene(isshow)
        }

        function addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        {
            mainitem.addGroup(head, name, groupid, curcount, maxcount, msg, msgtime)
        }

        function userLogin(user, pwd)
        {
            return mainitem.userLogin(user, pwd)
        }

        function userLoginFailed(param)
        {
            mainitem.userLoginFailed(param)
        }

        function userLoginSuccess()
        {
            mainitem.userLoginSuccess()
        }

        function registerResult(result)
        {
            mainitem.registerResult(result)
        }

        function processGameMessage(msgid, msgdata)
        {
            mainitem.processGameMessage(msgid, msgdata)
        }

        function testFunc()
        {
            mainitem.testFunc()
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
        onProcessGameMessage: {
            itemModel.processGameMessage(msgid, msgdata)
        }
    }

    // add test button
    Rectangle {
        id: testbtns
        width: parent.width; height: parent.btmMargin
        anchors { top: itemModel.bottom; bottom: parent.bottom }
        color: "red"
        visible: gpgame.debugButton==1 ? true : false

        property var idx2: JsIndex.createmap()
        property var idx3: JsIndex.createmap()

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
                                //JsIndex.add(testbtns.idx2, '120', 11)
                            }
                            else if( id === 4 )
                            {
                                //JsIndex.set(testbtns.idx2, '120', 13)
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
                                itemModel.testFunc()
                                //wrapper.SendHomeSignal()

                                //JsIndex.add(testbtns.idx2, '120', 1)
                                //JsIndex.display(testbtns.idx2)

                                //console.debug("<+++++>", Screen.desktopAvailableHeight, Screen.height)
                                //wrapper.TestCppCall()
                            }
                        }
                    }
                }
            }
        }
    }//*/
}
