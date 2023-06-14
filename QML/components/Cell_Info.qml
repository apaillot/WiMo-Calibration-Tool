import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    clip: true

    property alias title: l_title.text
    property alias subtitle: l_subtitle.text
    property var bool: false

    width: 128 * 9/10
    height: 128 * 9/10
    radius: 13

    //color: bool ? m_theme.accent : m_theme.lighterGray

    Label {
        id: l_title

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        font.capitalization: Font.AllUppercase
        font.pixelSize: 11
    }

    Label {
        id: l_subtitle
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
