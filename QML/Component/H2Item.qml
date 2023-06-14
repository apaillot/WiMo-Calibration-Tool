import QtQuick 2.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    property string sH2Title: ""
    property string sH2Img: ""
    property bool bHideTriangle: false
    property int uiLogoWidth: 28

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // QML
    //==============================================================
    implicitWidth: 590
    //implicitHeight: 108
    implicitHeight: 88
    z: -1

    Rectangle {
        id: rectangle
        y: 0
        anchors.fill: parent
        color: "#00efeef4"
        z: -1
        Rectangle {
            id: triangleTitle
            x: 92
            y: 74
            width: 28
            height: 28
            color: "#e4e3e9"
            rotation: 45
            visible: !(bHideTriangle)
        }
        Rectangle {
            id: rectangle1
            x: 72
            y: 0
            width: 68
            height: 68
            color: "#ffffff"
            radius: 31

            Image {
                id: image
                x: 22
                y: 19
                width:uiLogoWidth
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectFit
                source: sH2Img
            }
        }
        Text {
            id: dashboardTitle
            x: 151
            y: 22
            width: 404
            height: 27
            text: qsTr(sH2Title)
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 20
            font.weight: Font.DemiBold
            //font.family: "Open Sans"
            font.family: openSansFont.name
        }
    }
}
