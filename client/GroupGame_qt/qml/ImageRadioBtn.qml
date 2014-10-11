import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

RadioButton {
    id: btn
    //anchors.fill: parent
    property string btnSource:""
    style: RadioButtonStyle {
        panel: Item {
            BorderImage {
                id: btnimage
                anchors.top : parent.top
                anchors.topMargin: btn.height / 10
                anchors.horizontalCenter: parent.horizontalCenter
                height: btn.height * 0.6
                width: height * sourceSize.width / sourceSize.height

                source: btn.btnSource
                Text {
                    id: btntext
                    text: btn.text
                    anchors.top: parent.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: control.checked ? "#007AFF" : "#929292"
                    font.pixelSize: parent.height * 0.4
                    //renderType: Text.NativeRendering
                }
            }
        }
    }
}
