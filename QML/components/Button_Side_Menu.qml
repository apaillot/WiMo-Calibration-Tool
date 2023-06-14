import QtQuick 2.0
import QtQuick.Controls 2.12

ItemDelegate {
    id: btn_side_menu

    property alias rotation: l_text.rotation
    property bool _right: false

    width: parent.width
    height: 70

    checkable: true

    background: Rectangle {
        anchors.fill: parent
        //color: parent.highlighted ? m_theme.accent : m_theme.color_bg

        Border {
            id: b_btn_lb
            all: true
        }
    }

    Label {
        id: l_text
        anchors.fill: parent
        text: modelData
        font.pointSize: 10
        font.bold: parent.highlighted

        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignHCenter"
    }

    states: [
        State {
            name: "hovered"
            when: hovered
            PropertyChanges {
                target: btn_side_menu
                x: _right ? -height : 0
                width: parent.width + height
            }

            PropertyChanges {
                target: l_text
                rotation: 0
                font.pointSize: 13
            }

            PropertyChanges {
                target: b_btn_lb
                all: true
            }
        }
    ]

    transitions: Transition {
        NumberAnimation {
            properties: "x, width, rotation"
            easing.type: Easing.InOutQuint
            duration: 100
        }
    }
}
