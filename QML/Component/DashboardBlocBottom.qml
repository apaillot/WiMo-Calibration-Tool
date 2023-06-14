import QtQuick 2.0
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    property string sText1: "Licence"
    property string sText2: "details"
    property string sButtonText: "Open"
    property string sLogo: "qrc:/Logo/BuiltwithQtlogos/built-with-Qt_40px_606060.png"
    property int uiLogoWidth: 40

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal buttonClicked()

    //==============================================================
    // QML
    //==============================================================
    width: 150
    height: 100

    //------------------
    // Licence info
    //------------------
    DropShadow {
        color: "#20000000"
        radius: 12
        verticalOffset: 0
        anchors.fill: m_dashboardBlocBottomID
        samples: 30
        horizontalOffset: 0
        source: m_dashboardBlocBottomID
    }
    Rectangle {
        id:m_dashboardBlocBottomID
        anchors.fill:parent
        color: "#ffffff"

        // Logo Qt
        Image {
            id: imgLogoQt
            width: 40

            visible: true
            anchors.left: parent.left
            anchors.top: parent.top
            fillMode: Image.PreserveAspectFit
            source: sLogo
            anchors.topMargin: 12
            anchors.leftMargin: 15
        }
        //
        Text {
            id: txtSoftWareVersion
            color: "#606060"
            text: sText1
            anchors.left: imgLogoQt.right
            //anchors.bottom: parent.bottom
            anchors.top: imgLogoQt.top
            anchors.topMargin: 3
            horizontalAlignment: Text.AlignLeft
            font.bold: false
            font.family: openSansLightFont.name
            anchors.leftMargin: 10
            font.weight: Font.Normal
            font.pixelSize: 17
        }

        Text {
            id: txtSoftWareVersion1
            color: "#606060"
            text: sText2
            anchors.left: txtSoftWareVersion.left
            anchors.top: txtSoftWareVersion.bottom
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
            font.weight: Font.Normal
            font.family: openSansLightFont.name
            anchors.leftMargin: 0
            anchors.topMargin: -1
            font.bold: false
        }

        Button {
            id: button
            x: 15
            y: 68
            width: parent.width - 30
            height: 27
            text: qsTr(sButtonText)
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            onClicked: {
                //w_about.open()
                buttonClicked();
            }
        }
    }
}
