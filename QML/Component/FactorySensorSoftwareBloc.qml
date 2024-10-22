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
    // Update
    property int  m_uiUpdateProgressBar:  0
    property bool m_bBusyRefresh: false
    property bool m_bBtnRefreshVisible: true
    // Reset
    property int  m_uiResetProgressBar: 0
    property bool m_bResetBusyRefresh: false
    property bool m_bResetBusyRefresh: true

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal checkUpdateButtonClicked()
    signal openFileComButtonClicked()
    signal triggerResetButtonClicked()

    //==============================================================
    // QML
    //==============================================================
    id: m_factorySensorSoftwareBlocID

    implicitWidth: 542
    implicitHeight: 34+40*2+1*1
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
                sH3Text: "WiMo sensor software"
            }

            //---------------
            // Check for update et open file button
            //---------------
            RowBlueAndGreyButton{
                sTextDesc: "Update"
                sLogo: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                uiLogoHeight: 18
                sBlueButtonText: "Check for update"
                sGreyButtonText: "Open file..."
                uiBtnBlueWidth: 130
                m_uiUpdateProgressBar:  m_factorySensorSoftwareBlocID.m_uiUpdateProgressBar
                m_bBusyRefresh:         m_factorySensorSoftwareBlocID.m_bBusyRefresh
                m_bBtnRefreshVisible:   m_factorySensorSoftwareBlocID.m_bBtnRefreshVisible
                onBlueButtonClicked:{ m_factorySensorSoftwareBlocID.checkUpdateButtonClicked() }
                onGreyButtonClicked:{ m_factorySensorSoftwareBlocID.openFileComButtonClicked() }
            }
            SectionBlocLine{}
            //---------------
            // Reset inter
            //---------------
            //RowBlueUpdateButton{
            RowBlueButton{
                sTextDesc: "Reset"
                sLogo: "qrc:/Logo/Factory/factory-reset.png"
                uiLogoHeight: 20
                sBlueButtonText: "Trigger"
                uiBtnBlueWidth: 130
                //m_uiUpdateProgressBar: m_factorySensorSoftwareBlocID.m_uiResetProgressBar
                //m_bBusyRefresh:        m_factorySensorSoftwareBlocID.m_bResetBusyRefresh
                //m_bBtnRefreshVisible:  m_factorySensorSoftwareBlocID.m_bResetBusyRefresh
                onBlueButtonClicked:{ m_factorySensorSoftwareBlocID.triggerResetButtonClicked() }
            }
        }
    }
}
