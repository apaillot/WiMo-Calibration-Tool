import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "Component"
import "Windows"

Item {
    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Parameter COM
    property string m_sComPort:      tDataCom.sComPort
    property string m_sComPortState: tDataCom.sComPortState
    // Parameter information
    property string m_sIdentifier:      tDataSensor.sIdentifier
    //property string m_sChannels:        tDataSensor.sChannels
    property string m_sChannels:        tDataSensorCh0.sName + ((tDataSensor.uiChannelNb>1)?" - "+tDataSensorCh1.sName:"")+((tDataSensor.uiChannelNb>2)?" - "+tDataSensorCh2.sName:"")
    property string m_sManufacturer:    tDataSensor.sManufacturer
    property string m_sSerialNumber:    tDataSensor.sSN
    property string m_sFirmwareVersion: tDataSensor.sFirmwareVersion

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property int uiBlocWidthOffset: 60
    // Etat refresh
    property bool m_bBusyRefresh: tDashboardControler.m_bBusyRefresh
    //property bool m_bErrorRefresh: false

    //==============================================================
    // Déclaration des fonctions
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // QML
    //==============================================================
    id: dashBoardPartID
    width: window.width - 228
    height: (window.height<=600)?600:window.height
    implicitWidth: 590
    implicitHeight: 780

    // Background color
    Rectangle {
        anchors.fill: parent
        color: "#efeef4"
        z: -9
    }
    // Image interface usb d'en bas
    Image {
        id: image1
        x: 30
        y: 20
        width: 800
        height: 149
        z: -2
        rotation: 0
        fillMode: Image.PreserveAspectFit
        source: "qrc:/Logo/Dashboard/Interface-PC-Modbus-Vertical.png"
        opacity: 0.06
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    //-------------------------------
    // Titre H2
    //-------------------------------
    H2Item{
        id: m_H2ItemID
        sH2Title: "Dashboard"
        sH2Img: "qrc:/Logo/Dashboard/dashboard-dashboard.png"
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    //--------------------------------------------------
    // Bloc : dashboard refresh
    //--------------------------------------------------
    DashboardRefreshBloc{
        id: m_dashboardRefreshBlocID
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.leftMargin: uiBlocWidthOffset
        anchors.rightMargin: uiBlocWidthOffset
        anchors.top: m_H2ItemID.bottom
        // Property
        m_sCOMName:  dashBoardPartID.m_sComPort
        m_sCOMState: dashBoardPartID.m_sComPortState
        m_bBusyRefresh: dashBoardPartID.m_bBusyRefresh
        // Signaux
        onRefreshButtonClicked:{
            //-------------
            // %%TODO - Utilisation Controleur
            //-------------
            //tControlerDashboard.refreshSensor()
            //-------------
            console.log("Clicked" )
            // Reinit
            tMainData.bError = false;
            tMainData.sDebug = "btnRefreshButton clicked";
            // Réinit de l'état
            vFMAIN_ClearData()
            tMainData.sDebug = "vFMAIN_ClearData end";
            //
            //busyRefresh.visible = true
            //btnRefreshButton.visible = false
            //dashBoardPartID.m_bBusyRefresh = true
            // Clear dashboard
            vFLockUserClick();
            // Lancement
            timerRefreshSensor.start();
            tMainData.sDebug = "timerRefreshSensor.start end";
        }
        onChangeComButtonClicked:{
            //windowCOMAuto.visible=true
            // Ouverture de la fenêtre de changement de port COM
            tCOMEvent.vFComPortFindAutomaticSelection(false)
        }
    }

    // Timer
    Timer {
        id: timerRefreshSensor
        interval: 100;
        running: false;
        repeat: false;
        onTriggered: {
            tMainData.sDebug = "Test port COM toujours ouvert";

            tMainData.sDebug = "Test port COM toujours ouvert end";

            tMainData.sDebug = "Refresh sensor";
            // Refresh Sensor
            tDashboardControler.refreshSensor();
            tMainData.sDebug = "Refresh sensor end";

            // Init étape
            timerReadO2Sensor.uiO2State = 0
            // Demande de lecture capteur
            timerReadO2Sensor.start();
        }
    }
    // Timer
    Timer {
        id: timerReadO2Sensor
        interval: 500;
        running: false;
        repeat: true;
        property bool bWriteDone: false
        property bool bReadDone: false
        property int uiO2State: 0
        onTriggered: {
            // Tant que le grafcet est en busy
            if( tMainEvent.isModbusBusyState() ){
                return;
            }
            console.log( "bGenericSensor = "+bGenericSensor )
            console.log( "bWiper = "+bWiper )
            console.log( "tMainData.bErrorSensorNeedUpdate = "+tMainData.bErrorSensorNeedUpdate )
            console.log( "tMainData.bError = "+tMainData.bError )
            // Test si c'est un O2
            if(  ( !bGenericSensor && !bWiper )
              && !tMainData.bErrorSensorNeedUpdate
              && !tMainData.bError ){
                console.log("O2 DEBUGGGGGGG");
                console.log((new Date()).toLocaleTimeString());
                // Test de l'état
                switch( timerReadO2Sensor.uiO2State ){
                    //-------------------------
                    // Requête de lecture
                    //-------------------------
                    case 0:
                        console.log("tO2Event.sendSwitchToCalibrationModeCMD()")
                        // Requête de passage en mode calibration
                        tO2Event.sendSwitchToCalibrationModeCMD();
                        // Changement d'état
                        timerReadO2Sensor.uiO2State++;
                    break;
                    //-------------------------
                    // Passage du capteur en mode calibration
                    //-------------------------
                    case 1:
                        console.log("Read CMD - tO2Event.sendReadCMD");
                        // Requête de lecture
                        tO2Event.sendReadCMD();
                        // Changement d'état
                        timerReadO2Sensor.uiO2State++;
                    break;
                    //-------------------------
                    // Lecture offset temperature
                    //-------------------------
                    case 2:
                        console.log("Read Offset CMD");
                        // Uniquement en mode nke ou métro
                        if( tMainData.bMetroMode || tMainData.bNkeMode )
                         // Requête de lecture
                         tO2Event.sendReadTemperatureOffsetCMD();
                        // Changement d'état
                        timerReadO2Sensor.uiO2State++;
                    break;
                    //-------------------------
                    // CMD Lecture configuration vers PC
                    //-------------------------
                    case 3:
                        // CMD read
                        tO2Event.readConfFromI2CSensor();
                        // Changement d'état
                        timerReadO2Sensor.uiO2State++;
                    return;
                    //-------------------------
                    // Requête de lecture
                    //-------------------------
                    case 4:
                        // Stop du timer et du grafcet
                        timerReadO2Sensor.stop();
                        console.log("End of sensor calibration switch");
                    break;
                    //-------------------------
                    // Default
                    //-------------------------
                    default:
                        // Stop du timer et du grafcet
                        timerReadO2Sensor.stop();
                        console.log("Error dans la gestion du grafcet");
                    break;
                }
            }
            else{
                // Stop du timer et du grafcet
                timerReadO2Sensor.stop();
            }
            // Réinit
            tMainData.bError = false;
            // Fin du grafcet Refresh capteur
            // Lancement de la vérification mise à jour capteur ?
        }
    }

    //--------------------------------------------------
    // Bloc : dashboard information
    //--------------------------------------------------
    DashboardInformationBloc{
        id: m_dashboardInformationBlocID
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.leftMargin: uiBlocWidthOffset
        anchors.rightMargin: uiBlocWidthOffset
        //anchors.top: m_dashboardRefreshBlocID.bottom
        anchors.top: m_dashboardRefreshBlocID.bottom
        anchors.topMargin: 30
        // Property
        m_sIdentifier:      dashBoardPartID.m_sIdentifier
        m_sChannels:        dashBoardPartID.m_sChannels
        m_sManufacturer:    dashBoardPartID.m_sManufacturer
        m_sSerialNumber:    dashBoardPartID.m_sSerialNumber
        m_sFirmwareVersion: dashBoardPartID.m_sFirmwareVersion
    }

    //--------------------------------------------------
    // Bloc bottom : Licence details
    //--------------------------------------------------
    DashboardBlocBottom{
        id:m_licenceInfoID
        sText1: "Licence"
        sText2: "details"
        sButtonText: "Open"
        sLogo: "qrc:/Logo/BuiltwithQtlogos/built-with-Qt_40px_606060.png"
        uiLogoWidth: 40
        anchors.left: m_dashboardInformationBlocID.left
        anchors.top: m_dashboardInformationBlocID.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 50
        onButtonClicked: { w_about.open() }
    }

    //--------------------------------------------------
    // Bloc bottom : Online support
    //--------------------------------------------------
    DashboardBlocBottom{
        sText1: "Help"
        sText2: "online support"
        sButtonText: "Open"
        sLogo: "qrc:/Logo/Dashboard/dashboard-help-40.png"
        uiLogoWidth: 40
        anchors.left: m_licenceInfoID.right
        anchors.top: m_licenceInfoID.top
        anchors.leftMargin: 30
        anchors.topMargin: 0
        onButtonClicked: { tDashboardControler.openHelpSupportUrl()
                           messageDialogPasswordHelp.open() }
    }

    //------------------------------------------------------
    // Fenêtre
    //------------------------------------------------------
    //----------------------------------------------------
    // Fenêtre About
    //----------------------------------------------------
    Window_About { id: w_about }

    // Debug
    Text {
       id: text1
       x: 128
       y: 729
       text: tMainData.sDebug
       font.pixelSize: 12
       visible: bDebugQML
    }

    // MessageDialog - Password Help
    MessageDialogPerso {
        id: messageDialogPasswordHelp
        title: qsTr("Password online support")
        text: qsTr("Password: NkeSupport56$")
    }
}






