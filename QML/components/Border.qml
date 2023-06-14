import QtQuick 2.0

Item {
    id: border
    anchors.fill: parent

    property bool all: false

    property bool _top: false
    property bool _right: false
    property bool _bottom: false
    property bool _left: false

    property int size: 1

    //property var color: m_theme.color_border
    property var color: "#dddddd"

    Rectangle {
        id: border_top
        anchors.top: parent.top
        width: parent.width
        height: border.all || border._top ? border.size : 0
        color: border.color
    }

    Rectangle {
        id: border_right
        anchors.right: parent.right
        width: border.all || border._right ? border.size : 0
        height: parent.height
        color: border.color
    }

    Rectangle {
        id: border_bottom
        anchors.bottom: parent.bottom
        width: parent.width
        height: border.all || border._bottom ? border.size : 0
        color: border.color
    }

    Rectangle {
        id: border_left
        anchors.left: parent.left
        width: border.all || border._left ? border.size : 0
        height: parent.height
        color: border.color
    }
}
