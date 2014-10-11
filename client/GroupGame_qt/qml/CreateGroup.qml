import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: creategroupview
    width: parent.width

    property string resultText: ""
    property int timerstate: 0
    property int headerH: parent.headerHeight

    Rectangle {
        id: createscene
        anchors.fill: parent
        color: "white"

        Image {
            id: groupinfo
            height: creategroupview.headerH
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -height
            source: "../res/input.png"

            property int off: 10
            property alias groupname: inputname.text

            Text {
                id: strname
                text: qsTr("群名称：")
                font.pixelSize: parent.height * 0.4
                color: "black"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: parent.off
            }

           TextField {
                id: inputname
                width: parent.width - strname.width - 2*parent.off
                height: strname.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: strname.right
                anchors.leftMargin: parent.off
                style: TextFieldStyle {
                    background: Item {}
                }
                textColor: "black"
                maximumLength: 20
                font.pixelSize: strname.font.pixelSize * 0.8
                placeholderText: qsTr("点击输入群名称")
            }
        }

        Rectangle {
            id: btn
            width: groupinfo.width * 0.8
            height: groupinfo.height * 0.8
            anchors.top: groupinfo.bottom
            anchors.topMargin: groupinfo.height / 2
            anchors.horizontalCenter: parent.horizontalCenter
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
                text: qsTr("确认创建")
                font.pixelSize: parent.height * 0.6
                font.letterSpacing: 6
                font.weight: Font.DemiBold
                color: "white"
            }
        }

        function getGroupName()
        {
            return groupinfo.groupname
        }
    }

    Rectangle {
        id: loadingview
        anchors.fill: parent
        color: "black"
        opacity: 0.6
        visible: false

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
                        text: qsTr("正在创建群：") + createscene.getGroupName()
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
                    font.pixelSize: parent.height * 0.6
                    text: resultText
                }
            }

            function showState()
            {
                loading.visible = true
                result.visible = false
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

    Timer {
        id: waiting
        interval: 5000
        repeat: false

        onTriggered: {
            ontimer()
        }
    }

    function onclickButton()
    {
        createscene.enabled = false
        loadingview.showState()
        waiting.start()
        timerstate = 0
    }

    function ontimer()
    {
        console.debug("ontimer state=", timerstate)
        if(timerstate == 0)
        {
            waiting.interval = 2000
            loadingview.showResult(qsTr("创建成功"))
            waiting.start()
            timerstate = 1
        }
        else
        {
            createscene.enabled = true
            loadingview.visible = false
        }
    }
}
