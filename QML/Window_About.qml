import QtQuick 2.0
import QtQuick.Controls 2.13

import "about"
import "components"

Dialog {
    id: w_about

    //width: parent.width * 2/3
    //height: parent.height * 2/3
    width: parent.width
    height: parent.height

    anchors.centerIn: Overlay.overlay

    modal: true
    focus: true

    title: qsTr("About")

    MDrawer {
        id: mdrawer

        model: [
            /*"Copyright",*/
            qsTr("Licenses"),
            qsTr("Libraries")
        ]
    }

    Item {
        id: m_main

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: r_actions.top
        anchors.left: mdrawer.right

        anchors.leftMargin: 20

        clip: true
/*
        About_Copyright {
            anchors.fill: parent
            visible: mdrawer.currentIndex === 0
        }
*/
        About_License {
            anchors.fill: parent
            visible: mdrawer.currentIndex === 0
        }

        About_Librairies {
            anchors.fill: parent
            visible: mdrawer.currentIndex === 1
        }
    }

    Row {
        id: r_actions

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        spacing: 10

        Button {
            id: btn_close

            width: 80
            height: 25

            text: qsTr("Close")

            onClicked: w_about.close()
        }
    }
}
