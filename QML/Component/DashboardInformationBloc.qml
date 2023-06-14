import QtQuick 2.0
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
//import "Component"

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Parameter information
    property string m_sIdentifier: ""
    property string m_sChannels: ""
    property string m_sManufacturer: ""
    property string m_sSerialNumber: ""
    property string m_sFirmwareVersion: ""

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // QML
    //==============================================================
    id: m_dashboardInformationBlocID
    implicitWidth: 542
    implicitHeight: 238
    anchors.horizontalCenter: parent.horizontalCenter

    height: 238

    //-------------------------------------------
    // Bloc Dashboard - Information
    //-------------------------------------------
    // Bloc Dashboard - Information
    DropShadow {
        color: "#20000000"
        radius: 12
        verticalOffset: 0
        anchors.fill: m_dashboardInfoID
        samples: 30
        horizontalOffset: 0
        source: m_dashboardInfoID
        //visible: ( bGenericSensor || bWiper || bO2 )
    }
    Rectangle {
        id: m_dashboardInfoID
        color: "#ffffff"
        anchors.fill: parent

        H3BlocTitle{
            id: m_DashboardInfoTitleBlocID
            sH3Text: "Information"
        }

        Column{
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top: m_DashboardInfoTitleBlocID.bottom
            anchors.bottom: parent.bottom
            //---------------
            // Identifier
            //---------------
            RowText{
                sTextDesc: "Identifier"
                sLogo: "qrc:/Logo/Dashboard/dashboard-ID.png"
                sTextParam: m_sIdentifier
                uiLogoHeight: 25
            }
            SectionBlocLine{}
            //---------------
            // Channel
            //---------------
            RowText{
                sTextDesc: "Channels"
                sLogo: "qrc:/Logo/Dashboard/dashboard-version.png"
                sTextParam: m_sChannels
                uiLogoHeight: 22
            }
            SectionBlocLine{}
            //---------------
            // Manufacturer
            //---------------
            RowText{
                sTextDesc: "Manufacturer"
                sLogo: "qrc:/Logo/Dashboard/dashboard-manufacturer.png"
                sTextParam: m_sManufacturer
                uiLogoHeight: 20
            }
            SectionBlocLine{}
            //---------------
            // Serial number
            //---------------
            RowText{
                sTextDesc: "Serial number"
                sLogo: "qrc:/Logo/Dashboard/dashboard-SN2.png"
                sTextParam: m_sSerialNumber
                uiLogoHeight: 20
            }
            SectionBlocLine{}
            //---------------
            // Firmware version
            //---------------
            RowText{
                sTextDesc: "Firmware version"
                sLogo: "qrc:/Logo/Dashboard/dashboard-firmware.png"
                sTextParam: m_sFirmwareVersion
                uiLogoHeight: 25
            }
            //SectionBlocLine{}
        }
    }
}
