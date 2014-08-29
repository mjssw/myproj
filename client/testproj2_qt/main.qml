import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    id: wnd;
    visible: true
    width: Qt.platform.os === "android"? Screen.width: 480
    height: Qt.platform.os === "android"? Screen.height: 640

    Rectangle {
        color: cppColor
        anchors.fill: parent;
    }


    Image {
        id: titlebar;
        source: "res/topbar.png";
        anchors.top : parent.top;
        width: parent.width;
    }

    Image  {
        id: toolbar;
        source: "res/bottombar.png";
        anchors.left: parent.left;
        anchors.bottom : parent.bottom;
        width: parent.width;
    }
    //*/

    function btn2func()
    {
        //lb1.text=mgr.User;
        console.log( mgr.User );
        console.log("btn2")
        resetLabel(mgr.User)
    }

    function btn1func()
    {
        mgr.User = "user1";
        console.log("btn1")
    }

    function resetLabel(param)
    {
        lb1.text = param
    }

    function btn3func()
    {
        console.log(mgr.getText1());
        console.log(mgr.getText2());
        console.log(mgr.testmethod());
        resetLabel(mgr.testmethod())
    }

    Connections {  // 关键：Connections就是专门用来获取qml对象的信号的
            target: mgr  // 目标对象
            onResetLabel: {  // 接收信号的名字，注意前面有个on，信号名第一个字母要大写
                resetLabel(text);
            }
        }

    Button {
        id: btn
        anchors.top: titlebar.bottom;
        anchors.left: parent.left;
        width:200
        height:100
        text: "setuser"
        onClicked:btn1func()
    }

    Button {
        id: btn2
        anchors.top: titlebar.bottom;
        anchors.right: parent.right;
        width:200
        height:100
        text: "getuser"
        onClicked:btn2func()
    }

    Button {
        id: btn3
        anchors.top: btn.bottom;
        anchors.left: parent.left;
        width:200
        height:100
        text: "callcpp_getText1"
        onClicked:btn3func()
    }

    Button {
        id: btn4
        anchors.top: btn2.bottom;
        anchors.right: parent.right;
        width:200
        height:100
        text: "resetlabel"
        onClicked:mgr.func1("hello");
    }

    Label{
        id: lb1;
        height:100
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: toolbar.top;
        text: "default"
        color: "red"
    }
}
