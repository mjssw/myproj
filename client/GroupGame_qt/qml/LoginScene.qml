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

    Component {
        id: touchStyle

        TextFieldStyle {
            textColor: "white"
            font.pixelSize: 28
            background: Item {}
        }
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
                onclickLogin()
            }
        }
    }

    function onclickLogin()
    {
        console.debug("loginscene >> press login btn", userinput.userText, passinput.passText)
        parent.userLogin(userinput.userText, passinput.passText)
    }
}
