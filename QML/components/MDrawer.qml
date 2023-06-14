import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {

    property alias model: listView.model
    property alias currentIndex: listView.currentIndex

    property color tLightGray: "#D3D3D3"
    property color tLighterGray: "#E3E3E3"

    width: 200

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.left: parent.left

    color: tLighterGray

    ListView {
        id: listView

        highlightFollowsCurrentItem: false
        anchors.fill: parent

        highlight: Rectangle {
            x: 0
            y: listView.currentItem.y
            width: 5
            height: listView.currentItem.height
            /*
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: m_theme.accent
                }

                GradientStop {
                    position: 1
                    color: m_theme.accent
                }
            }
            */

            Behavior on y { SmoothedAnimation { velocity: 400 } }
        }

        delegate: ItemDelegate {
            id: itemDelegate
            width: parent.width
            height: 48

            onClicked: listView.currentIndex = index;

            Label {
                text: modelData
                font.bold: listView.currentIndex == index
                leftPadding: 16
                anchors.verticalCenter: parent.verticalCenter
                //color: listView.currentIndex == index ? m_theme.accent : m_theme.dark
            }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }
}
