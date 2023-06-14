import QtQuick 2.0
//import QtCharts 2.3
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Controls 2.13

// Ajout pour directive Screen.desktopAvailableHeigt
import QtQuick.Window 2.2

import "Component"
import "Windows"

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Sensor
    property string m_sSensorSN: tDataSensor.sSN
    // Ch0
    property bool m_bCh0Active:  ( tDataSensor.uiChannelNb > 0 )
    property bool m_bCh0Enabled: tDataSensorCh0.bEnabled
    property int m_uiCh0Index:   tDataSensorCh0.uiIndex
    property double m_dCh0Value: tDataSensorCh0.fValue
    property string m_sCh0Value: tDataSensorCh0.sValue
    property string m_sCh0Name:  tDataSensorCh0.sName
    property string m_sCh0Unit:  tDataSensorCh0.sUnit
    // Ch1
    property bool m_bCh1Active:  ( tDataSensor.uiChannelNb > 1 )
    property bool m_bCh1Enabled: tDataSensorCh1.bEnabled
    property int m_uiCh1Index:   tDataSensorCh1.uiIndex
    property double m_dCh1Value: tDataSensorCh1.fValue
    property double m_sCh1Value: tDataSensorCh1.sValue
    property string m_sCh1Name:  tDataSensorCh1.sName
    property string m_sCh1Unit:  tDataSensorCh1.sUnit
    // Ch2
    property bool m_bCh2Active:  ( tDataSensor.uiChannelNb > 2 )
    property bool m_bCh2Enabled: tDataSensorCh2.bEnabled
    property int m_uiCh2Index:   tDataSensorCh2.uiIndex
    property double m_dCh2Value: tDataSensorCh2.dValue
    property string m_sCh2Value: tDataSensorCh2.sValue
    property string m_sCh2Name:  tDataSensorCh2.sName
    property string m_sCh2Unit:  tDataSensorCh2.sUnit

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property int uiBlocWidthOffset: 60
    // Sauvegarde de l'ancienne cadence
    property int saveMainMeasurePeriod: 5;
    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal advancedSettingsClicked()

    //==============================================================
    // Fonction
    //==============================================================
    // Signal de fin de refresh
    function vFEnfOfRefreshSignal(){
        console.log("vFEnfOfRefreshSignal")
        // Propagation du signal
        plotview.vFEnfOfRefreshSignal()
        ///%%AP - 2020.09.14 - Caché
        // Propagation du signal
        windowsQwtPlot.vFEnfOfRefreshSignal( liveviewID.channelNumber )
    }
    // Démarrage calibration O2
    function vFStartO2CalibrationLiveview(){
        console.log("vFStartO2CalibrationLiveview")
        // Sauvegarde de la cadence principale
        saveMainMeasurePeriod = uiMainMeasurePeriod;
        // Modification de la cadence
        vFMAINSetMeasurePeriod( 5 );
        // Démarrage de la mesure
        vFMAINStartMeasure()
        // Start mesure O2
        vFMAINStartMeasureO2Calib()
        // Modification état bouton switch
        bStarMeasureSwitchState = true;
        // Affichage de la fenêtre courbe
        //windowsQwtPlot.y = Screen.desktopAvailableHeight - windowsQwtPlot.height
        // Fonctionne bien
        //windowsQwtPlot.x = Screen.desktopAvailableWidth - windowsQwtPlot.width
        windowsQwtPlot.y = window.y + window.height/2 - windowsQwtPlot.height/2;
        windowsQwtPlot.x = window.x + window.width;
        windowsQwtPlot.visible = true
    }
    // Fermeture calibration O2
    function vFStopO2CalibrationLiveview(){
        console.log("vFStopO2CalibrationLiveview")
        // Stop de la mesure
        vFMAINStopMeasure()
        // Stop mesure O2
        vFMAINStopMeasureO2Calib()
        // On remet la cadence de mesure initiale
        vFMAINSetMeasurePeriod( saveMainMeasurePeriod )

        // Créer un bug ?
        // Modification état switch bouton
        bStarMeasureSwitchState = false
        // Retrait de l'affichage de la courbe
        windowsQwtPlot.visible = false
    }

    // Mise à jour de la profondeur d'affichage du graphique
    function vFLiveViewPart_UpdateGraphicalDepth( uiGraphicalDepth ){
        console.log("vFLiveViewPart_UpdateGraphicalDepth")
        // Mise à jour de la profondeur d'affichage du graphique
        plotview.vFPlotView_UpdateGraphicalView( uiGraphicalDepth );
        // Mise à jour
        windowsQwtPlot.vFWindowsQwt_UpdateGraphicalView( uiGraphicalDepth );
    }

    // Fenêtre d'affichage graphique
    WindowsQwt {
        id: windowsQwtPlot
        visible: false
    }


    //=============================================================
    // Connection
    //=============================================================
    Connections{
        target:liveviewID
        //
        function onMeasureFinishedChanged() {
            console.log("============================")
            console.log("= onMeasureFinishedChanged =")
            console.log("============================")
            // Si mesure stoppée
            if( !bStarMeasureSwitchState && !bStartMeasureO2 ) return;

            //%%AP - 2021.09.15 - Déplace en dessous
            //windowsQwtPlot.plotview.vFDisplayData()
            windowsQwtPlot.vFDisplayNewSamples();
            // Affichage de la donnée
            plotview.vFDisplayNewSamples();
        }
    }

    function vFCallStartMeasure() {
        console.log("Start measure");
        liveviewID.startMeasure()
    }

    //==============================================================
    // QML
    //==============================================================

    //width: 662
    width: window.width - 228
    //height: 600
    height: window.height
    implicitWidth: 590
    implicitHeight: 600

    property alias liveViewPartIDWidth: liveViewPartID.width
    id: liveViewPartID

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: liveViewPartID.width
        height: liveViewPartID.height
        color: "#00efeef4"
        radius: 1

        //-------------------------------
        // Titre H2
        //-------------------------------
        H2Item{
            id: m_H2ItemID
            sH2Title: "Live view"
            sH2Img: "qrc:/Logo/LiveView/Realtime.png"
            uiLogoWidth: 34
            anchors.top: parent.top
            anchors.topMargin: 20
            bHideTriangle: true
        }

        //-------------------------------
        // Bouton advanced panel
        //-------------------------------
        LiveviewButtonAdvancedPanel{
            id: m_buttonAdvancedPanel
            //x: 423
            anchors.top: parent.top
            anchors.topMargin: 28
            anchors.right: parent.right
            anchors.rightMargin: 2
            onClicked:{
                //m_liveviewID.advancedSettingsClicked()
                liveViewStackView.incrementCurrentIndex();
            }
        }

        //--------------------------------------------------------
        // Valeur numérique
        //--------------------------------------------------------
        RowLayout {
            id: rowLayout
            x: 60
            y: 92
            width: liveViewPartID.width - 60
            height: 159
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0
            //activeFocusOnTab: false

            //-------------
            // Ch0
            //-------------
            LiveviewNumGraphicalBloc{
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                visible: liveViewPartID.m_bCh0Active

                bBlocActive:     liveViewPartID.m_bCh0Enabled
                m_sChannelSN:    liveViewPartID.m_sSensorSN
                m_uiSensorChannelIndex: liveViewPartID.m_uiCh0Index
                m_sChannelValue: liveViewPartID.m_sCh0Value
                m_sChannelName:  liveViewPartID.m_sCh0Name
                m_sChannelUnit:  liveViewPartID.m_sCh0Unit
                // On blocClicked
                onBlocClicked: {
                    if( !liveViewPartID.m_bCh0Enabled ){
                        tLiveviewControler.setNumGraphicalBlocEnabled(0)
                        plotview.vFEnableChannel(0)
                    }
                    else{
                        tLiveviewControler.setNumGraphicalBlocDisabled(0)
                        plotview.vFDisableChannel(0)
                    }
                }
            }

            //------------------------
            // Ch1 - Affichage carré
            //------------------------
            LiveviewNumGraphicalBloc{
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                visible: liveViewPartID.m_bCh1Active

                bBlocActive:     liveViewPartID.m_bCh1Enabled
                m_sChannelSN:    liveViewPartID.m_sSensorSN
                m_uiSensorChannelIndex: liveViewPartID.m_uiCh1Index
                m_sChannelValue: liveViewPartID.m_sCh1Value
                m_sChannelName:  liveViewPartID.m_sCh1Name
                m_sChannelUnit:  liveViewPartID.m_sCh1Unit
                // On blocClicked
                onBlocClicked: {
                    if( !liveViewPartID.m_bCh1Enabled ){
                        tLiveviewControler.setNumGraphicalBlocEnabled(1)
                        plotview.vFEnableChannel(1)
                    }
                    else{
                        tLiveviewControler.setNumGraphicalBlocDisabled(1)
                        plotview.vFDisableChannel(1)
                    }
                }
            }

            //------------------------
            // Ch2 - Affichage carré
            //------------------------
            LiveviewNumGraphicalBloc{
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                visible: liveViewPartID.m_bCh2Active

                bBlocActive:     liveViewPartID.m_bCh2Enabled
                m_sChannelSN:    liveViewPartID.m_sSensorSN
                m_uiSensorChannelIndex: liveViewPartID.m_uiCh2Index
                m_sChannelValue: liveViewPartID.m_sCh2Value
                m_sChannelName:  liveViewPartID.m_sCh2Name
                m_sChannelUnit:  liveViewPartID.m_sCh2Unit
                // On blocClicked
                onBlocClicked: {
                    if( !liveViewPartID.m_bCh1Enabled ){
                        tLiveviewControler.setNumGraphicalBlocEnabled(2)
                        plotview.vFEnableChannel(2)
                    }
                    else{
                        tLiveviewControler.setNumGraphicalBlocDisabled(2)
                        plotview.vFDisableChannel(2)
                    }
                }
            }
        }


        //--------------------------------------------------------
        // Affichage graphique
        //--------------------------------------------------------
        Rectangle {
            id: chartBackground
            x: 5
            //y: 274
            width: liveViewPartID.width - 10
            //height: liveViewPartID.height - 235
            height: liveViewPartID.height - 280
            color: "#ffffff"
            //color: "#fbfbfb"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            border.color: "#9a000000"
            border.width: 3

            function vFDisplayNewSamples(){
                console.log("vFDisplayNewSamples")
                //liveviewID.
                plotview.vFDisplayNewSamples();
            }
            PlotView {
                id: plotview
                width: parent.width - 40
                height: parent.height - 45
                anchors.verticalCenterOffset: -3
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                id: buttonExtendGraphic
                width: 85
                //x: 482
                //y: 3
                height: 26
                text: qsTr("Extend")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                anchors.left: parent.left
                anchors.leftMargin: 3
                display: AbstractButton.TextBesideIcon
                icon.source: "qrc:/Logo/LiveView/liveview-extend.png"
                onClicked: {
                    windowsQwtPlot.y = window.y + window.height/2 - windowsQwtPlot.height/2;
                    windowsQwtPlot.x = window.x + window.width;
                    windowsQwtPlot.visible = true
                }
            }
            Button {
                id: buttonClearGraphic
                //x: 482
                //y: 3
                width: 85
                height: 26
                text: qsTr("Clear")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                anchors.left: buttonExtendGraphic.right
                anchors.leftMargin: 3
                display: AbstractButton.TextBesideIcon
                icon.source: "qrc:/Logo/LiveView/liveview-clear.png"
                onClicked: {
                    //windowsQwtPlot.visible = true
                    plotview.vFClearGraphic()
                }
            }
            Button {
                id: buttonExportGraphic
                //x: 482
                //y: 3
                width: 85
                height: 26
                text: qsTr("Export")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                anchors.left: buttonClearGraphic.right
                anchors.leftMargin: 3
                display: AbstractButton.TextBesideIcon
                icon.source: "qrc:/Logo/LiveView/liveview-export.png"
                onClicked: {
                    //windowsQwtPlot.visible = true
                    plotview.vFExport()
                }
            }

            SwitchButtonPerso {
                id: recordMeasureID
                width: 175
                //height: 59
                anchors.left: buttonExportGraphic.right
                anchors.verticalCenterOffset: -5
                anchors.leftMargin: 25
                //anchors.bottom:  buttonExportGraphic.bottom
                text: qsTr("Record Measurement")
                anchors.verticalCenter: buttonExportGraphic.verticalCenter
                enabled: !bStarMeasureSwitchState
                checked: true
            }
            Button {
                id: buttonRecordingPath
                width: 90
                height: 26
                text: qsTr("Location...")
                /*
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                */
                anchors.verticalCenter: recordMeasureID.verticalCenter
                anchors.left: recordMeasureID.right
                anchors.leftMargin: 3
                display: AbstractButton.TextOnly
                property string sRecordLocation: ""
                //icon.source: "qrc:/Logo/LiveView/liveview-export.png"
                onClicked: {
                    console.log("Location: "+CurDirPath);
                    var sLocation = CurDirPath.split("/Exe")[0]
                    console.log(sLocation+"/Measure")
                    sRecordLocation = sLocation+"/Measure"
                    // Ouverture de la fenêtre
                    messageDialogRecordingLocation.open();
                    tMainEvent.openWindowsExplorer()
                }
            }
            // MessageDialog - Update interface success
            MessageDialogPerso {
                id: messageDialogRecordingLocation
                title: qsTr("Measure recording location")
                text: buttonRecordingPath.sRecordLocation
            }
        }


        //-------------------------------
        // Bouton start acquisition
        //-------------------------------
        SwitchButtonPerso {
            id: startStopMeasureID
            x: 286
            y: 31
            width: 117
            //height: 59
            text: qsTr("On/off")
            anchors.verticalCenter: dashboardTitle.verticalCenter
            //checked: false
            checked: bStarMeasureSwitchState
            onClicked: {
                console.log( "=> SwitchButtonPerso change state" );
                // Si switch mesure n'est pas activé
                if( !bStarMeasureSwitchState ){
                    tLiveviewControler.setNumGraphicalBlocEnabled(0)
                    tLiveviewControler.setNumGraphicalBlocEnabled(1)
                    tLiveviewControler.setNumGraphicalBlocEnabled(2)

                    // Blocage click utilisateur
                    vFLockUserClick();
                    var uiChannel = 0;
                    console.log(Date.now()+" > === BOUcLE VERIF AVANT MESURE ==");
                    console.log(Date.now()+" > liveviewID.channelNumber: "+liveviewID.channelNumber );
                    console.log(Date.now()+" > Start While 1");
                    // Recherche d'une voie non-activée
                    while( uiChannel < liveviewID.channelNumber ){
                        console.log(Date.now()+" > uiChannel: "+uiChannel );
                        // Vérification que la voie 0 est bien activé (activation si pas)
                        if( !tMainEvent.isChannelActivated( uiChannel ) ){
                            // Requête pour modifier la configuration
                            tMainEvent.setChannelActivation( uiChannel, true );
                            // Sauvegarde de la configuration dans le capteur
                            tMainEvent.saveChannelConfiguration( uiChannel );
                            // Lancement du timer de fin de requête
                            timerStartMeasureRequest.uiCurrentChannel = uiChannel;
                            timerStartMeasureRequest.start();
                            break;
                        }
                        uiChannel++;
                    }
                    console.log(Date.now()+" > End While 1");
                    // Si toutes les voies sont déjà activées
                    if( uiChannel >= liveviewID.channelNumber ){
                        // Activation de la mesure
                        console.log(Date.now()+" > Start mesure");
                        // Si enregistrement actif
                        if( recordMeasureID.checked ){
                            // Création d'un fichier
                            liveviewID.startMeasureInit();
                        }
                        //%%AP - 2021.10.11 - Clear à chaque démarrage ?
                        // Effacement du graphique
                        plotview.vFClearGraphic();
                        // Effacement graphique fenêtre supplémentaire
                        windowsQwtPlot.vFWindowsQwt_ClearGraphic();
                        // Changement d'état
                        bStarMeasureSwitchState = true
                        // Réautorise le click utilisateur
                        vFAllowUserClick();
                        //%%AP - 2020.12.10 - Ajout
                        vFMAINStartMeasure();
                    }
                }
                // -- Stop mesure --
                else{
                    console.log("Stop mesure");
                    // Création d'un fichier
                    liveviewID.stopMeasureInit( uiMainMeasurePeriod );
                    // Changement d'état
                    bStarMeasureSwitchState = false;
                    tLiveviewControler.setNumGraphicalBlocDisabled(0)
                    tLiveviewControler.setNumGraphicalBlocDisabled(1)
                    tLiveviewControler.setNumGraphicalBlocDisabled(2)

                    //%%AP - 2020.12.10 - Ajout
                    vFMAINStopMeasure();
                }
            }
        }
        // Timer pour la requête set config : active les voies désactivées
        Timer {
            id: timerStartMeasureRequest
            interval: 100;
            running: false;
            repeat: true;
            property int  uiCurrentChannel: 0
            onTriggered: {
                console.log(Date.now()+" > Test tMainEvent.isModbusBusyState");
                // Tant que le grafcet est en busy
                if( tMainEvent.isModbusBusyState() ){
                    return;
                }
                // On passe à la voie suivante
                uiCurrentChannel++;
                while( uiCurrentChannel < liveviewID.channelNumber ){
                    // Vérification que la voie 0 est bien activé (activation si pas)
                    if( !tMainEvent.isChannelActivated( uiCurrentChannel ) ){
                        // Requête pour modifier la configuration
                        tMainEvent.setChannelActivation( uiCurrentChannel, true );
                        // Sauvegarde de la configuration dans le capteur
                        tMainEvent.saveChannelConfiguration( uiCurrentChannel );
                        // Lancement du timer de fin de requête
                        //timerStartMeasureRequest.uiCurrentChannel = uiChannel
                        //timerStartMeasureRequest.start();
                        return;
                    }
                    uiCurrentChannel++;
                }
                // Stop du timer
                timerStartMeasureRequest.stop();
                timerStartMeasureRequest.uiCurrentChannel = 0
                // Passage en busy
                //busyCalibPoints.visible   = false;

                // Changement d'état
                bStarMeasureSwitchState = true
                // Réautorise le click utilisateur
                vFAllowUserClick();

                // Activation de la mesure
                console.log(Date.now()+" > Start mesure");
                // Si enregistrement actif
                if( recordMeasureID.checked ){
                    // Création d'un fichier
                    liveviewID.startMeasureInit();
                }
                // Effacement du graphique
                plotview.vFClearGraphic();
                // Effacement graphique fenêtre supplémentaire
                windowsQwtPlot.vFWindowsQwt_ClearGraphic();
                //%%AP - 2020.12.10 - Ajout
                vFMAINStartMeasure();
            }
        }
    }

    //-----------------------------------------------------
    // Démarrage mesure O2 calibration
    //-----------------------------------------------------
    function vFLiveViewStartMeasureFile(){
        console.log("vFLiveViewStartMeasureFile")
        if( recordMeasureID.checked ){
            // Création d'un fichier
            liveviewID.startMeasureInit();
        }
        // Effacement du graphique
        plotview.vFClearGraphic()
    }
    //-----------------------------------------------------
    // Stop mesure O2 calibration
    //-----------------------------------------------------
    function vFLiveViewStopMeasureFile(){
        console.log("vFLiveViewStopMeasureFile")
        // Création d'un fichier
        liveviewID.stopMeasureInit( uiMainMeasurePeriod );
    }

    // Debug
    Text {
       id: text1
       x: 19
       y: 253
       text: tMainData.sDebug
       font.pixelSize: 12
       visible: bDebugQML
    }

    function vFLiveviewPartStopMeasure_ClickSwitch(){
        console.log("vFLiveviewPartStopMeasure_ClickSwitch")
        // Seulement si mesure active
        if( bStarMeasureSwitchState ){
            // Click de désactivation
            startStopMeasureID.click()
        }
    }

    //---------------------------------------------------------
    // Fenêtre continuer fichier
    //---------------------------------------------------------
    /*
    MessageDialog {
        id: windowContinueFile
        title: qsTr("Liveview")
        //icon: StandardIcon.Question
        text: qsTr("Continue current recording ?")
        standardButtons: StandardButton.Yes | StandardButton.No
        //Component.onCompleted: visible = true
        //Component.onCompleted: visible = true
        onYes: {
            console.log("Continue")
            // Appel de la fonction de mise à jour
            //tMainEvent.vFMainWindowStartUpdate()
        }
        onNo: {
            console.log("New recording")
        }
        onRejected: {
            console.log("Rejected")
        }
    }
    */

}






