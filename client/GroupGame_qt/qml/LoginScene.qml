import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: loginscene;
    anchors.fill: parent

    property int offset:15
    property string defaultUserText: qsTr("帐号")
    property string defaultPwsText: qsTr("密码")
    property string defaultRegText: qsTr("注册")
    property string defaultLoginText: qsTr("登录")
    property string backLoginText: qsTr("返回登录")
    property string funcText: defaultRegText
    property string probText: qsTr("无法登陆？")
    property string btnText: defaultLoginText
    property bool islogin: true
    property string logopath: "../res/icon1.png"
    property string resultText: qsTr("帐号密码error")
    property string loginingText: qsTr("正在登录")
    property string registingText: qsTr("正在注册")
    property string loadingText: loginingText

    Item {
        id: mainview
        anchors.fill: parent

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: inputarea.top
            anchors.left: parent.left
            anchors.right: parent.right
            color: "lightsteelblue"
        }

        Rectangle {
            anchors.top: inputarea.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            gradient: Gradient {
                GradientStop { position: 0.0; color: "lightsteelblue" }
                GradientStop { position: 1.0; color: "steelblue" }
            }
        }

        Image {
            id: logo
            source: logopath
            anchors.bottom: inputarea.top
            anchors.bottomMargin: (inputarea.y - sourceSize.height) / 2
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: inputarea
            width: parent.width
            height:parent.height / 7
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: funcbtn.top
            anchors.bottomMargin: funcbtn.height / 3
            color: "white"

            property alias userText: usertext.text
            property alias passText: passtext.text

            Rectangle {
                width: parent.width
                height: 1
                anchors.centerIn: parent
                color: "lightgray"
            }

            Component {
                id: touchStyle

                TextFieldStyle {
                    textColor: "black"
                    font.pixelSize: inputarea.height / 2 * 0.5
                    font.bold: true
                    background: Item {}
                }
            }

            TextField {
                id: usertext
                width: parent.width * 0.9
                height: parent.height / 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                style: touchStyle
                placeholderText: defaultUserText
            }

            TextField {
                id: passtext
                width: parent.width * 0.9
                height: parent.height / 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                style: touchStyle
                echoMode: TextInput.Password
                placeholderText: defaultPwsText
            }
        }

        Rectangle {
            id: funcbtn
            width: parent.width * 0.9
            height: parent.height / 14
            anchors.centerIn: parent
            color: "#C96A6A"
            radius: 8

            property alias pressed: clickarea.pressed
            opacity: pressed ? 0.7 : 0.95

            MouseArea{
                id: clickarea
                anchors.fill: parent
                onClicked: {
                    onclickButton()
                }
            }

            Text {
                anchors.centerIn: parent
                text: btnText
                font.pixelSize: parent.height * 0.6
                font.bold: true
                font.letterSpacing: 10
                color: "white"
            }
        }

        Item {
            id: funcitem
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: offset
            anchors.bottomMargin: offset
            width: functext.width
            height: functext.height

            property int textheight: funcbtn.height * 0.5

            signal clicked
            onClicked: {
                onclickFuncitem()
                console.debug("textheight=", textheight)
            }

            Text {
                id: functext
                text: funcText
                font.pixelSize: parent.textheight
                color: funcmouse.pressed ? "#DEDE6F" : "white"
            }

            MouseArea {
                id: funcmouse
                anchors.fill: parent
                onClicked: {
                    funcitem.clicked()
                }
            }
        }

        Item {
            id: probitem
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: offset
            anchors.bottomMargin: offset
            width: probtext.width
            height: probtext.height

            property int textheight: funcbtn.height * 0.5

            signal clicked
            onClicked: {
                onclickProblemitem()
            }

            Text {
                id: probtext
                text: probText
                font.pixelSize: parent.textheight
                color: probmouse.pressed ? "#DEDE6F" : "white"
            }

            MouseArea {
                id: probmouse
                anchors.fill: parent
                onClicked: {
                    probitem.clicked()
                }
            }
        }

        function getUser()
        {
            return inputarea.userText
        }

        function getPassword()
        {
            return inputarea.passText
        }
    }

    Item {
        id: loadingview
        anchors.fill: parent
        visible: false

        Rectangle {
            id: loadingbg
            anchors.fill: parent
            color: "black"
            opacity: 0.6
        }

        Item {
            id: stateitem
            width: parent.width * 0.9
            height: parent.height / 14
            anchors.top: parent.top
            anchors.topMargin: 3
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                anchors.fill: parent
                color: "black"
                radius: 8
                opacity: 0.7
            }

            Item {
                id: loading
                anchors.fill: parent

                Row {
                    spacing: 10
                    height: parent.height * 0.6
                    anchors.centerIn: parent

                    Item {
                        id: loadingimg
                        width: parent.height
                        height: parent.height
                        NumberAnimation on rotation {
                            from: 0
                            to: 360
                            duration: 5000
                            loops: Animation.Infinite
                        }
                        Image {
                            anchors.fill: parent
                            source: "../res/loading.png"
                            rotation: parent.rotation
                        }
                    }

                    Text {
                        color: "white"
                        font.pixelSize: parent.height * 0.6
                        text: loadingText
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Item {
                id: result
                anchors.fill: parent
                visible: false

                Text {
                    id: errtext
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: parent.height * 0.5
                    text: resultText
                }
            }

            function showState()
            {
                loading.visible = true
                result.visible = false
                if( islogin )
                {
                    loadingText = loginingText
                }
                else
                {
                    loadingText = registingText
                }
            }

            function showResult(retmsg)
            {
                loading.visible = false
                result.visible = true
                resultText = retmsg
            }
        }

        function showLoadingView(isshow)
        {
            visible = isshow
        }

        function showState()
        {
            showLoadingView( true )
            stateitem.showState()
        }

        function showResult(retmsg)
        {
            showLoadingView( true )
            stateitem.showResult(retmsg)
        }
    }

    function onclickProblemitem()
    {
        // TODO
        console.debug("onclickProblemitem")
    }

    function onclickFuncitem()
    {
        if( islogin )
        {
            // change to register
            funcText = backLoginText
            btnText = defaultRegText
        }
        else
        {
            // change to login
            funcText = defaultRegText
            btnText = defaultLoginText
        }
        islogin = !islogin
    }


    function onclickButton()
    {
        if( islogin )
        {
            clickLogin()
        }
        else
        {
            clickRegister()
        }
    }

    function clickLogin()
    {
        var user = mainview.getUser()
        var pass = mainview.getPassword()
        var ret = wrapper.UserLogin(user, pass)
        console.debug("user login:", user, pass, ret)

        if( ret )
        {
            mainview.enabled = false
            loadingview.showState()
        }
        else
        {
            console.error("clickLogin -> wrapper.UserLogin FAILED")
        }
    }

    function clickRegister()
    {
        var user = mainview.getUser()
        var pass = mainview.getPassword()
        var ret = wrapper.UserRegister(user, pass)
        console.debug("user register:", user, pass, ret)

        if( ret )
        {
            mainview.enabled = false
            loadingview.showState()
        }
        else
        {
            console.error("clickRegister -> wrapper.UserRegister FAILED")
        }
    }

    function userLoginFailed(param)
    {
        console.debug("userLoginFailed", param)
        loadingview.showResult(param)
    }

    function userLoginSuccess()
    {
        console.debug("userLoginSuccess")
        loadingText = qsTr("验证成功，正在进入...")
    }

    function registerResult(result)
    {
        console.debug("registerResult", result)
        var msg = result? qsTr("注册成功") : qsTr("注册失败")
        loadingview.showResult(msg)
    }
}
