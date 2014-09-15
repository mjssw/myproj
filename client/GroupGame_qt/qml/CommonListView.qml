import QtQuick 2.0

Item {
    id: commlist
    anchors.fill: parent

    property string listname: "mylistname"

    ListModel {
        id: lstmodel

        /*
        ListElement {
            type: 1
            icon: "../res/0.png"
            str: "abc"
            w: 60
            moretype: 0
            more: ""
            flag: 0
        }

        ListElement {
            type: 1
            icon: "../res/1.png"
            str: "xyz"
            w: 60
            moretype: 1
            more: "xxxx"
            flag: 0
        }

        ListElement {
            type: 0
            icon: ""
            str: "123"
            w: 30
            moretype: 0
            more: ""
            flag: 0
        }

        ListElement {
            type: 1
            icon: "../res/2.png"
            str: "hhh"
            w: 60
            moretype: 2
            more: "../res/3.png"
            flag: 1
        }
        //*/
    }

    Component {
        id: lstdelegate

        Rectangle {
            id: lstitem
            width: commlist.width
            height: w
            color: type==0 ? "lightgray" : "white"

            Rectangle {
                anchors.fill: parent
                color: "#7A7A7A"
                visible: mouse.pressed
            }

            Image {
                id: lstimg
                visible: type==0 ? false : true
                source: icon
                height: parent.height * 0.8
                width: type==0 ? 0 : parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            Text {
                id: lsttext
                text: str
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: type==0 ? parent.left : lstimg.right
                anchors.leftMargin: 10
                font.pixelSize: parent.height * 0.5
                font.bold: true
                color: "black"
            }

            Rectangle {
                height: 1
                width: parent.width
                color: "lightgray"
                anchors.bottom: parent.bottom
            }

            Item {
                id: moreitem
                visible: moretype==0 ? false : true
                height: parent.height

                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: moretext
                    visible: moretype==1 ? true : false
                    text: more
                    font.pixelSize: parent.height * 0.4
                    font.bold: true
                    color: "lightgray"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Image {
                    id: moreimg
                    visible: moretype==2 ? true : false
                    source: moretype==2 ? more : ""
                    height: parent.height * 0.6
                    width: height
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                // TODO
                Rectangle {
                    visible: flag==1 ? true : false
                    anchors.top: moreimg.top
                    anchors.topMargin: -height/2
                    anchors.right: moreimg.right
                    anchors.rightMargin: -width/2
                    width: 10
                    height: 10
                    color: "red"
                    radius: 5
                }
            }

            signal clicked
            onClicked: {
                //callback()
                console.debug(callback.toString(), typeof(callback), typeof(icon), typeof(flag))

                var f = callback
                //f()
                //var func = function(callback)
                Function.call(callback)
            }

            MouseArea {
                id: mouse
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: type==0 ? parent.top : parent.bottom
                onClicked: {
                    lstitem.clicked()
                }
            }
        }
    }

    ListView {
        id: lstview
        anchors.fill: parent
        model: lstmodel
        delegate: lstdelegate
    }

    function addItem(type_, icon_, str_, w_, moretype_, more_, flag_, callback_)
    {
        lstmodel.append({
            "type": type_,
            "icon": icon_,
            "str": str_,
            "w": w_,
            "moretype": moretype_,
            "more": more_,
            "flag": flag_,
            "callback": callback_
        })
    }

    function defaultCallback()
    {
        console.debug("CommonListView::defaultCallback")
    }

    function addElement(icon_, str_, w_, callback_)
    {
        //addItem(1, icon_, str_, w_, 0, "", 0, callback_)
        addItem(1, icon_, str_, w_, 0, "", 0, defaultCallback )
    }

    function addElementWithTailStr(icon_, str_, w_, more_)
    {
        addItem(1, icon_, str_, w_, 1, more_, 0, defaultCallback)
    }

    function addElementWithTailImg(type_, icon_, str_, w_, more_)
    {
        addItem(1, icon_, str_, w_, 2, more_, 0, defaultCallback)
    }

    function addSpliteElement(str_, w_)
    {
        addItem(0, "", str_, w_, 0, "", 0, defaultCallback)
    }
}
