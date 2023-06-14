import QtQuick 2.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    property string sH3Text:  "H3 title"

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // QML
    //==============================================================
    implicitWidth: 542
    implicitHeight: 34

    anchors.left: parent.left
    anchors.right: parent.right
    //width: parent.width
    height: 34
    Rectangle {
        anchors.fill:parent
        color: "#e4e3e9"
        transformOrigin: Item.Center
        clip: false
        Text {
            text: qsTr(sH3Text)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.weight: Font.DemiBold
            font.family: "Open Sans"
            font.bold: false
            font.pixelSize: 13
        }
    }
}
