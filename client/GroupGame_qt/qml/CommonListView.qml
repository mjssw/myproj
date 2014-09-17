import QtQuick 2.0

Item {
    id: commlist
    anchors.fill: parent

    property string listname: "mylistname"

    ListModel {
        id: lstmodel
    }

    Component {
        id: lstdelegate

        Rectangle {
            id: lstitem
            width: commlist.width
            height: w
            color: type==0 ? "lightgray" : "white"

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
                font.pixelSize: parent.height * 0.4
                font.weight: Font.DemiBold
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
                    font.weight: Font.DemiBold
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

            Rectangle {
                anchors.fill: parent
                color: "#7A7A7A"
                visible: mouse.pressed
                opacity: 0.7
            }

            signal clicked
            onClicked: {
                clickElement(clickkey)
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

    function clearAll()
    {
        lstmodel.clear()
    }

    function addItem(type_, icon_, str_, w_, moretype_, more_, flag_, key_)
    {
        lstmodel.append({
            "type": type_,
            "icon": icon_,
            "str": str_,
            "w": w_,
            "moretype": moretype_,
            "more": more_,
            "flag": flag_,
            "clickkey": key_
        })
    }

    function addElement(icon_, str_, w_, key_)
    {
        addItem(1, icon_, str_, w_, 0, "", 0, key_)
    }

    function addElementWithTailStr(icon_, str_, w_, more_, key_)
    {
        addItem(1, icon_, str_, w_, 1, more_, 0, key_)
    }

    function addElementWithTailImg(type_, icon_, str_, w_, more_, key_)
    {
        addItem(1, icon_, str_, w_, 2, more_, 0, key_)
    }

    function addSpliteElement(str_, w_)
    {
        addItem(0, "", str_, w_, 0, "", 0, 0)
    }
}
