import QtQuick 2.0
import QtQuick.Controls 2.12

Button {
    clip: true

    property alias _text: l_text.text

    width: 128 * 9/10
    height: 128 * 9/10

    Label {
        id: l_text
        anchors.fill: parent
        anchors.margins: {
            left: 5
            right: 5
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        fontSizeMode: Text.HorizontalFit
        minimumPixelSize: 20
        font.pixelSize: 72
        font.bold: true
        wrapMode: "WordWrap"
        elide: "ElideRight"
    }
}
