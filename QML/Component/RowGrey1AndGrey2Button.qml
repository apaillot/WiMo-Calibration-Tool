import QtQuick 2.0
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
import "Component"

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
    property string sGrey1ButtonText: ""
    property int uiBtnGrey1Width: 120
    property string sGrey2ButtonText: ""
    property int uiBtnGrey2Width: 120

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
    signal button1Clicked()
    signal button2Clicked()

    //==============================================================
    // QML
    //==============================================================
    id: m_rowGrey1AndGrey2Button
    implicitWidth: 542
    implicitHeight: 40

    height: implicitHeight
    anchors.left: parent.left
    anchors.right: parent.right

    Row{
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        //anchors.top: parent.top
        //anchors.bottom: parent.bottom
        //anch
        spacing: 0

        // 1 ----
        Rectangle {
            //width: ( parent.width - 15 ) / 3
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
            //width: ( parent.width - 15 ) / 3
            width: ( parent.width ) / 3

            ButtonPerso {
                text: qsTr(sGrey1ButtonText)
                height: 28
                width: uiBtnGrey1Width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: true
                //running: false
                onClicked: {
                    m_rowGrey1AndGrey2Button.button1Clicked()
                }
            }
            BusyIndicatorPerso {
                width: 36
                height: 37
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: false
            }
        }
        // 3 ----
        Rectangle {
            color: "#ffffff"
            height: parent.height
            //width: ( parent.width - 15 ) / 3
            width: ( parent.width ) / 3

            ButtonPerso {
                text: qsTr(sGrey2ButtonText)
                height: 28
                width: uiBtnGrey2Width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: true
                //running: false
                onClicked: {
                    m_rowGrey1AndGrey2Button.button2Clicked()
                }
            }
            BusyIndicatorPerso {
                width: 36
                height: 37
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: false
            }
        }
    }
}
