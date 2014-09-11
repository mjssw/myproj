import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: loginscene;
    anchors.fill: parent

    BorderImage {
        id: loginbg
        anchors.fill : parent
        source: "../res/loginbg.png"
    }

    property int default_width: 640
    property int default_height: 960
    property int offset:15
    property int textoff:120
    property string defaultUserText: qsTr("帐号")
    property string defaultPwsText: qsTr("密码")
    property string defaultRegText: qsTr("注册")
    property string defaultLoginText: qsTr("登录")
    property string backLoginText: qsTr("返回登录")
    property string titleText: defaultLoginText
    property string funcText: defaultRegText
    property string btnText: defaultLoginText
    property bool islogin: true

    Component {
        id: touchStyle

        TextFieldStyle {
            textColor: "white"
            font.pixelSize: 28
            background: Item {}
        }
    }

    Text {
        id: titletext
        text: titleText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/4
        font.pixelSize: 50
        color: "white"
    }

    Image {
        id: userinput
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "../res/user.png"
        width: parent.width*sourceSize.width/default_width
        height:parent.height*sourceSize.height/default_height

        property alias userText: usertext.text

        TextField {
            id: usertext
            anchors.fill: parent
            anchors.leftMargin: parent.width*textoff/default_width
            style: touchStyle
            placeholderText: defaultUserText
        }
    }

    Item {
        id: funcitem
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: offset
        anchors.topMargin: offset
        width: functext.width
        height: functext.height

        signal clicked
        onClicked: {
            onclickFuncitem()
        }

        Text {
            id: functext
            text: funcText
            font.pixelSize: 25
            font.underline: true
            color: mouse.pressed ? "#969696" : "white"
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            onClicked: {
                funcitem.clicked()
            }
        }
    }

    Image {
        id: passinput
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: userinput.bottom
        anchors.topMargin: offset
        source: "../res/password.png"
        width: parent.width*sourceSize.width/default_width
        height:parent.height*sourceSize.height/default_height

        property alias passText: passtext.text

        TextField {
            id: passtext
            anchors.fill: parent
            anchors.leftMargin: parent.width*textoff/default_width
            style: touchStyle
            placeholderText: defaultPwsText
        }
    }

    Image {
        id: loginbtn
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:passinput.bottom
        anchors.topMargin: offset
        source: "../res/loginbtn.png"
        width: parent.width*sourceSize.width/default_width
        height: parent.height*sourceSize.height/default_height

        property alias pressed: clickarea.pressed
        opacity: pressed ? 0.7 : 1

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
            font.pixelSize: 30
            font.bold: true
            color: "white"
        }
    }

    Text {
        id: errtext
        anchors.top: loginbtn.bottom
        anchors.topMargin: offset
        anchors.horizontalCenter: parent.horizontalCenter

        color: "yellow"
        font.pixelSize: 25
        text: ""
        visible: false
    }

    Item {
        id: loading
        width: loadingimg.sourceSize.width
        height: loadingimg.sourceSize.height
        anchors.centerIn: parent
        visible: false
        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 2000
            loops: Animation.Infinite
        }
        Image {
            id: loadingimg
            anchors.fill: parent
            source: "../res/loading.png"
            rotation: parent.rotation
        }
    }

    function onclickButton()
    {
        if( islogin )
        {
            onclickLogin()
        }
        else
        {
            onclickRegister()
        }
    }

    function onclickLogin()
    {
        var ret = parent.userLogin(userinput.userText, passinput.passText)
        console.debug("loginscene >> press login btn", userinput.userText, passinput.passText, ret)

        errtext.visible = false
        if( ret )
        {
            userinput.visible = false
            passinput.visible = false
            loginbtn.visible = false
            funcitem.visible = false
            loading.visible = true
        }
        else
        {
            errtext.visible = true
            errtext.text = qsTr("连接服务器失败")
        }
    }

    function onclickRegister()
    {
        console.debug("onclickRegister")
        var ret = wrapper.UserRegister(userinput.userText, passinput.passText)

        errtext.visible = false
        if( ret )
        {
            userinput.visible = false
            passinput.visible = false
            loginbtn.visible = false
            funcitem.visible = false
            loading.visible = true
        }
        else
        {
            errtext.visible = true
            errtext.text = qsTr("连接服务器失败")
        }
    }

    function onclickFuncitem()
    {
        if( islogin )
        {
            // change to register
            titleText = defaultRegText
            funcText = backLoginText
            btnText = defaultRegText
        }
        else
        {
            // change to login
            titleText = defaultLoginText
            funcText = defaultRegText
            btnText = defaultLoginText
        }
        islogin = !islogin
    }

    function userLoginFailed(param)
    {
        console.debug("LoginScene->connectLoginError", param)
        userinput.visible = true
        passinput.visible = true
        loginbtn.visible = true
        loading.visible = false
        errtext.visible = true
        errtext.text = param
        funcitem.visible = true
    }

    function userLoginSuccess()
    {
        errtext.visible = true
        errtext.text = qsTr("登录成功，正在进入，请稍后...")
    }

    function initMainScene()
    {

    }

    function registerResult(result)
    {
        userinput.visible = true
        passinput.visible = true
        loginbtn.visible = true
        loading.visible = false
        errtext.visible = true
        errtext.text = result? qsTr("注册成功") : qsTr("注册失败")
        funcitem.visible = true
    }
}
