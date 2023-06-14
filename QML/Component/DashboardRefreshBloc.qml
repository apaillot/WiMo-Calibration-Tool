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
    // Parameter information
    property string m_sCOMName:  "COM1"
    property string m_sCOMState: "open"
    property bool   m_bBusyRefresh: false

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal refreshButtonClicked()
    signal changeComButtonClicked()

    //==============================================================
    // QML
    //==============================================================
    id: m_dashboardRefershBlocID
    implicitWidth: 542
    implicitHeight: 115
    anchors.horizontalCenter: parent.horizontalCenter

    height: implicitHeight

    //--------------------------------------------------
    // Bloc : dashboard refresh
    //--------------------------------------------------
    // Bloc Dashboard - refresh
    DropShadow {
        color: "#20000000"
        radius: 12
        verticalOffset: 0
        anchors.fill: m_DashboardRefreshID
        samples: 30
        horizontalOffset: 0
        source: m_DashboardRefreshID
    }
    Rectangle {
        id: m_DashboardRefreshID
        color: "#ffffff"
        anchors.fill: parent

        Column{
            anchors.fill: parent

            H3BlocTitle{
                sH3Text: "Sensor detection"
            }

            //---------------
            // Serial port
            //---------------
            RowText{
                sTextDesc: "Serial port"
                sLogo: "qrc:/Logo/Dashboard/serialCOM-low.png"
                sTextParam: m_sCOMName +": " +m_sCOMState
                uiLogoHeight: 20
            }
            SectionBlocLine{}
            //---------------
            // Refresh et Change interface button
            //---------------
            //DashboardRefreshButtonRow{}
            RowBlueAndGreyButton{
                sTextDesc: "Bus connexion"
                sLogo: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                uiLogoHeight: 20
                sBlueButtonText: "Refresh"
                sGreyButtonText: "Change interface..."
                m_bBusyRefresh: m_dashboardRefershBlocID.m_bBusyRefresh
                m_bBtnRefreshVisible: (m_dashboardRefershBlocID.m_sCOMState=="Open") && !m_dashboardRefershBlocID.m_bBusyRefresh
                onBlueButtonClicked:{ m_dashboardRefershBlocID.refreshButtonClicked() }
                onGreyButtonClicked:{ m_dashboardRefershBlocID.changeComButtonClicked() }
            }
        }
    }
}
