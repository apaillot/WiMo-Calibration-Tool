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
    property string sTextParam: "COM1"
    property string sGreyButtonText: ""
    property int uiBtnGreyWidth: 120

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

    //==============================================================
    // QML
    //==============================================================
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
            Text {
                id: txtCOMEditable
                text: sTextParam
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height
                //anchors.left: imgCOM.right
                //anchors.leftMargin: 10
                //anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "open sans"
                font.pixelSize: 12
                font.weight: Font.Normal
            }
        }
        // 3 ----
        Rectangle {
            color: "#ffffff"
            height: parent.height
            //width: ( parent.width - 15 ) / 3
            width: ( parent.width ) / 3

            ButtonPerso {
                id: btnChangePortCom
                text: qsTr(sGreyButtonText)
                height: 28
                width: uiBtnGreyWidth
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: true
                //running: false
                onClicked: {
                }
            }
            BusyIndicatorPerso {
                id: busyChangePortCom
                width: 36
                height: 37
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: false
            }
        }
    }
}
