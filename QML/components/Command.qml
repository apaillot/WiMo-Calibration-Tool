import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

Item {
    id: m_main
    clip: true

    property alias model: m_content.model
    property alias title: m_header.text

    property bool risky: false

    signal clicked

    height: accordion_item.height + border_bottom.height

    enabled: iutManager.running

    Item {
        id: accordion_item
        height: m_header.height + m_content.height

        anchors.right: parent.right
        anchors.left: parent.left

        anchors.rightMargin: 20
        anchors.leftMargin: 20

        ItemDelegate {
            id: m_header

            height: 48

            anchors.top: parent.top
            anchors.right: btn_send.left
            anchors.left: parent.left

            checkable: true
            checked: false

            //Universal.accent: risky ? "red" : m_theme.accent
            Universal.accent: risky ? "red" : "transparent"
        }

        Button {
            id: btn_send

            width: m_header.height *2
            height: m_header.height

            anchors.right: parent.right

            enabled: m_content.contentHeight === 0 || m_header.highlighted
            text: "SEND"
            font.bold: true

            onClicked: m_main.clicked()
        }

        ListView {
            id: m_content
            clip: true

            anchors.top: m_header.bottom
            anchors.right: parent.right
            anchors.left: parent.left
        }
    }

    MenuSeparator {
        id: border_bottom

        anchors.top: accordion_item.bottom
        anchors.right: parent.right
        anchors.left: parent.left
    }

    states: [
        State {
            name: "opened"
            when: m_header.checked

            PropertyChanges {
                target: m_content;
                height: m_content.contentHeight;
            }

            PropertyChanges {
                target: m_header
                highlighted: m_content.contentHeight !== 0
            }
        }
    ]

    transitions: Transition {
        to: "opened"
        reversible: true

        SequentialAnimation {
            PropertyAnimation { property: "height"; duration: 100 }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
