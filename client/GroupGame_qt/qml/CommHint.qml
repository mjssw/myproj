import QtQuick 2.0

Rectangle {
    id: commhint
    anchors.fill: parent
    color: "black"
    radius: 8
    opacity: 0.7
    visible: false

    property string hintText: ""
    property int showtime: 1300

    Text {
        id: hinttext
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: parent.height * 0.5
        text: parent.hintText
    }

    Timer {
        id: showtimer
        interval: parent.showtime
        repeat: false

        onTriggered: {
            commhint.visible = false
        }
    }

    function showHint(text)
    {
        hintText = text
        commhint.visible = true
        showtimer.start()
    }
}
