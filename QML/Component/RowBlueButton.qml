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
    property string sTextDesc: "Serial port"
    property string sLogo: "qrc:/Logo/Dashboard/serialCOM-low.png"
    property int uiLogoHeight: 20
    property string sBlueButtonText: ""
    property int uiBtnBlueWidth: 100

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Fonction
    //==============================================================
    // Retourne la valeur du input
    function sGetInputValue(){return( m_inputValue );}

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal blueButtonClicked()

    //==============================================================
    // QML
    //==============================================================
    id: m_rowBlueButton
    implicitWidth: 542
    implicitHeight: 40

    height: implicitHeight
    anchors.left: parent.left
    anchors.right: parent.right

    Row{
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        spacing: 0

        // 1 ----
        Rectangle {
            width: ( parent.width ) / 3
            height: parent.height
            color: "#ffffff"

            Image {
                id: imgCOM
                width: 30
                height: uiLogoHeight
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectFit
                source: sLogo
            }

            Text {
                text: qsTr(sTextDesc)
                height: parent.height
                anchors.left: imgCOM.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "open sans"
                font.pixelSize: 12
                font.weight: Font.DemiBold
            }
        }
        // 2 ----
        Rectangle {
            color: "#ffffff"
            height: parent.height
            width: ( parent.width ) / 3

            DropShadow {
                color: "#20000000"
                radius: 10
                verticalOffset: 0
                anchors.fill:btnRefreshButton
                samples: 25
                horizontalOffset: 0
                source: btnRefreshButton
            }
            BlueButtonPerso {
                id: btnRefreshButton
                text: qsTr(sBlueButtonText)
                width: uiBtnBlueWidth
                height: 28
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                visible: true
                onButtonClicked: {
                    m_rowBlueButton.blueButtonClicked()
                }
            }
            BusyIndicatorPerso {
                id: busyRefresh
                width: 36
                height: 37
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter:   parent.verticalCenter
                visible: false
            }
            ////////////////////////////////
        }
        // 3 ----
        Rectangle {
            id: rectangleCOMDummy
            color: "#00ffffff"
            height: parent.height
            width: ( parent.width ) / 3
        }
    }
}
