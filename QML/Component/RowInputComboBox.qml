import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQml.Models 2.15

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
    property string sTextInput: "COM1"
    property string sTooltipText: "Tooltip texte"

    property var tCombomodel: ListModel{}

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
            id: rectangle
            color: "#00ffffff"
            height: parent.height
            width: ( parent.width ) / 3
            ComboBox {
                id: selectCalibType
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                // Si activation
                //enabled: !bNkeProdMode
                //model: listCalibTypeGeneral
                //model: testModel
                //model: combomodel.comboList
                model: tCombomodel

                //textRole: "text"
                // Tooltip calib type
                ToolTip {
                    id: tooltipCalibType
                    visible: false
                    text: qsTr("InvalidValue")
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
