import QtQuick 2.0

Item{
    id:blueButton
    property string text: "default"
    property bool bHover:  false
    width: 100
    height: 28
    visible: true
    signal buttonClicked

    Rectangle {
        color: (bHover)?"#FF32549C":"#FF193d8a"
        radius: 7
        transformOrigin: Item.Center
        anchors.fill: parent
        visible: parent.visible

        Text {
            x: 28
            y: 17
            color: "white"
            text: blueButton.text
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Open Sans"
            font.pixelSize: 11
            visible: parent.visible
        }

    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            parent.buttonClicked();
        }
        onEntered: {
            bHover = true;
        }
        onExited:  {
            bHover = false;
        }
    }
}
