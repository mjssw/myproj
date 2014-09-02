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

        TextField {
            anchors.fill: parent
            anchors.leftMargin: parent.width*textoff/default_width
            style: touchStyle
            placeholderText: qsTr("帐号")
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

        TextField {
            anchors.fill: parent
            anchors.leftMargin: parent.width*textoff/default_width
            style: touchStyle
            placeholderText: qsTr("密码")
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
        opacity: pressed?0.7:1

        MouseArea{
            id: clickarea
            anchors.fill: parent
            onClicked: {
                console.debug("press login btn")
            }
        }
    }
}
