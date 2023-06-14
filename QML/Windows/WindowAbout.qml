import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Window {
    id: window

    width: 820
    height: 560

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width
    color: "#efeef4"
    title: qsTr("About")
    visible: false

    Text {
        id: element
        x: 22
        y: 18
        width: 61
        height: 25
        text: qsTr("About")
        font.weight: Font.DemiBold
        font.family: "Open Sans"
        font.pixelSize: 17
    }


    Rectangle {
        id: rectangle
        x: 22
        y: 54
        width: 195
        height: 484
        color: "#132644"
        RowLayout {
            id: rowLayout
            width: 177
            height: 473
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

}
