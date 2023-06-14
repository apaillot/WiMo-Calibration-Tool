import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQml.Models 2.15
import QtGraphicalEffects 1.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================
    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Parameter
    property var m_tChannelList: ListModel{}
    property int m_uiCurrentChannel: 0
    property bool m_bSelectSensorCalibration: true

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal currentChannelChanged( int uiIndex )

    //==============================================================
    // QML
    //==============================================================
    id: m_calibrationSelectionBlocID
    implicitWidth: 542
    implicitHeight: 238

    height: 74

    //-----------------------------------------------
    // Premier bloc : channel selection
    //-----------------------------------------------
    DropShadow {
        color: "#20000000"
        radius: 12
        verticalOffset: 0
        anchors.fill: m_calibSensorChoice
        samples: 30
        horizontalOffset: 0
        source: m_calibSensorChoice
    }
    // Calibration settings
    Rectangle {
        id: m_calibSensorChoice
        color: "#ffffff"
        anchors.fill: parent

        H3BlocTitle{
            sH3Text: "Channel selection"
        }
        //------------------------------
        // Channel Selection
        //------------------------------
        Row {
            id: rowSensor
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            anchors.bottom: parent.bottom

            Rectangle {
                id: rowSensor2
                width: ( parent.width ) / 3
                height: parent.height

                Image {
                    id: m_imgSelID
                    width: 30
                    height: 20
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/Logo/Calibration/calib-sensor.png"
                }

                Text {
                    text: qsTr("Channel")
                    height: parent.height
                    anchors.left: m_imgSelID.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.family: "open sans"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }
            ComboBox {
                id: selectSensorCalibration
                width: ( parent.width ) / 3 - 20
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                currentIndex: m_uiCurrentChannel

                model: m_calibrationSelectionBlocID.m_tChannelList
                // Si activation
                enabled: m_bSelectSensorCalibration

                //%%AP - 2020.08.24 - Correction de problème d'affichage et de sélection
                //%%AP - de la ComboBox de voie d'acquisition
                //onCurrentIndexChanged:{
                onActivated:{
                    console.log("currentIndex: "+currentIndex);
                    m_calibrationSelectionBlocID.currentChannelChanged(currentIndex)
                }
            }
            Rectangle {
                id: rectangleCOMDummy
                color: "#00ffffff"
                height: parent.height
                width: ( parent.width ) / 3
            }
        }

    }
}
