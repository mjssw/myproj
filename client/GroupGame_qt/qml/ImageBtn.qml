import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
    id: btn
    property string btnSource:""
    style: ButtonStyle {
        panel: Item {
            BorderImage {
                id: btnimage
                anchors.top : parent.top;
                anchors.topMargin: btn.anchors.topMargin;
                anchors.horizontalCenter: parent.horizontalCenter;
                source: btn.btnSource
                Text {
                    id: btntext
                    text: btn.text
                    anchors.top: parent.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    color: control.pressed ? "white" : "#929292"
                    font.pixelSize: 20
                    renderType: Text.NativeRendering
                }
            }
        }
    }
}
