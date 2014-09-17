import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    id: inputarea
    anchors.fill: parent
    color: "white"
    border.color: "lightgray"
    border.width: 1

    property string defaultText: ""
    property int maxLength: 30

    Component {
        id: touchStyle
        TextFieldStyle {
            textColor: "black"
            font.pixelSize: inputarea.height * 0.5
            background: Item {}
        }
    }

    TextField {
        id: searchtext
        anchors.fill: parent
        style: touchStyle
        placeholderText: parent.defaultText
        maximumLength: parent.maxLength
    }

    function getEditText()
    {
        return searchtext.text
    }
}
