import QtQuick 2.0
import QtQuick.Controls 2.13

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
    property string sText1Input: "0"
    property string sText2Input: "0"
    property string sTooltipText1: "Tooltip texte"
    property string sTooltipText2: "Tooltip texte"

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
            id: rectangleSN4
            color: "#00ffffff"
            height: parent.height
            width: ( parent.width ) / 3

            TextField {
                id: inputSN_Y
                width: parent.width * 1 / 3 - 10 - txtSNSpace.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                //anchors.right: txtSNSpace.left
                //anchors.rightMargin: 0
                text: qsTr(sText1Input)
                height: 30
                activeFocusOnPress: true
                selectByMouse: true
                anchors.verticalCenter: parent.verticalCenter

                ToolTip {
                    id: tooltipSN_Y
                    visible: false
                    text: qsTr(sTooltipText1)
                    font.weight: Font.Light
                    font.family: "Open Sans"
                    opacity: 0.95
                    enter: Transition {
                        NumberAnimation { property: "opacity"; from: 0.0; to: 1 }
                    }
                    exit: Transition {
                        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                    }
                    background: Rectangle{
                        border.color: "#555555"
                        color: "white"
                    }
                }
            }
            Text {
                id: txtSNSpace
                text: qsTr(" - ")
                anchors.left: inputSN_Y.right
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
                font.family: "open sans"
                font.pixelSize: 12
                font.weight: Font.DemiBold
            }
            TextField {
                id: inputSN_N
                width: parent.width * 2 / 3 - 10
                text: qsTr(sText2Input)
                renderType: Text.QtRendering
                anchors.left: txtSNSpace.right
                anchors.leftMargin: 0
                //anchors.right: parent.right
                //anchors.rightMargin: 10
                height: 30
                activeFocusOnPress: true
                selectByMouse: true
                anchors.verticalCenter: parent.verticalCenter

                ToolTip {
                    id: tooltipSN_N
                    visible: false
                    text: qsTr(sTooltipText2)
                    font.weight: Font.Light
                    font.family: "Open Sans"
                    opacity: 0.95
                    enter: Transition {
                        NumberAnimation { property: "opacity"; from: 0.0; to: 1 }
                    }
                    exit: Transition {
                        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                    }
                    background: Rectangle{
                        border.color: "#555555"
                        color: "white"
                    }
                }
            }
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
