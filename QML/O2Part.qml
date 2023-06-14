import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

import QtQuick.Dialogs 1.3
import QtQuick.Extras 1.4

import QtQml.Models 2.15


import "Component"
import "Windows"

Item {

    // DEBUG
    //=============================================================
    // Property
    //=============================================================
    //property bool bUpdate: false
    // Variable d'affichage
    property bool bSetSensorMsg: false

    property string m_ucSensorType: ""
    property string m_ucIntensity: ""
    property int  m_ucAmp: 0
    property int  m_uliCodeDphi0   : 0
    property int  m_uliCodeDphi100 : 0
    property bool m_bExternalTemperature: false
    property int  m_liTemperature : 0
    property int  m_uliPressure   : 0
    property bool m_bCalibrated   : false

    property bool m_bExternalTemperature100: false
    property int  m_liTemperature100: 0
    property int  m_uliPressure100: 0

    property bool m_bExternalTemperature0: false
    property int  m_liTemperature0: 0

    // Si calibration usine ou O/100
    property bool bFactorycalibration: true
    // Mode expert
    property bool bDphiDetailDisplay: false

    // Sauvegarde de point de calibration
    property int m_uliPreviousDphi100: 0
    property int m_uliPreviousDphi0: 0
    property int m_uliPreviousTemperature100: 0
    property int m_uliPreviousTemperature0: 0

    property double m_fTextOpacityFullValue: 1.0
    property double m_fTextOpacityMidValue: 0.6

    // Etat du bouton start calibration quand busy
    property bool bBtnStartCalibrationBusy: false

    //=============================================================
    // Connection
    //=============================================================
    Connections {
        target:tO2Event
        function onConfigO2DisplaySignal( bExternalTemperature,
                                          liTemperature,
                                          uliPressure,
                                          // SC7-540-200
                                          ucSensorType,
                                          ucIntensity,
                                          ucAmp,
                                          // Phase shift with sensor code
                                          uliCodeDphi0,
                                          uliCodeDphi100,
                                          // Temperature offset
                                          liTemperatureOffset,
                                          uliDummy,
                                          // Etat de la calibration
                                          bCalibrated,
                                          // Calibration
                                          bExternalTemperature0,
                                          liTemperature0,
                                          bExternalTemperature100,
                                          liTemperature100,
                                          uliPressure100 ) {
            console.log( "== onConfigO2DisplaySignal ==" );
            console.log( "fTemperature = "+liTemperature );
            console.log( "fPressure = "+uliPressure );
            console.log( "ucSensorType = "+ucSensorType );
            console.log( "ucIntensity = "+ucIntensity );
            console.log( "ucAmp = "+ucAmp );
            console.log( "uliCodeDphi0 = "+uliCodeDphi0 );
            console.log( "uliCodeDphi100 = "+uliCodeDphi100 );

            console.log( "liTemperatureOffset = "+liTemperatureOffset );
            console.log( "uliDummy = "+uliDummy );
            console.log( "bCalibrated = "+bCalibrated );
            console.log( "bExternalTemperature0 = "+bExternalTemperature0 );
            console.log( "fTemperature0 = "+liTemperature0 );
            console.log( "bExternalTemperature100 = "+bExternalTemperature100 );
            console.log( "fTemperature100 = "+liTemperature100 );
            console.log( "fPressure100 = "+uliPressure100 );

            // Calibration state
            m_bCalibrated = bCalibrated

            // Selon l'état de bCalibrated
            if( bCalibrated ){
                // On est en calibration 100%/0%
                selectO2CalibType.currentIndex = 1;
                bFactorycalibration = false;
            }else{
                // On est en factory calibration
                selectO2CalibType.currentIndex = 0;
                bFactorycalibration = true;
            }

            // Sauvegarde des valeurs dphi dans variable
            m_uliCodeDphi100 = uliCodeDphi100
            m_uliCodeDphi0   = uliCodeDphi0
            m_liTemperature100 = liTemperature100
            m_liTemperature0   = liTemperature0

            // Settings
            inputSC0.text = ucSensorType.toString()+ucIntensity.toString()+ucAmp.toString()
            inputSC1.text = parseInt(uliCodeDphi0 / 100)
            inputSC2.text = parseInt(uliCodeDphi100 / 100)

            // Condition in the sample
            //switchExternalTemperature.checked = bExternalTemperature
            //inputFixedTemperature.text        = parseFloat( liTemperature ) / 1000
            inputFixedPressure.text           = parseFloat( uliPressure ) / 1000
            // Temperature offset
            inputTemperatureOffset.text = parseFloat( liTemperatureOffset ) / 1000;

            // Air calibration
            inputFixedPressure100.text           = parseFloat( uliPressure100 ) / 1000

            // ShowPoint popup
            showPointDphi0.text   = parseFloat( uliCodeDphi0 ) / 1000;
            showPointDphi100.text = parseFloat( uliCodeDphi100 ) / 1000;
            elementDphi0Display.text = parseFloat( uliCodeDphi0 ) / 1000;
            elementDphi100Display.text = parseFloat( uliCodeDphi100 ) / 1000;
            // SetPoint popup
            setPointDphi0.text   = parseFloat( uliCodeDphi0 ) / 1000
            setPointDphi100.text = parseFloat( uliCodeDphi100 ) / 1000
/*
            // External temperature - Si activation
            if( switchExternalTemperature.checked ){
                inputFixedTemperature.enabled = false
            }
            // Désactivation
            else{
                inputFixedTemperature.enabled = true
            }
            */
        }
    }

    //=============================================================
    // Fonctions
    //=============================================================
    NumberAnimation {
        id: startNewCalibrationWindowWidth
        target: window
        properties: "width"
        from: window.width
        to: window.minimumWidth
        duration: 250
        onStopped: {
            // Remonte la scrollbar
            calibScrollViewO2.ScrollBar.vertical.position = 0

            // Désactivation de la première page
            firstCalibSwipe.enabled = false
            // Activation du bloc
            airCalibrationBloc.enabled = true
            //
            swipeViewO2Calibration.currentIndex = 1
            //
            //vFStartCalibrationAfterResize()
            //msgDialStartCalibration.vFStartCalibrationAfterResize()
        }
    }
    //-------------------------------------------------------------
    // Démarrage d'une nouvelle calibration
    //-------------------------------------------------------------
    function vFStartNewCalibration(){
        //%%AP - A remettre
        //var uiWindowWidth = parseFloat( window.width )
        //startNewCalibrationWindowWidth.from = uiWindowWidth
        //startNewCalibrationWindowWidth.start()

        //
        swipeViewO2Calibration.currentIndex = 1
        // Modification de la taille de la fenêtre
        window.width = window.minimumWidth
        // Remonte la scrollbar
        calibScrollViewO2.ScrollBar.vertical.position = 0
        // Désactivation de la première page
        firstCalibSwipe.enabled = false
        // Activation du bloc
        airCalibrationBloc.enabled = true
        //
        msgDialStartCalibration.vFStartCalibrationAfterResize()

        // Remonte la scrollbar
        //calibScrollViewO2.ScrollBar.vertical.position = 0
        //
        console.log("vFStartNewCalibration")
        // Affichage du graphique
        //vFStartO2CalibrationLiveview_MAIN();
        // Démarrage de la mesure

        // Désactivation de la première page
        //firstCalibSwipe.enabled = false
        // Activation du bloc
        //airCalibrationBloc.enabled = true
        //
        //swipeViewO2Calibration.currentIndex = 1
    }
    function vFReturnFromAirCalibration(){
        // Remonte la scrollbar
        calibScrollViewO2.ScrollBar.vertical.position = 0
        //
        console.log("vFReturnFromAirCalibration")
        // Stop affichage du graphique
        vFStopO2CalibrationLiveview_MAIN();



        // On fait apparaître le busy
        busyO2SetAir.visible  = true
        // On désactive l'état d'erreur si présent
        errorO2SetAir.visible = false

        // Remplacement des valeurs dphi0 et dphi100
        tO2Event.setCalibPointManually( m_uliPreviousDphi0, m_uliPreviousDphi100 )
        tO2Event.setAirCalibrationTemperature( m_uliPreviousTemperature100 );
        tO2Event.set0CalibrationTemperature( m_uliPreviousTemperature0 );
        // Envoi de la première commande modbus
        tO2Event.sendConfToI2CSensor()
        // Réinit des paramètres du timer
        timerSubmitReturnFromAirCalibration.ucState = 0
        // Démarrage du timer de gestion
        timerSubmitReturnFromAirCalibration.start()
    }
    // Timer pour le fonctionnement du start calibration
    Timer {
        id: timerSubmitReturnFromAirCalibration
        interval: 100;
        running: false;
        repeat: true;
        property int ucState: 0
        onTriggered: {
            // Tant que le grafcet est en busy
            if( tMainEvent.isModbusBusyState() ){
                return;
            }
            // Selon l'état courant
            switch( ucState ){
                // Fin envoi config I2C
                //-------------------------
                // CMD Envoi configuration vers capteur pyro
                //-------------------------
                case 0:
                    // CMD d'envoi de la configuration
                    tO2Event.sendConfigWriteCMD();
                    // Incrément de l'état
                    ucState++;
                // Quitte
                return;
                //-------------------------
                // CMD Lecture configuration vers capteur pyro
                //-------------------------
                case 1:
                    // CMD read
                    tO2Event.sendReadCMD();
                    // Incrément de l'état
                    ucState++;
                return;
                //-------------------------
                // CMD Lecture configuration vers PC
                //-------------------------
                case 2:
                    // CMD read
                    tO2Event.readConfFromI2CSensor();
                    // Incrément de l'état
                    ucState++;
                return;
                //-------------------------
                // CMD Sauvegarde permanente de la configuration
                //-------------------------
                case 3:
                    // CMD Save configuration permanently
                    tO2Event.saveRegisterPermanently();
                    // Incrément de l'état
                   // ucState++;
                break;
                //-------------------------
                // Default
                //-------------------------
                default: break;
            }
            // Stop du timer
            timerSubmitReturnFromAirCalibration.stop();

            // On fait apparaître le busy
            busyO2SetAir.visible  = false
            // Désactivation de la première page
            firstCalibSwipe.enabled = true
            // Activation du bloc
            airCalibrationBloc.enabled = false
            // Taille de fenêtre par défaut
            window.width = 1020
            //
            swipeViewO2Calibration.currentIndex = 0

            // On fait disparaitre le busy
            //busyO2StartCalibration.visible  = false
            // On désactive l'état d'erreur si présent
            //errorO2StartCalibration.visible = false
            // Réactivation du bouton start calibration
            //btnStartCalibration.visible     = true
            // Affichage du graphique
            //vFStartO2CalibrationLiveview_MAIN();
        }
    }

    //-------------------------------------------------------------
    // Set Air
    //-------------------------------------------------------------
    function vFExitAirCalibration_GoTo0PCalibration(){
        // Remonte la scrollbar
        calibScrollViewO2.ScrollBar.vertical.position = 0
        // Désactivation du bloc
        airCalibrationBloc.enabled = false
        // Activation du bloc
        zeroCalib.enabled = true
        // Swipe page 2
        //swipeViewO2Calibration.currentIndex = 2
    }
    function vFReturnFrom0PCalibration(){
        // Remonte la scrollbar
        calibScrollViewO2.ScrollBar.vertical.position = 0
        // Désactivation de la premier page
        //firstCalibSwipe.enabled = true
        airCalibrationBloc.enabled = true
        // Activation du bloc
        zeroCalib.enabled = false
        // Return to set air
        //swipeViewO2Calibration.currentIndex = 1
    }

    //-------------------------------------------------------------
    // End of calibration
    //-------------------------------------------------------------
    function vFEndCalibrationStart(){
        // Remonte la scrollbar
        calibScrollViewO2.ScrollBar.vertical.position = 0
        // Stop affichage du graphique
        vFStopO2CalibrationLiveview_MAIN();
        // Activation du bloc
        zeroCalib.enabled = false
        // Activation de la fin de calibration
        endCalibrationBloc.enabled = true
        // Go to end of calibration/save calibration
        //swipeViewO2Calibration.currentIndex = 2
    }
    function vFReturnFromEndCalibration(){
        // Remonte la scrollbar
        calibScrollViewO2.ScrollBar.vertical.position = 0
        // Affichage du graphique
        vFStartO2CalibrationLiveview_MAIN();
        // Activation du bloc
        zeroCalib.enabled = true
        // Désactivation de la fin de calibration
        endCalibrationBloc.enabled = false
        // Return to set 0%
        //swipeViewO2Calibration.currentIndex = 1
    }

    //=============================================================
    // Objet
    //=============================================================

    //*************************************************************
    // Le code de l'objet
    //*************************************************************
    id: o2PartID
    width: 662
    //height: 635
    height: 435
    property alias swipeViewO2Calibration: swipeViewO2Calibration
    implicitWidth: 662
    //implicitHeight: 1080
    //implicitHeight: 850
    implicitHeight: 650
    //implicitHeight: (tMainData.bMetroMode || tMainData.bNkeMode)?820:780

    clip: true

    Rectangle {
        id: rectanglePartO2
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#efeef4"
        clip: true
        //--------------------------------------------
        // Titre O2 Calibration avec logo
        //--------------------------------------------
        Rectangle {
            id: triangleTitleO2
            x: 92
            y: 91
            width: 28
            height: 28
            color: "#e4e3e9"
            rotation: 45
        }

        Text {
            id: o2Title
            x: 0
            y: 38
            width: 450
            height: 27
            text: qsTr("O2 Calibration")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
            font.weight: Font.DemiBold
            font.family: "Open Sans"
            Rectangle {
                id: rectangle4
                x: 72
                y: -21
                width: 68
                height: 68
                color: "#ffffff"
                radius: 31
                Image {
                    id: image
                    x: 19
                    y: 21
                    width: 34
                    anchors.horizontalCenterOffset: 1
                    source: "qrc:/Logo/Calibration/calib-title-low.png"
                    anchors.verticalCenterOffset: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }


        SwipeView{
            id: swipeViewO2Calibration
            width: parent.width
            height: parent.height - 100
            clip: true
            currentIndex: 0
            y: 99

            Item{
                id: firstCalibSwipe
              //--------------------------------------------------
                // Bloc : Condition in the sample
                //--------------------------------------------------
                DropShadow {
                    color: "#20000000"
                    radius: 12
                    verticalOffset: 0
                    anchors.fill: conditionInTheSamples
                    samples: 30
                    horizontalOffset: 0
                    source: conditionInTheSamples
                    visible: (!bFactorycalibration)
                }
                Rectangle {
                    id: conditionInTheSamples
                    width: o2PartID.width - 120
                    visible: true

                    //height: 213
                    //height: ( tMainData.bMetroMode || tMainData.bNkeMode)?255:213
                    //height: ( tMainData.bMetroMode || tMainData.bNkeMode )?325:283
                    //height: ( tMainData.bMetroMode || tMainData.bNkeMode )?372:330
                    //height: (340 + (( tMainData.bMetroMode || tMainData.bNkeMode )?42:0) + (( o2PartID.width < 780 )?40:0))
                    height: (185 + (( tMainData.bMetroMode || tMainData.bNkeMode )?42:0) + (( o2PartID.width < 780 )?40:0))
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    //anchors.top: sensorSettings.bottom
                    //anchors.topMargin: 20
                    Rectangle {
                        id: cits2
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        color: "#e4e3e9"
                        Text {
                            id: citsTxt
                            //x: 63
                            //y: 14
                            text: qsTr("Conditions in the sample")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.DemiBold
                            font.family: "Open Sans"
                            font.bold: false
                            font.pixelSize: 13
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutCits355
                        width: parent.width
                        //height: parent.height - 44 - 35
                        anchors.top: cits2.bottom
                        anchors.topMargin: 10
                        anchors.bottom: btnConfirmSettings.top
                        anchors.bottomMargin: 8
                        spacing: (o2PartID.width>800)?5:0
                        /*
                        // Calibration type explication
                        Rectangle {
                            //Layout.preferredHeight: 15
                            Layout.preferredHeight: (o2PartID.width>780)?15:8
                            Layout.preferredWidth: parent.width
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                text: qsTr("O2 sensor temperature compensation")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }

                        // Calibration type explication
                        Rectangle {
                            //x: 0
                            //y: 52
                            Layout.preferredWidth: parent.width
                            //height: 61
                            //Layout.preferredHeight: 45
                            Layout.preferredHeight: (o2PartID.width>780)?45:55
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false

                            Text {
                                color: "#545454"
                                text: qsTr("By default, the dissolved oxygen value is not temperature compensated.\r\n"
                                        +"Click on the \"O2 sensor temperature\" button to compensate this value"
                                        +" with embbeded O2 sensor temperature sensor. \r\n"
                                        +"Otherwise, the dissolved oxygen value will be determined for a fixed"
                                        +" temperature entered by the user: \"Actual temperature\" field.")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 11
                                verticalAlignment: Text.AlignTop
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }
                        */


                        // Fixed pressure explication tittle
                        Rectangle {
                            //width: parent.width
                            //height: 10
                            //Layout.preferredHeight: 15
                            Layout.preferredHeight: (o2PartID.width>780)?15:8
                            Layout.preferredWidth: parent.width
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                text: qsTr("Pressure compensation")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }

                        // Fixed pressure explication
                        Rectangle {
                            Layout.preferredWidth: parent.width
                            //height: 61
                            //Layout.preferredHeight: 20
                            Layout.preferredHeight: (o2PartID.width>780)?20:28
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false

                            Text {
                                id: fixedPressureExplanationText
                                color: "#545454"
                                text: qsTr("Dissolved oxygen value will be determined for a fixed"
                                        +" athmospheric pressure entered by the user: \"Athmospheric pressure\" field.")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 11
                                verticalAlignment: Text.AlignTop
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }

                        //--- Ligne ---
                        Rectangle {
                            id: conditionInTheSamplesTextLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }

                        //------------------------------
                        // External temperature
                        //------------------------------
                        /*
                        RowLayout {
                            id: rowExternalTemperature
                            Layout.preferredWidth: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            width: parent.width - 20

                            Rectangle {
                                id: rowExternalTemperature2
                                width: ( parent.width - 25 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgExternalTemperature
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    width: 30
                                    height: 16

                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    id: txtExternalTemperature
                                    text: qsTr("O2 sensor temperature")
                                    height: parent.height
                                    anchors.left: imgExternalTemperature.right
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                            Rectangle {
                                id: rectangleExternalTemperature4
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                // Switch bouton d'activation
                                SwitchButtonPerso {
                                    id: switchExternalTemperature
                                    height: 42
                                    scale: 0.9
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    onClicked: {
                                        console.log( " switchExternalTemperature = "+ switchExternalTemperature.checked )
                                        // Si activation
                                        if( switchExternalTemperature.checked ){
                                            inputFixedTemperature.enabled = false
                                            inputFixedTemperature.text = "-300"
                                        }
                                        // Désactivation
                                        else{
                                            inputFixedTemperature.enabled = true
                                            inputFixedTemperature.text = "20"
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                id: rectangleExternalTemperatureDummy
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                        } // RowLayout ExternalTemperature
                        //--- Ligne ---
                        Rectangle {
                            id: externalTemperatureLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        */
                        /*
                        //------------------------------
                        // Fixed temperature
                        //------------------------------
                        RowLayout {
                            id: rowFixedTemperature
                            Layout.preferredWidth: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            width: parent.width - 20

                            Rectangle {
                                id: rowFixedTemperature2
                                width: ( parent.width - 25 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgFixedTemperature
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    width: 30
                                    height: 16

                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    id: txtFixedTemperature
                                    text: qsTr("Actual temperature (°C)")
                                    height: parent.height
                                    anchors.left: imgFixedTemperature.right
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                            Rectangle {
                                id: rectangleFixedTemperature4
                                color: "#00000000"
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                TextField {
                                    id: inputFixedTemperature
                                    width: parent.width - 30
                                    text: qsTr("")
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true

                                    ToolTip {
                                        id: tooltipFixedTemperature
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
                            Rectangle {
                                id: rectangleFixedTemperatureDummy
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                        } // RowLayout ExternalTemperature
                        //--- Ligne ---
                        Rectangle {
                            id: fixedTemperatureLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        */

//                        //--- Ligne ---
//                        Rectangle {
//                            width: parent.width
//                            height: 1
//                            color: "#62888888"
//                            Layout.fillWidth: true
//                        }

                        //------------------------------
                        // Fixed pressure
                        //------------------------------
                        RowLayout {
                            id: rowFixedPressure
                            Layout.preferredWidth: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            width: parent.width - 20

                            Rectangle {
                                id: rowFixedPressure2
                                width: ( parent.width - 25 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgFixedPressure
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    width: 30
                                    height: 16

                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    id: txtFixedPressure
                                    text: qsTr("Atmospheric pressure  (hPa)")
                                    height: parent.height
                                    anchors.left: imgFixedPressure.right
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                            Rectangle {
                                id: rectangleFixedPressure4
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                TextField {
                                    id: inputFixedPressure
                                    width: parent.width - 30
                                    text: qsTr("")
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true

                                    ToolTip {
                                        id: tooltipFixedPressure
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
                            Rectangle {
                                id: rectangleFixedPressureDummy
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                        } // RowLayout ExternalTemperature
                        //--- Ligne ---
                        Rectangle {
                            id: fixedPressureLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        //------------------------------
                        // Temperature Offset
                        //------------------------------
                        RowLayout {
                            id: rowTemperatureOffset
                            Layout.preferredWidth: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            width: parent.width - 20
                            visible: tMainData.bMetroMode || tMainData.bNkeMode

                            Rectangle {
                                id: rowTemperatureOffset2
                                width: ( parent.width - 25 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgTemperatureOffset
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    width: 30
                                    height: 16

                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    id: txtTemperatureOffset
                                    text: qsTr("Temperature offset (°C)")
                                    height: parent.height
                                    anchors.left: imgTemperatureOffset.right
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                            Rectangle {
                                id: rectangleTemperatureOffset4
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                TextField {
                                    id: inputTemperatureOffset
                                    width: parent.width - 30
                                    text: qsTr("")
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true

                                    ToolTip {
                                        id: tooltipTemperatureOffset
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
                            Rectangle {
                                id: rectangleTemperatureOffsetDummy
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                        } // RowLayout ExternalTemperature
                        //--- Ligne ---
                        Rectangle {
                            id: fixedTemperatureOffsetLine
                            visible: tMainData.bMetroMode || tMainData.bNkeMode
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                    } // ColumnLayout
                    ButtonPerso {
                        id: btnConfirmSettings
                        width: 100
                        height: 28
                        text: qsTr("Submit")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8

                        onClicked: {
                            console.log("Condition in the sample submit");
                            //%%AP - 2022.08.26 - Création de variable pour cohérence
                            //var bExternalTemperature = switchExternalTemperature.checked
                            var bExternalTemperature = true;

                            //var liTemperature = parseInt( inputFixedTemperature.text )
                            var uliPressure   = parseInt( inputFixedPressure.text )

                            //var f parseFloat( inputTemperatureOffset.text );


                            // On désactive l'état d'erreur si présent
                            errorO2ConditionInTheSample.visible = false
                            // On fait apparaître le busy
                            busyO2ConditionInTheSample.visible = true

                            //--------------
                            // External temperature
                            //--------------
                            //%%AP - 2022.08.26 - Création variable
                            //liTemperature = parseInt( -300000 )
                            var liTemperature = parseInt( -300000 )
                            /*
                            if( bExternalTemperature ){
                                liTemperature = parseInt( -300000 )
                            }
                            else{
                                //-------------------------
                                // Fixed temperature
                                //-------------------------
                                var fFixedTemperature        = parseFloat( inputFixedTemperature.text );
                                var fFixedTemperatureRounded = parseFloat(parseInt(fFixedTemperature*1000))/1000;
                                // Vérification fonctionnement avec un entier
                                if( fFixedTemperature !== fFixedTemperatureRounded ){
                                    // On désactive l'état occupé
                                    busyO2ConditionInTheSample.visible  = false
                                    errorO2ConditionInTheSample.visible = true
                                    tooltipFixedTemperature.show(qsTr("Please enter a value with a maximum of 3 decimal places"))
                                    return;
                                }
                                if(  ( fFixedTemperature > 300  )
                                  || ( fFixedTemperature < -300 ) ){
                                    // On désactive l'état occupé
                                    busyO2ConditionInTheSample.visible  = false
                                    errorO2ConditionInTheSample.visible = true
                                    tooltipFixedTemperature.show(qsTr("Value must be between -300 and 300"))
                                    return;
                                }
                                liTemperature = parseInt( fFixedTemperature * 1000 )
                            }
                            */
                            //-------------------------
                            // Fixed pressure
                            //-------------------------
                            var fFixedPressure        = parseFloat( inputFixedPressure.text );
                            var fFixedPressureRounded = parseFloat(parseInt(fFixedPressure*1000))/1000;
                            // Vérification fonctionnement avec un entier
                            if( fFixedPressure !== fFixedPressureRounded ){
                                // On désactive l'état occupé
                                busyO2ConditionInTheSample.visible  = false
                                errorO2ConditionInTheSample.visible = true
                                tooltipFixedPressure.show(qsTr("Please enter a value with a maximum of 3 decimal places"))
                                return;
                            }
                            if(  ( fFixedPressure > 10000 )
                              || ( fFixedPressure < 0.001 ) ){
                                // On désactive l'état occupé
                                busyO2ConditionInTheSample.visible  = false
                                errorO2ConditionInTheSample.visible = true
                                tooltipFixedPressure.show(qsTr("Value must be between 0.001 and 10000"))
                                return;
                            }
                            uliPressure   = parseInt( fFixedPressure * 1000 )

                            //-------------------------
                            // Temperature offset
                            //-------------------------
                            // Si mode métro ou nke
                            if( tMainData.bMetroMode || tMainData.bNkeMode ){
                                var fTemperatureOffset        = parseFloat( inputTemperatureOffset.text );
                                var fTemperatureOffsetRounded = parseFloat(parseInt(fTemperatureOffset*1000))/1000;
                                // Vérification fonctionnement avec un entier
                                if( fTemperatureOffset !== fTemperatureOffsetRounded ){
                                    // On désactive l'état occupé
                                    busyO2ConditionInTheSample.visible  = false
                                    errorO2ConditionInTheSample.visible = true
                                    tooltipTemperatureOffset.show(qsTr("Please enter a value with a maximum of 3 decimal places"))
                                    return;
                                }
                                if(  ( fFixedPressure > 500000  )
                                  || ( fFixedPressure < -500000 ) ){
                                    // On désactive l'état occupé
                                    busyO2ConditionInTheSample.visible  = false
                                    errorO2ConditionInTheSample.visible = true
                                    tooltipTemperatureOffset.show(qsTr("Value must be between -500000 and +500000"))
                                    return;
                                }
                                var liTemperatureOffset   = parseInt( fTemperatureOffset * 1000 )
                                console.log( "liTemperatureOffset = "+liTemperatureOffset );
                                // Vers la structure capteur
                                tO2Event.setTemperatureOffset( liTemperatureOffset );
                            }
                            else{
                                // Forçage à 0
                                //tO2Event.setTemperatureOffset( 0 );
                            }


                            // Temperature offset
                           // inputTemperatureOffset.text = parseFloat( liTemperatureOffset ) / 1000;


                            console.log( "== Après validation ==" );
                            //console.log( "bExternalTemperature = "+bExternalTemperature );
                            console.log( "liTemperature = "+liTemperature );
                            console.log( "uliPressure = "+uliPressure );

                            // Conversion valeur
                            m_bExternalTemperature = bExternalTemperature;
                            m_liTemperature = liTemperature;
                            m_uliPressure   = uliPressure;
                            // Affichage fenêtre : calibration reinit
                            //bSetSensorMsg = true;
                            //msgDialSetConditionInTheSample.open();
                            // On fait apparaître le busy
                            //busyO2ConditionInTheSample.visible = true

                            // Vers le capteur
                            tO2Event.setConditionInTheSample(
                                        m_bExternalTemperature,
                                        m_liTemperature,
                                        m_uliPressure );
                            // Envoi de la commande modbus
                            tO2Event.sendConfToI2CSensor();
                            // Réinit des paramètres du timer
                            timerSubmitConditionInTheSample.ucState = 0
                            // Démarrage du timer de gestion
                            timerSubmitConditionInTheSample.start()
                        }
                    } // BoutonPerso

                    /*
                    //! [messagedialog]
                    MessageDialog {
                        id: msgDialSetConditionInTheSample
                        title: "Warning"
                        text: "Settings submit: calibration will be reset"
                        standardButtons: StandardButton.Ok | StandardButton.Cancel
                        onAccepted:{
                            if( bSetSensorMsg && StandardButton.Ok ){
                                bSetSensorMsg = false
                                console.log("OK");
                                // Vers le capteur
                                tO2Event.setConditionInTheSample(
                                            m_bExternalTemperature,
                                            m_liTemperature,
                                            m_uliPressure );
                                // Envoi de la commande modbus
                                tO2Event.sendConfToI2CSensor();
                                // Réinit des paramètres du timer
                                timerSubmitConditionInTheSample.ucState = 0
                                // Démarrage du timer de gestion
                                timerSubmitConditionInTheSample.start()
                                // On fait apparaître le busy
                                busyO2ConditionInTheSample.visible = true
                            }
                        }
                        onRejected:{
                            if( bSetSensorMsg ){
                                bSetSensorMsg = false
                                console.log("onRejected");
                                // On fait disparaître le busy
                                busyO2ConditionInTheSample.visible = false
                            }
                        }
                    }
                    */
                    // Timer pour le fonctionnement du submit sensor code
                    Timer {
                        id: timerSubmitConditionInTheSample
                        interval: 100;
                        running: false;
                        repeat: true;
                        property int ucState: 0
                        onTriggered: {
                            // Tant que le grafcet est en busy
                            if( tMainEvent.isModbusBusyState() ){
                                return;
                            }
                            // Selon l'état courant
                            switch( ucState ){
                                // Fin envoi config I2C
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 0:
                                    // CMD d'envoi de la configuration
                                    tO2Event.sendConfigWriteCMD();
                                    // Incrément de l'état
                                    ucState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 1:
                                    // CMD d'envoi de la configuration
                                    tO2Event.sendWriteTemperatureOffsetCMD();
                                    // Incrément de l'état
                                    ucState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 2:
                                    // CMD Save configuration permanently
                                    tO2Event.saveRegisterPermanently();
                                    // Incrément de l'état
                                    ucState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers capteur pyro
                                //-------------------------
                                case 3:
                                    // CMD read
                                    tO2Event.sendReadCMD();
                                    // Incrément de l'état
                                    ucState++;
                                return;
                                //-------------------------
                                // Lecture offset temperature
                                //-------------------------
                                case 4:
                                    console.log("Read Offset CMD");
                                    // Uniquement en mode nke ou métro
                                    if( tMainData.bMetroMode || tMainData.bNkeMode )
                                     // Requête de lecture
                                     tO2Event.sendReadTemperatureOffsetCMD();
                                    // Changement d'état
                                    ucState++;
                                break;
                                //-------------------------
                                // CMD Lecture configuration vers PC
                                //-------------------------
                                case 5:
                                    // CMD read
                                    tO2Event.readConfFromI2CSensor();
                                    // Incrément de l'état
                                    ucState++;
                                break;
                                // Default
                                default: break;
                            }
                            // Stop du timer
                            this.stop();
                            // On désactive l'état d'erreur si présent
                            //errorO2Settings.visible = false
                            errorO2ConditionInTheSample.visible = false
                            // On fait apparaître le busy
                            busyO2ConditionInTheSample.visible = false

                            // Gestion de l'affichage
                            if( selectO2CalibType.currentIndex == 0 )
                             bFactorycalibration = true;
                            else
                             bFactorycalibration = false;
                        }
                    }
                    //--------------------
                    // Busy : calibration settings
                    //--------------------
                    BusyIndicatorPerso {
                        id: busyO2ConditionInTheSample
                        width: 31
                        height: 32
                        anchors.left: btnConfirmSettings.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                    //--------------------
                    // Error : calibration settings
                    //--------------------
                    Image {
                        id: errorO2ConditionInTheSample
                        width: 31
                        height: 31
                        source: "qrc:/Logo/Global/No_Cross.png"
                        anchors.left: btnConfirmSettings.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                }

                //--------------------------------------------------
                // Bloc : Sensor settings
                //--------------------------------------------------
                DropShadow {
                    color: "#20000000"
                    radius: 12
                    verticalOffset: 0
                    anchors.fill: sensorSettings
                    samples: 30
                    horizontalOffset: 0
                    source: sensorSettings
                }
                Rectangle {
                    id: sensorSettings
                    //x: 60
                    //y: 99
                    width: o2PartID.width - 120

                    //implicitWidth: 536
                    //height: 330
                    height: 300
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: conditionInTheSamples.bottom
                    anchors.topMargin: 20
                    Rectangle {
                        id: sensorSettings2
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        color: "#e4e3e9"
                        Text {
                            id: sensorSettingsTxt
                            text: qsTr("Two possible calibration modes")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.DemiBold
                            font.family: "Open Sans"
                            font.bold: false
                            font.pixelSize: 13
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutSensorSettings
                        x: 0
                        //y: 39
                        width: parent.width
                        //height: parent.height - 44 - 44 + 4
                        anchors.top: sensorSettings2.bottom
                        anchors.topMargin: 8
                        anchors.bottom: btnConfirmSensorCode.top
                        anchors.bottomMargin: 8
                        /*
                        // Calibration type explication
                        Rectangle {
                            id: cits41512
                            x: 0
                            y: 52
                            Layout.preferredWidth: parent.width
                            //height: 10
                            Layout.preferredHeight: 15
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                text: "2 possible calibration mode :"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }
                        //--- Ligne ---
                        Rectangle {
                            id: rowO2CalibTypeLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        */

                        // Calibration type explication
                        Rectangle {
                            id: cits4151
                            x: 0
                            y: 52
                            Layout.preferredWidth: parent.width
                            //height: 10
                            Layout.preferredHeight: 15
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                text: qsTr("2 points 0%-100% mode")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }

                        // Calibration type explication
                        Rectangle {
                            id: cits416
                            x: 0
                            y: 52
                            Layout.preferredWidth: parent.width
                            //height: 61
                            Layout.preferredHeight: 30
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false

                            Text {
                                id: text551
                                color: "#545454"
                                text: qsTr("O2 sensor calibration is determined in 2 points, "
                                +"where dphi0 and dphi100 are the raw value measured in 0%-standard "
                                +"(anoxic) and 100%-standard (20.95% O2), respectively."
                                +"")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 11
                                verticalAlignment: Text.AlignTop
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                                width: parent.width
                            }
                        }

                        // Calibration type explication
                        Rectangle {
                            id: cits415
                            x: 0
                            y: 52
                            //width: parent.width
                            //height: 10
                            Layout.preferredHeight: 15
                            Layout.preferredWidth: parent.width
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                text: qsTr("Factory mode")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }

                        // Calibration type explication
                        Rectangle {
                            id: cits4161
                            x: 0
                            y: 52
                            Layout.preferredWidth: parent.width
                            //height: 61
                            Layout.preferredHeight: 45
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false

                            Text {
                                id: text55
                                color: "#545454"
                                text: qsTr("The factory calibration is provided with the whole sensor code. "
                                +"It contains rough calibration value for 20.95% and 0% calibration point (dphi100 and dphi0). "
                                +"They are measured during the production of the sensor at standard environmental conditions "
                                +"(20°C, 0%RH, 1013mbar).")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                font.pixelSize: 11
                                verticalAlignment: Text.AlignTop
                                wrapMode: Text.WordWrap
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                            }
                        }

                        //--- Ligne ---
                        Rectangle {
                            id: comLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }

                        //------------------------------
                        // Calibration type
                        //------------------------------
                        RowLayout {
                            id: rowO2CalibType
                            Layout.preferredWidth: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            width: parent.width - 20

                            Rectangle {
                                id: rowO2CalibType2
                                width: ( parent.width - 25 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height

                                Image {
                                    id: imgO2CalibType
                                    width: 30
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    fillMode: Image.PreserveAspectFit
                                    source: "qrc:/Logo/Calibration/calib-type.png"
                                }

                                Text {
                                    id: txtO2CalibType
                                    text: qsTr("Calibration mode")
                                    height: parent.height
                                    anchors.left: imgO2CalibType.right
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                            /*
                            ListModel {
                                id: listCalibTypeGeneral
                                ListElement { text:"None"; value: 'N' }
                                ListElement { text:"Multipoint"; value: 'M' }
                                ListElement { text:"Linear";     value: 'L' }
                                ListElement { text:"2nd degree polynomial"; value: '2' }
                                ListElement { text:"3rd degree polynomial"; value: '3' }
                            }
                            */
                            ListModel {
                                id: listO2CalibType
                                ListElement { text:qsTr("Factory mode"); }
                                /*ListElement { text:"2 points: 0%-100%"; }*/
                                // Réautorisation pour debug
                                ListElement { text:qsTr("2 points: 0%-100%"); }
                            }
                            ComboBox {
                                id: selectO2CalibType
                                width: 200

                                // Si activation
                                //enabled: !bNkeProdMode
                                model: listO2CalibType
                                height: 200
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                onAccepted: {
                                    //selectO2CalibType.currentIndex
                                    /*
                                    if( selectO2CalibType.currentIndex == 0 )
                                     bFactorycalibration = true;
                                    else
                                     bFactorycalibration = false;
                                     */
                                }

                                // Tooltip calib type
                                ToolTip {
                                    id: tooltipO2CalibType
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
                            Rectangle {
                                id: rectangleO2CalibTypeDummy
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

//                                //--------------------
//                                // Busy : start calibration
//                                //--------------------
//                                BusyIndicatorPerso {
//                                    id: busyO2StartCalibration
//                                    width: 31
//                                    height: 32
//                                    anchors.horizontalCenter: parent.horizontalCenter
//                                    anchors.verticalCenter: parent.verticalCenter
//                                    visible: bBtnStartCalibrationBusy
//                                }
//                                BlueButtonPerso {
//                                    id: btnStartCalibration
//                                    width: 120
//                                    //height: 28
//                                    height: 32
//                                    text: qsTr("Start calibration")
//                                    anchors.horizontalCenter: parent.horizontalCenter
//                                    anchors.verticalCenter: parent.verticalCenter
//                                    visible: (!bFactorycalibration) && (!bBtnStartCalibrationBusy)
//                                    //enabled: (!bFactorycalibration) && (!bBtnStartCalibrationBusy)
//                                    //onClicked: {
//                                    onButtonClicked: {
//                                        // Lancement
//                                        // Appel de la fonction
//                                        //vFStartNewCalibration()
//                                        bSetSensorMsg = true
//                                        msgDialStartCalibration.open()
//                                    }
//                                }
                            }
                        }

                        //--- Ligne ---
                        Rectangle {
                            id: externalSensorCodeLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        //------------------------------
                        // Sensor Code
                        //------------------------------
                        RowLayout {
                            id: rowSC
                            Layout.preferredWidth: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            width: parent.width - 20

                            Rectangle {
                                id: rowSC2
                                width: ( parent.width - 25 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgSC
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    width: 30
                                    height: 16

                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    id: txtSC
                                    text: qsTr("Sensor code")
                                    height: parent.height
                                    anchors.left: imgSC.right
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                            Rectangle {
                                id: rectangleSC4
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                TextField {
                                    id: inputSC0
                                    width: parent.width / 3 - 10
                                    text: qsTr("")
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true

                                    ToolTip {
                                        id: tooltipSC0
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
                                Text {
                                    id: txtSCSpace
                                    text: " - "
                                    anchors.left: inputSC0.right
                                    anchors.leftMargin: 0
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                                TextField {
                                    id: inputSC1
                                    width: parent.width / 3 - 10
                                    text: qsTr("")
                                    renderType: Text.QtRendering
                                    anchors.left: txtSCSpace.right
                                    anchors.leftMargin: 0
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true
                                    //color: "#ffffff"
                                    //%%AP - Test
                                    //ToolTip.visible: right
                                    //ToolTip.text: qsTr("Invalide value")
                                    enabled: (selectO2CalibType.currentIndex==0)

                                    ToolTip {
                                        id: tooltipSC1
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
                                Text {
                                    id: txtSCSpace2
                                    text: " - "
                                    anchors.left: inputSC1.right
                                    anchors.leftMargin: 0
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                                TextField {
                                    id: inputSC2
                                    width: parent.width / 3 - 10
                                    text: qsTr("")
                                    renderType: Text.QtRendering
                                    anchors.left: txtSCSpace2.right
                                    anchors.leftMargin: 0
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true
                                    enabled: (selectO2CalibType.currentIndex==0)
                                    //color: "#ffffff"
                                    //%%AP - Test
                                    //ToolTip.visible: right
                                    //ToolTip.text: qsTr("Invalide value")

                                    ToolTip {
                                        id: tooltipSC2
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
                            Rectangle {
                                id: rectangleSCDummy
                                width: parent.width / 3 - 10
                                height: 30
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                TextField {
                                    id: inputSCT0
                                    width: parent.width / 2 - 10
                                    text: m_liTemperature0
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true
                                    visible: (selectO2CalibType.currentIndex==0)&&tMainData.bNkeMode
                                }
                                Text {
                                    id: txtSCSpace21
                                    //width: parent.width / 2 - 10
                                    text: " - "
                                    anchors.left: inputSCT0.right
                                    anchors.leftMargin: 0
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    visible: (selectO2CalibType.currentIndex==0)&&tMainData.bNkeMode
                                }
                                TextField {
                                    id: inputSCT1
                                    width: parent.width / 2 - 10
                                    text: m_liTemperature100
                                    renderType: Text.QtRendering
                                    anchors.left: txtSCSpace21.right
                                    anchors.leftMargin: 0
                                    height: 30
                                    activeFocusOnPress: true
                                    selectByMouse: true
                                    enabled: (selectO2CalibType.currentIndex==0)
                                    visible: (selectO2CalibType.currentIndex==0)&&tMainData.bNkeMode
                                }
                            }
                        } // RowLayout Average

                        //--- Ligne ---
                        Rectangle {
                            id: sensorcodeEndLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                    } // Column layout end
                    ButtonPerso {
                        id: btnConfirmSensorCode
                        width: 100
                        height: 28
                        text: qsTr("Submit")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        // Validation du sensor code
                        onClicked: {
                            console.log("Sensor code submit");

                            var ucSensorType = inputSC0.text[0]
                            var ucIntensity = inputSC0.text[1]
                            var ucAmp = inputSC0.text[2]
                            var uliCodeDphi0   = parseInt( inputSC1.text )
                            var uliCodeDphi100 = parseInt( inputSC2.text )
                            var uliTemperature0   = parseInt( inputSCT0.text )
                            var uliTemperature100 = parseInt( inputSCT1.text )

                            // On désactive l'état d'erreur si présent
                            errorO2SensorCode.visible = false
                            // On fait apparaître le busy
                            busyO2SensorCode.visible = true

                            //--------------
                            // Sensor type
                            //--------------
                            // Test d'intégrité
                            if( ucSensorType != "S" ){
                                // On désactive l'état occupé
                                //calibrationID.bBusyState  = false
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC0.show(qsTr("Sensor code must begin with Sxx"))
                                return;
                            }
                            //--------------
                            // Intensity
                            //--------------
                            if(  ( ucIntensity != "A" )
                              && ( ucIntensity != "B" )
                              && ( ucIntensity != "C" )
                              && ( ucIntensity != "D" )
                              && ( ucIntensity != "E" )
                              && ( ucIntensity != "F" )
                              && ( ucIntensity != "G" )
                              && ( ucIntensity != "H" ) ){
                                // On désactive l'état occupé
                                //calibrationID.bBusyState  = false
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC0.show(qsTr("Second character must be between A and H"))
                                return;
                            }
                            //--------------
                            // Amp
                            //--------------
                            if(  ( ucAmp != "4" )
                              && ( ucAmp != "5" )
                              && ( ucAmp != "6" )
                              && ( ucAmp != "7" ) ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC0.show(qsTr("Third character must be between 4 and 7"))
                                return;
                            }
                            //--------------
                            // Dphi0 et Dphi100
                            //--------------
                            var fCodeDphi0          = parseFloat( inputSC1.text );
                            var fCodeDphi0Rounded   = parseFloat(parseInt(inputSC1.text));
                            var fCodeDphi100        = parseFloat( inputSC2.text );
                            var fCodeDphi100Rounded = parseFloat(parseInt(inputSC2.text));
                            // Vérification fonctionnement avec un entier
                            if( fCodeDphi0 !== fCodeDphi0Rounded ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC1.show(qsTr("Value must be an integer"))
                                return;
                            }
                            if( fCodeDphi100 !== fCodeDphi100Rounded ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC2.show(qsTr("Value must be an integer"))
                                return;
                            }
                            if(  ( fCodeDphi0 > 900 )
                              || ( fCodeDphi0 < 0     ) ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC1.show(qsTr("Value must be between 0 and 900"))
                                return;
                            }
                            if(  ( fCodeDphi100 > 900 )
                              || ( fCodeDphi100 < 0     ) ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC2.show(qsTr("Value must be between 0 and 900"))
                                return;
                            }
                            // On adapte les signaux
                            uliCodeDphi0   = uliCodeDphi0   * 100
                            uliCodeDphi100 = uliCodeDphi100 * 100

                            console.log( "== Après validation ==" );

                            console.log( "ucSensorType = "+ucSensorType );
                            console.log( "ucIntensity = "+ucIntensity );
                            console.log( "ucAmp = "+ucAmp );
                            console.log( "uliCodeDphi0 = "+uliCodeDphi0 );
                            console.log( "uliCodeDphi100 = "+uliCodeDphi100 );
                            // Conversion valeur
                            m_ucSensorType   = ucSensorType
                            m_ucIntensity    = ucIntensity
                            m_ucAmp          = ucAmp
                            m_uliCodeDphi0   = uliCodeDphi0
                            m_uliCodeDphi100 = uliCodeDphi100
                            // Calibration factory mode
                            if( selectO2CalibType.currentIndex == 0 ){
                                // Met à jour les paramètres
                                tO2Event.setFactoryModeSensorCode( m_ucSensorType,
                                                                   m_ucIntensity,
                                                                   m_ucAmp,
                                                                   m_uliCodeDphi0,
                                                                   m_uliCodeDphi100 );
                                // Température et pression de calibration
                                tO2Event.setAirCalibrationTemperature( 20000 );
                                tO2Event.setAirCalibrationPressure( 1013000 );
                                tO2Event.set0CalibrationTemperature( 20000 );
                                if( tMainData.bNkeMode ){
                                    tO2Event.setAirCalibrationTemperature( uliTemperature100 );
                                    tO2Event.set0CalibrationTemperature( uliTemperature0 );
                                }
                                //%%AP - 2021.11.19 - N'a pas de sens
                                // Temperature in the sample
                                //tO2Event.setConditionInTheSample( true, parseInt(-300000), 1013000 );
                            }
                            // Calibration 2 points mode
                            else{
                                // Mode deux points
                                tO2Event.set2PointsModeSensorCode( m_ucSensorType,
                                                                   m_ucIntensity,
                                                                   m_ucAmp );
                            }
                            // Envoi de la commande modbus
                            //tO2Event.setSensorSettingsCMD();
                            tO2Event.sendConfToI2CSensor();
                            // Réinit des paramètres du timer
                            timerSubmitSensorCodeConfig.ucState = 0
                            // Démarrage du timer de gestion
                            timerSubmitSensorCodeConfig.start()
                        }
                    }
                    // Timer pour le fonctionnement du submit sensor code
                    Timer {
                        id: timerSubmitSensorCodeConfig
                        interval: 100;
                        running: false;
                        repeat: true;
                        property int ucState: 0
                        onTriggered: {
                            // Tant que le grafcet est en busy
                            if( tMainEvent.isModbusBusyState() ){
                                return;
                            }
                            // Selon l'état courant
                            switch( ucState ){
                                // Fin envoi config I2C
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 0:
                                    // CMD d'envoi de la configuration
                                    tO2Event.sendConfigWriteCMD();
                                    // Incrément de l'état
                                    ucState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 1:
                                    // CMD Save configuration permanently
                                    //tO2Event.saveRegisterPermanently();
                                    // Incrément de l'état
                                    ucState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers capteur pyro
                                //-------------------------
                                case 2:
                                    // CMD read
                                    tO2Event.sendReadCMD();
                                    // Incrément de l'état
                                    ucState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers PC
                                //-------------------------
                                case 3:
                                    // CMD read
                                    tO2Event.readConfFromI2CSensor();
                                    // Incrément de l'état
                                    ucState++;
                                break;
                                // Default
                                default: break;
                            }
                            // Stop du timer
                            timerSubmitSensorCodeConfig.stop();
                            // On désactive l'état d'erreur si présent
                            errorO2SensorCode.visible = false
                            // On fait apparaître le busy
                            busyO2SensorCode.visible = false

                            // Gestion de l'affichage
                            if( selectO2CalibType.currentIndex == 0 )
                             bFactorycalibration = true;
                            else
                             bFactorycalibration = false;
                        }
                    }

                    Rectangle {
                        id: rectangleO2StartCalibrationDummy
                        width: 200
                        height: 28
                        color: "#00ffffff"
                        //Layout.preferredHeight: parent.height
                        //Layout.preferredWidth: ( parent.width - 15 ) / 3
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width / 4 - rectangleO2StartCalibrationDummy.width / 1.5

                        //--------------------
                        // Busy : start calibration
                        //--------------------
                        BusyIndicatorPerso {
                            id: busyO2StartCalibration
                            width: 31
                            height: 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            visible: bBtnStartCalibrationBusy
                        }
                        BlueButtonPerso {
                            id: btnStartCalibration
                            width: 120
                            //height: 28
                            height: 32
                            text: qsTr("Start calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            visible: (!bFactorycalibration) && (!bBtnStartCalibrationBusy)
                            //enabled: (!bFactorycalibration) && (!bBtnStartCalibrationBusy)
                            //onClicked: {
                            onButtonClicked: {
                                // Lancement
                                // Appel de la fonction
                                //vFStartNewCalibration()
                                bSetSensorMsg = true
                                msgDialStartCalibration.open()
                            }
                        }
                    }

/*
                    //! [messagedialog]
                    MessageDialog {
                        id: msgDialSetSensorSettings
                        title: "Warning"
                        text: "2 points calibration selected: sensor code will be reset"
                        standardButtons: StandardButton.Ok | StandardButton.Cancel
                        //StandardButton.Ok: true
                        //StandardButton.Cancel: true
                        //onYes:{}
                        onAccepted:{
                            if( bSetSensorMsg && StandardButton.Ok ){
                                bSetSensorMsg = false
                                console.log("OK");

                                // Met à jour les paramètres
                                tO2Event.setFactoryModeSensorCode( m_ucSensorType,
                                                                   m_ucIntensity,
                                                                   m_ucAmp,
                                                                   530000,
                                                                   200000 );
                                // Envoi de la commande modbus
                                tO2Event.sendConfToI2CSensor();
                                // Réinit des paramètres du timer
                                timerSubmitSensorCodeConfig.ucState = 0
                                // Démarrage du timer de gestion
                                timerSubmitSensorCodeConfig.start()
                            }
                        }
                        onRejected:{
                            if( bSetSensorMsg ){
                                bSetSensorMsg = false
                                console.log("onRejected");
                                // On fait disparaître le busy
                                busyO2Settings.visible = false
                            }
                        }
                    }
*/
                    //--------------------
                    // Busy : calibration settings
                    //--------------------
                    BusyIndicatorPerso {
                        id: busyO2SensorCode
                        width: 31
                        height: 32
                        anchors.left: btnConfirmSensorCode.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                    //--------------------
                    // Error : calibration settings
                    //--------------------
                    Image {
                        id: errorO2SensorCode
                        width: 31
                        height: 31
                        source: "qrc:/Logo/Global/No_Cross.png"
                        anchors.left: btnConfirmSensorCode.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }

                } // Bloc sensor settings


                //--------------------------------------------------
                // Bloc : Calibration
                //--------------------------------------------------
                DropShadow {
                    color: "#20000000"
                    radius: 12
                    verticalOffset: 0
                    anchors.fill: startCalibrationBloc
                    samples: 30
                    horizontalOffset: 0
                    source: startCalibrationBloc
                    visible: (!bFactorycalibration) && 0
                }
                Rectangle {
                    id: startCalibrationBloc
                    x: 1
                    width: o2PartID.width - 120
                    //height: 121
                    height: (bDphiDetailDisplay?160:85)
                    //height: 160
                    color: "#ffffff"
                    anchors.top: conditionInTheSamples.bottom
                    anchors.topMargin: 20
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: (!bFactorycalibration) && 0
                    Rectangle {
                        id: sensorSettings3
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        color: "#e4e3e9"
                        Text {
                            id: sensorSettingsTxt1
                            text: qsTr("Calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.weight: Font.DemiBold
                            font.bold: false
                            font.family: "Open Sans"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutSensorSettings1
                        x: 0
                        width: parent.width
                        height: parent.height - 44
                        anchors.top: sensorSettings3.bottom
                        anchors.topMargin: 4
                        //--------------------------
                        // Calibration state
                        //--------------------------
                        RowLayout {
                            id: rowSC1
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            visible: false
                            Rectangle {
                                id: rowSC3
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgSC1
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtSC1
                                    height: parent.height
                                    text: qsTr("State")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgSC1.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }


                            Rectangle {
                                id: rectangleSC5
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                Text {
                                    id: element
                                    x: 66
                                    y: 8
                                    text: ( m_bCalibrated )?qsTr("Calibrated"):qsTr("Not calibrated")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: 12
                                }
                            }

                            Rectangle {
                                id: rectangleSCDummy1
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                ButtonPerso {
                                    id: btnConfirmSettings3
                                    width: 122
                                    height: 28
                                    text: qsTr("Show point")
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    onClicked: {
                                        popupShowPoint.open()
                                    }
                                }
                                // Pop-up - Rentrée des points
                                Popup {
                                    id: popupShowPoint
                                    anchors.centerIn: parent
                                    width: 120

                                    onClosed: {
                                        // Quitte le popup
                                        //popTestPhyTooltip.hide();
                                        //popTestRawTooltip.hide();
                                    }
                                    enter: Transition {
                                        NumberAnimation { property: "opacity"; from: 0.0; to: 1 }
                                    }
                                    exit: Transition {
                                        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                                    }
                                    ColumnLayout {
                                        width: parent.width
                                        //anchors.fill: parent
                                        //CheckBox { text: qsTr("E-mail") }
                                        //CheckBox { text: qsTr("Calendar") }
                                        //CheckBox { text: qsTr("Contacts") }
                                        Text{
                                            text: qsTr("dphi 0%")
                                            //Layout.alignment: Qt.AlignHCenter
                                            Layout.leftMargin: 20
                                        }
                                        Text{
                                            id: showPointDphi0
                                            text: "--"
                                            //Font.Light: true
                                            font.weight: Font.Light
                                            Layout.leftMargin: 30
                                            //Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text{
                                            text: qsTr("dphi 100%")
                                            Layout.leftMargin: 20
                                            //Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text{
                                            id: showPointDphi100
                                            text: "--"
                                            //Font.Light: true
                                            font.weight: Font.Light
                                            Layout.leftMargin: 30
                                            //Layout.alignment: Qt.AlignHCenter
                                        }
                                        Button{
                                            id: popupSubmitBtn
                                            text: qsTr("Exit")
                                            //anchors.horizontalCenter: parent.horizontalCenter
                                            //Layout.
                                            //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                            Layout.alignment: Qt.AlignHCenter
                                            visible: true
                                            width: parent.width
                                            onClicked: {
                                                // Fermeture de la fenêtre
                                                popupShowPoint.close();
                                            }
                                        }
                                    }
                                }

                            }

                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                        }

                        Rectangle {
                            id: fixedTemperatureLine3
                            width: parent.width
                            visible: false
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }

                        //--------------------------
                        // Dphi100
                        //--------------------------
                        RowLayout {
                            id: rowDphi100Display
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            //visible: false
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                            visible: bDphiDetailDisplay

                            Rectangle {
                                id: rowDphi100Display3
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgDphi100Display1
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtDphi100Display1
                                    height: parent.height
                                    text: qsTr("Dphi 100%")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgDphi100Display1.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleDphi100Display5
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                Text {
                                    id: elementDphi100Display
                                    x: 66
                                    y: 8
                                    text: "530"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: 12
                                }
                            }

                            Rectangle {
                                id: rectangleDphi100DisplayDummy1
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                        }

                        Rectangle {
                            id: fixedDphi100Line3
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                            visible: bDphiDetailDisplay
                        }
                        //--------------------------
                        // Dphi0
                        //--------------------------
                        RowLayout {
                            id: rowDphi0Display
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            //visible: false
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                            visible: bDphiDetailDisplay

                            Rectangle {
                                id: rowDphi0Display3
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgDphi0Display1
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtDphi0Display1
                                    height: parent.height
                                    text: qsTr("Dphi 0%")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgDphi0Display1.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleDphi0Display5
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                Text {
                                    id: elementDphi0Display
                                    x: 66
                                    y: 8
                                    text: "530"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: 12
                                }
                            }

                            Rectangle {
                                id: rectangleDphi0DisplayDummy1
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                        }

                        Rectangle {
                            id: fixedDphi0Line3
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                            visible: bDphiDetailDisplay
                        }

                        //------------------------------
                        // Start calibration button
                        //------------------------------
                        RowLayout {
                            id: rowSC6
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                            Rectangle {
                                id: rowSC7
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgSC3
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtSC3
                                    height: parent.height
                                    text: qsTr("New calibration")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgSC3.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleSC7
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                //--------------------
                                // Busy : start calibration
                                //--------------------
                                BusyIndicatorPerso {
                                    id: busyO2StartCalibration2
                                    width: 31
                                    height: 32
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: false
                                }
                                //--------------------
                                // Error : start calibration
                                //--------------------
                                Image {
                                    id: errorO2StartCalibration
                                    width:  31
                                    height: 31
                                    source: "qrc:/Logo/Global/No_Cross.png"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: false
                                }
                                ButtonPerso {
                                    id: btnStartCalibration2
                                    width: 100
                                    height: 28
                                    text: qsTr("Start")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    onClicked: {
                                        // Lancement
                                        // Appel de la fonction
                                        //vFStartNewCalibration()
                                        bSetSensorMsg = true
                                        msgDialStartCalibration.open()
                                    }
                                }


                                //! [messagedialog]
                                MessageDialogPerso {
                                    id: msgDialStartCalibration
                                    title: qsTr("Warning")
                                    //text:  "Start new calibration: last one will be reset.\r\n"+
                                    //       "Please keep in note previous dphi point if you are not sure."
                                    text:  qsTr("Start new calibration: last one will be reset.")
                                    standardButtons: StandardButton.Ok | StandardButton.Cancel

                                    function vFStartCalibrationAfterResize(){
                                        // Sauvegarde des points précédents en cas de return
                                        m_uliPreviousDphi100 = m_uliCodeDphi100
                                        m_uliPreviousDphi0   = m_uliCodeDphi0
                                        m_uliPreviousTemperature100 = m_liTemperature100
                                        m_uliPreviousTemperature0   = m_liTemperature0

                                        // Remplacement des valeurs dphi0 et dphi100
                                        tO2Event.setCalibPointManually( 53000, 20000 )
                                        tO2Event.setAirCalibrationTemperature( parseInt(20000) );
                                        tO2Event.set0CalibrationTemperature( parseInt(20000) );
                                        // Envoi de la première commande modbus
                                        tO2Event.sendConfToI2CSensor()
                                        // Réinit des paramètres du timer
                                        timerSubmitStartCalibration.ucState = 0
                                        // Démarrage du timer de gestion
                                        timerSubmitStartCalibration.start()
                                    }
                                    onAccepted:{
                                        if( bSetSensorMsg && StandardButton.Ok ){
                                            bSetSensorMsg = false
                                            console.log("OK");
                                            //busyO2StartCalibration.visible = true
                                            //btnStartCalibration.visible    = false
                                            bBtnStartCalibrationBusy = true
                                            // Appel de la fonction
                                            vFStartNewCalibration()
                                        }
                                    }
                                    onRejected:{
                                        if( bSetSensorMsg ){
                                            bSetSensorMsg = false
                                            console.log("onRejected");
                                            // On fait disparaître le busy
                                            busyO2Settings.visible = false
                                        }
                                    }
                                }
                                // Timer pour le fonctionnement du start calibration
                                Timer {
                                    id: timerSubmitStartCalibration
                                    interval: 100;
                                    running: false;
                                    repeat: true;
                                    property int ucState: 0
                                    onTriggered: {
                                        // Tant que le grafcet est en busy
                                        if( tMainEvent.isModbusBusyState() ){
                                            return;
                                        }
                                        // Selon l'état courant
                                        switch( ucState ){
                                            // Fin envoi config I2C
                                            //-------------------------
                                            // CMD Envoi configuration vers capteur pyro
                                            //-------------------------
                                            case 0:
                                                // CMD d'envoi de la configuration
                                                tO2Event.sendConfigWriteCMD();
                                                // Incrément de l'état
                                                ucState++;
                                            // Quitte
                                            return;
                                            //-------------------------
                                            // CMD Lecture configuration vers capteur pyro
                                            //-------------------------
                                            case 1:
                                                // CMD read
                                                tO2Event.sendReadCMD();
                                                // Incrément de l'état
                                                ucState++;
                                            return;
                                            //-------------------------
                                            // CMD Lecture configuration vers PC
                                            //-------------------------
                                            case 2:
                                                // CMD read
                                                tO2Event.readConfFromI2CSensor();
                                                // Incrément de l'état
                                                //ucState++;
                                            break;
                                            //-------------------------
                                            // CMD Sauvegarde permanente de la configuration
                                            //-------------------------
                                            //case 3:
                                            //    // CMD Save configuration permanently
                                            //    tO2Event.saveRegisterPermanently();
                                            //    // Incrément de l'état
                                            //   // ucState++;
                                            //break;
                                            //-------------------------
                                            // Default
                                            //-------------------------
                                            default: break;
                                        }
                                        // Stop du timer
                                        timerSubmitStartCalibration.stop();
                                        // On fait disparaitre le busy
                                        //busyO2StartCalibration.visible  = false
                                        // On désactive l'état d'erreur si présent
                                        errorO2StartCalibration.visible = false
                                        // Réactivation du bouton start calibration
                                        //btnStartCalibration.visible     = true
                                        bBtnStartCalibrationBusy = false
                                        // Affichage du graphique
                                        vFStartO2CalibrationLiveview_MAIN();
                                        //%%AP - 2022.07.25 - Test
                                        //ch0Enable.checked
                                    }
                                }
                            }

                            //--------------------------------
                            // Set point manually bloc
                            //--------------------------------
                            Rectangle {
                                id: rectangleSCDummy3
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                //visible: bDphiDetailDisplay

                                ButtonPerso {
                                    id: btnSetPointManually
                                    width: 122
                                    height: 28
                                    text: qsTr("Set point manually")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: bDphiDetailDisplay
                                    onClicked: {
                                        // Ouverture du popup
                                        popupSetPoint.open()
                                    }
                                }

                                // Pop-up - Rentrée des points
                                Popup {
                                    id: popupSetPoint
                                    anchors.centerIn: parent
                                    width: 120

                                    onClosed: {
                                        // Quitte le popup
                                        //popTestPhyTooltip.hide();
                                        //popTestRawTooltip.hide();
                                    }
                                    enter: Transition {
                                        NumberAnimation { property: "opacity"; from: 0.0; to: 1 }
                                    }
                                    exit: Transition {
                                        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                                    }
                                    ColumnLayout {
                                        width: parent.width
                                        Text{
                                            text: qsTr("dphi 0%")
                                            //Layout.alignment: Qt.AlignHCenter
                                            //Layout.leftMargin: 20
                                        }
                                        TextField{
                                            id: setPointDphi0
                                            placeholderText: "--"
                                            //Font.Light: true
                                            font.weight: Font.Light
                                            Layout.preferredWidth : parent.width
                                            Layout.alignment: Qt.AlignHCenter
                                            selectByMouse: true

                                            Keys.onEnterPressed:{
                                                console.log( "Press enter" );
                                                popupSetPointSubmitBtn.clicked()
                                                //popupSetPoint.click()
                                            }
                                            Keys.onReturnPressed:{
                                                console.log( "Press onReturnPressed" );
                                                popupSetPointSubmitBtn.click();
                                            }
                                            ToolTip {
                                                id: popupSetPointDphi0
                                                visible: false
                                                text: qsTr("")
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
                                        Text{
                                            text: qsTr("dphi 100%")
                                            //Layout.leftMargin: 20
                                            //Layout.alignment: Qt.AlignHCenter
                                        }
                                        TextField{
                                            id: setPointDphi100
                                            placeholderText: "--"
                                            //Font.Light: true
                                            font.weight: Font.Light
                                            Layout.preferredWidth :  parent.width
                                            Layout.alignment: Qt.AlignHCenter
                                            selectByMouse: true

                                            Keys.onEnterPressed:{
                                                console.log( "Press enter" );
                                                popupSetPoint.clicked();
                                            }
                                            Keys.onReturnPressed:{
                                                console.log( "Press onReturnPressed" );
                                                popupSetPoint.clicked();
                                            }
                                            ToolTip {
                                                id: popupSetPointDphi100
                                                visible: false
                                                text: qsTr("")
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
                                        Button{
                                            id: popupSetPointSubmitBtn
                                            text: qsTr("Submit")
                                            //anchors.horizontalCenter: parent.horizontalCenter
                                            //Layout.
                                            //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                            Layout.alignment: Qt.AlignHCenter
                                            visible: true
                                            width: parent.width
                                            onClicked: {
                                                // Busy
                                                popupSetPointSubmitBtn.visible = false
                                                busyO2SetPoints.visible  = true
                                                errorO2SetPoints.visible = false
                                                // Déclaration des variables
                                                var fDphi0   = parseFloat( setPointDphi0.text );
                                                var fDphi100 = parseFloat( setPointDphi100.text );
                                                // Pour vérification du nombre de chiffre après la virgule
                                                var fDphi0Rounded   = parseFloat(parseInt(fDphi0 * 1000))/1000;
                                                var fDphi100Rounded = parseFloat(parseInt(fDphi100 * 1000))/1000;
                                                // Vérification des paramètres

                                                // fDphi0 : Test si max step est bien à 0.001
                                                if( fDphi0 !== fDphi0Rounded )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi0.show(qsTr("Please enter a value with a maximum of 3 decimal places"))
                                                    return;
                                                 }
                                                // Phy : Test si max step est bien à 0.001
                                                if(  ( fDphi0 > 90000 )
                                                  || ( fDphi0 < 0 ) )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi0.show(qsTr("Value must be between 0 and 90000"))
                                                    return;
                                                 }
                                                // Raw : Test si max step est bien à 0.1
                                                if( fDphi100 !== fDphi100Rounded )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi100.show(qsTr("Please enter a value with a maximum of 1 decimal places"))
                                                    return;
                                                 }
                                                // Raw : Test si max step est bien à 0.001
                                                if(  ( fDphi100 > 90000 )
                                                  || ( fDphi100 < 0 ) )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi100.show(qsTr("Value must be between 0 and 90000"))
                                                    return;
                                                 }

                                                // Déclaration des variables
                                                var uliDphi0   = parseInt( fDphi0 * 1000 );
                                                var uliDphi100 = parseInt( fDphi100 * 1000 );

                                                console.log( "uliDphi0 = "+uliDphi0 );
                                                console.log( "uliDphi100 = "+uliDphi100 );

                                                // Enregistrement des paramètres
                                                tO2Event.setCalibPointManually( uliDphi0, uliDphi100 );
                                                // Envoi de la première commande modbus
                                                tO2Event.sendConfToI2CSensor()
                                                // Réinit des paramètres du timer
                                                timerSubmitSetPointManually.ucState = 0
                                                // Démarrage du timer de gestion
                                                timerSubmitSetPointManually.start()
                                            }
                                        }
                                        // Timer pour le fonctionnement du start calibration
                                        Timer {
                                            id: timerSubmitSetPointManually
                                            interval: 100;
                                            running: false;
                                            repeat: true;
                                            property int ucState: 0
                                            onTriggered: {
                                                // Tant que le grafcet est en busy
                                                if( tMainEvent.isModbusBusyState() ){
                                                    return;
                                                }
                                                // Selon l'état courant
                                                switch( ucState ){
                                                    // Fin envoi config I2C
                                                    //-------------------------
                                                    // CMD Envoi configuration vers capteur pyro
                                                    //-------------------------
                                                    case 0:
                                                        // CMD d'envoi de la configuration
                                                        tO2Event.sendConfigWriteCMD();
                                                        // Incrément de l'état
                                                        ucState++;
                                                    // Quitte
                                                    return;
                                                    //-------------------------
                                                    // CMD Envoi configuration vers capteur pyro
                                                    //-------------------------
                                                    case 1:
                                                        // CMD Save configuration permanently
                                                        tO2Event.saveRegisterPermanently();
                                                        // Incrément de l'état
                                                        ucState++;
                                                    return;
                                                    //-------------------------
                                                    // CMD Lecture configuration vers capteur pyro
                                                    //-------------------------
                                                    case 2:
                                                        // CMD read
                                                        tO2Event.sendReadCMD();
                                                        // Incrément de l'état
                                                        ucState++;
                                                    return;
                                                    //-------------------------
                                                    // CMD Lecture configuration vers PC
                                                    //-------------------------
                                                    case 3:
                                                        // CMD read
                                                        tO2Event.readConfFromI2CSensor();
                                                        // Incrément de l'état
                                                        ucState++;
                                                    break;
                                                    //-------------------------
                                                    // Default
                                                    //-------------------------
                                                    default:
                                                        // Stop du timer et du grafcet
                                                        timerSubmitSetPointManually.stop();
                                                        console.log("Error dans la gestion du grafcet");
                                                    break;
                                                }
                                                // Stop du timer
                                                timerSubmitSetPointManually.stop();
                                                // Fermeture de la fenêtre
                                                popupShowPoint.close();
                                                // Busy
                                                popupSetPointSubmitBtn.visible = true
                                                busyO2SetPoints.visible = false

                                                // Désactivation Air calibration
                                                //airCalibrationBloc.enabled = false
                                                // Activation calibration 0%
                                                //zeroCalib.enabled = true
                                            }
                                        }

                                        //--------------------
                                        // Busy : calibration points
                                        //--------------------
                                        BusyIndicatorPerso {
                                            id: busyO2SetPoints
                                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                            Layout.preferredHeight: 31
                                            Layout.preferredWidth: 31
                                            visible: false
                                        }
                                        //--------------------
                                        // Error : calibration points
                                        //--------------------
                                        Image {
                                            id: errorO2SetPoints
                                            source: "qrc:/Logo/Global/No_Cross.png"
                                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                            Layout.preferredHeight: 31
                                            Layout.preferredWidth: 31
                                            visible: false
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //=========================================================
            // SECOND CALIB SWIPE : Set air calibration
            //=========================================================
            Item{
                id: secondCalibSwipe
                //width: parent.width
                //--------------------------------------------------
                // Bloc : Air calibration
                //--------------------------------------------------
                DropShadow {
                    color: "#20000000"
                    radius: 12
                    verticalOffset: 0
                    anchors.fill: airCalibrationBloc
                    samples: 30
                    horizontalOffset: 0
                    source: airCalibrationBloc
                }
                Rectangle {
                    id: airCalibrationBloc
                    width: o2PartID.width - 120
                    //height: 440
                    //height: 250
                    //height: 240
                    //height: 280
                    //height: 270
                    //height: 310
                    //height: 300
                    height: 250
                    color: "#ffffff"
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    enabled: false

                    Rectangle {
                        id: cits3
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        //color: "#193d8a"
                        color: "#e4e3e9"
                        Text {
                            id: citsTxt1
                            //color: "#ffffff"
                            color: "#000000"
                            font.weight: Font.Bold
                            text: qsTr("100% calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.bold: false
                            font.family: "Open Sans"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: (airCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutCits1
                        width: parent.width
                        //height: 350
                        //height: airCalibrationBloc.height - cits3.height - btnConfirmSetAir.height - btnConfirmSetAir.bottom - 8

                        anchors.top: cits3.bottom
                        //anchors.topMargin: 10
                        anchors.topMargin: 8
                        anchors.bottom: btnConfirmSetAir.top
                        anchors.bottomMargin: 8

                        // 1 - 100% point calibration condition
                        Rectangle {
                            id: cits4
                            //height: 42
                            height: 30
                            transformOrigin: Item.Center
                            clip: false
                            Layout.preferredWidth: airCalibrationBloc.width

                            ColumnLayout {
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8
                                width: parent.width - 40

                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 20

                                Text {
                                    text: qsTr("1 - Immerse the sensor into air-saturated water")
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    opacity: (airCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                                }
                            }
                        }

                        Rectangle {
                            id: fixedPressureLine1
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: airCalibrationBloc.width
                        }
                        //-------------------------------------------
                        // Air calib - Fixed pressure
                        //-------------------------------------------
                        Rectangle {
                            id: rowFixedPressure4
                            //height: 50
                            height: 30
                            transformOrigin: Item.Center
                            clip: false
                            Layout.preferredWidth: airCalibrationBloc.width

                            ColumnLayout {
                                id: airCalibTextBlocRect
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8
                                //width: parent.width - 40
                                width: parent.width * 2 / 3 - 20

                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.right: parent.right
                                //anchors.rightMargin: 20
                                anchors.rightMargin: 20 + parent.width / 3

                                Text {
                                    //y: 10
                                    text: qsTr("2 - Set current barometric pressure")
                                    width: parent.width
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.fillWidth: true
                                    //anchors.left: parent.left
                                    //anchors.right: parent.right
                                    opacity: (airCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                                }
                                /*
                                Text {
                                    id: airCalibExplanationText
                                    text: qsTr("Calibration 100% point is determined for a fixed athmospheric pressure entered by the user.")
                                    //anchors.leftMargin: 20
                                    font.pixelSize: 11
                                    styleColor: "#545454"
                                    opacity: (airCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                                    wrapMode: Text.WordWrap
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.fillWidth: true
                                    //anchors.left: parent.left
                                    //anchors.right: parent.right
                                }
                                */
                            }
                            TextField {
                                id: inputFixedPressure100
                                width: parent.width / 3 - 20
                                //width: parent.width
                                height: 30
                                text: qsTr("")
                                anchors.right: parent.right
                                anchors.left: airCalibTextBlocRect.right
                                anchors.rightMargin: 20
                                anchors.leftMargin: 20
                                //anchors.bottom: airCalibTextBlocRect.bottom
                                anchors.verticalCenter: parent.verticalCenter
                                selectByMouse: true
                                activeFocusOnPress: true
                                ToolTip {
                                    id: tooltipFixedPressure100
                                    text: qsTr("InvalidValue")
                                    font.weight: Font.Light
                                    exit: Transition {
                                        NumberAnimation {
                                            property: "opacity"
                                            from: 1
                                            to: 0
                                        }
                                    }
                                    visible: false
                                    background: Rectangle {
                                        color: "white"
                                        border.color: "#555555"
                                    }
                                    font.family: "Open Sans"
                                    opacity: 0.95
                                    enter: Transition {
                                        NumberAnimation {
                                            property: "opacity"
                                            from: 0
                                            to: 1
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: cits31Line
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: airCalibrationBloc.width
                        }

                        // 3 - Wait for steady condition in the graphical part
                        Rectangle {
                            id: cits31
                            //height: 34
                            height: 30
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            Layout.preferredWidth: airCalibrationBloc.width
                            Text {
                                id: text3
                                text: qsTr("3 - Reach a steady-state condition")
                                //text: qsTr("2 - Wait for steady condition in the graphical part")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                opacity: (airCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                            }
                        }
                        Rectangle {
                            id: cits411Line
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: airCalibrationBloc.width
                        }
                        // 4 - Press below button to calibrate
                        Rectangle {
                            id: cits411
                            //height: 34
                            height: 30
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            Layout.preferredWidth: airCalibrationBloc.width
                            Text {
                                id: text41
                                text: qsTr("4 - Press \"Set 100%\" button to calibrate")
                                //text: qsTr("3 - Press below button to calibrate")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                opacity: (airCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                            }
                        }

                        Rectangle {
                            id: btnConfirmSetAirLine
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: airCalibrationBloc.width
                        }
                    }
                    ButtonPerso {
                        width: 100
                        height: 28
                        text: qsTr("Return")
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        anchors.leftMargin: 20
                        onClicked: {
                            // Retour de la calibration
                            vFReturnFromAirCalibration();
                        }
                    }
                    ButtonPerso {
                        id: btnConfirmSetAir
                        width: 100
                        height: 28
                        text: qsTr("Set 100%")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8

                        onClicked: {
                            console.log("Air calibration submit");
                            vFLockUserClick();

                            // On fait apparaître le busy
                            busyO2SetAir.visible  = true
                            // On désactive l'état d'erreur si présent
                            errorO2SetAir.visible = false

                            var uliPressure100   = parseInt( inputFixedPressure100.text )
                            console.log( "uliPressure100 = "+uliPressure100 );

                            //-------------------------
                            // Fixed pressure
                            //-------------------------
                            var fFixedPressure        = parseFloat( inputFixedPressure100.text );
                            var fFixedPressureRounded = parseFloat(parseInt(fFixedPressure*1000))/1000;
                            // Vérification fonctionnement avec un entier
                            if( fFixedPressure !== fFixedPressureRounded ){
                                // On désactive l'état occupé
                                busyO2SetAir.visible  = false
                                errorO2SetAir.visible = true
                                tooltipFixedPressure100.show(qsTr("Please enter a value with a maximum of 3 decimal places"))
                                vFAllowUserClick()
                                return;
                            }
                            if(  ( fFixedPressure > 10000 )
                              || ( fFixedPressure < 0.001 ) ){
                                // On désactive l'état occupé
                                busyO2SetAir.visible  = false
                                errorO2SetAir.visible = true
                                tooltipFixedPressure100.show(qsTr("Value must be between 0.001 and 10000"))
                                vFAllowUserClick()
                                return;
                            }
                            uliPressure100   = parseInt( fFixedPressure * 1000 )

                            console.log( "== Après validation ==" );

                            //console.log( "bExternalTemperature100 = "+bExternalTemperature100 );
                            //console.log( "liTemperature100 = "+liTemperature100 );
                            console.log( "uliPressure100 = "+uliPressure100 );

                            // Sauvegarde des paramètres et déclenchement de la commande Set air
                            tO2Event.setAirCalibrationPressure( uliPressure100 );

                            // Arrêt de la mesure
                            vFMAINStopMeasure();
                            // Start timer
                            console.log("airCalibrationTimer.start();");
                            //timerAirCalibration.interval = 1000
                            timerAirCalibration.uiState = 0
                            timerAirCalibration.start();
                            setAirDialogLabel.text = qsTr("Stopping measurement...");
                            setAirDialog.open();
                        }

                        Timer {
                            id: timerAirCalibration
                            //interval: function(){ return( uiMeasurePeriod * 1000 ) }; running: true; repeat: true
                            interval: 100; running: false; repeat: true
                            property int uiState: 0
                            property int uiCompteurSecondLast: 0
                            onTriggered: {
                                // Tant que le grafcet est en busy
                                if( tMainEvent.isModbusBusyState() ){
                                    return;
                                }
                                // Selon l'état courant
                                switch( uiState ){
                                    // Fin d'arrêt de la mesure
                                    //-------------------------
                                    // CMD Transfert configuration vers interface capteur
                                    //-------------------------
                                    case 0:
                                        console.log("Valeur de temperature de calibration: "+liveviewID.fCh2Data);
                                        // Valeur de température
                                        tO2Event.setAirCalibrationTemperature( parseInt(liveviewID.fCh2Data * 1000) );
                                        // Envoi de la première commande modbus
                                        tO2Event.sendConfToI2CSensor();
                                        // Incrément de l'état
                                        uiState++;
                                    // Quitte
                                    return;

                                    //-------------------------
                                    // CMD Envoi configuration vers capteur pyro
                                    //-------------------------
                                    case 1:
                                        // CMD d'envoi de la configuration
                                        tO2Event.sendConfigWriteCMD();
                                        // Incrément de l'état
                                        uiState++;
                                    // Quitte
                                    return;
                                    //-------------------------
                                    // CMD Set Air calibration CMD
                                    //-------------------------
                                    case 2:
                                        timerAirCalibration.interval = 100
                                        // CMD d'envoi de la configuration
                                        tO2Event.setAirCalibrationCMD();
                                        // Modification du délais
                                        uiCompteurSecondLast = 65;
                                        // Modification du texte de la fenêtre
                                        setAirDialogLabel.text = qsTr("Set air command occuring...")
                                        // Incrément de l'état
                                        uiState++;
                                    // Quitte
                                    return;
                                    //-------------------------
                                    // Attente fin set air
                                    //-------------------------
                                    case 3:
                                        // Tant que délais CHI pas fini
                                        if( uiCompteurSecondLast > 0 ){
                                            // Décrément du délais
                                            uiCompteurSecondLast--;
                                            // Modification du texte de la fenêtre
                                            setAirDialogLabel.text = qsTr("Set air command occuring...")+"\r\n"+
                                            (parseFloat(uiCompteurSecondLast)/10)+
                                            qsTr(" seconds left.");
                                            return;
                                        }
                                        // On augmente l'interval
                                        timerAirCalibration.interval = 500
                                        // Incrément de l'état
                                        uiState++;
                                    // Quitte
                                    return;
                                    //-------------------------
                                    // Read windows + delais
                                    //-------------------------
                                    case 4:
                                        // Modification du texte de la fenêtre
                                        setAirDialogLabel.text = qsTr("Reading new configuration...")+"\r\n";
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // CMD Lecture configuration vers capteur pyro
                                    //-------------------------
                                    case 5:
                                        // CMD read
                                        tO2Event.sendReadCMD();
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // CMD Lecture configuration vers PC
                                    //-------------------------
                                    case 6:
                                        // CMD read
                                        tO2Event.readConfFromI2CSensor();
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // CMD Envoi configuration vers capteur pyro
                                    //-------------------------
                                    case 7:
                                        //%%AP - 2021.02.12 - Enregistrement ici?
                                        // CMD Save configuration permanently
                                        //tO2Event.saveRegisterPermanently();
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // Fin grafcet
                                    //-------------------------
                                    case 8:
                                        // CMD read
                                        //tO2Event.readConfFromI2CSensor();
                                        // Stop du timer et du grafcet
                                        timerAirCalibration.stop();
                                        // Incrément de l'état
                                        //uiState++;
                                    break;

                                    //-------------------------
                                    // Default
                                    //-------------------------
                                    default:
                                        // Stop du timer et du grafcet
                                        timerAirCalibration.stop();
                                        console.log("Error dans la gestion du grafcet");
                                    break;
                                }


                                console.log("QML End of Air calibration grafcet");
                                // Relance de la mesure
                                vFMAINStartMeasure();
                                // Changement onglet
                                vFExitAirCalibration_GoTo0PCalibration();
                                // On fait disparaitre le busy
                                busyO2SetAir.visible = false;
                                // Désactivation calibration set air
                                airCalibrationBloc.enabled = false;
                                // Activation calibration 0%
                                zeroCalib.enabled = true;
                                // Fermeture
                                setAirDialog.close();
                                // Autorisation du click
                                vFAllowUserClick();
                            }
                        }
                    }
                    Dialog {
                        id: setAirDialog
                        title: qsTr("Set air")
                        standardButtons: Dialog.NoButton

                        Label {
                            id: setAirDialogLabel
                            text: qsTr("Stopping measurement...")
                        }
                    }
                    //--------------------
                    // Busy : calibration settings
                    //--------------------
                    BusyIndicatorPerso {
                        id: busyO2SetAir
                        width: 31
                        height: 32
                        anchors.left: btnConfirmSetAir.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                    //--------------------
                    // Error : calibration settings
                    //--------------------
                    Image {
                        id: errorO2SetAir
                        width: 31
                        height: 31
                        source: "qrc:/Logo/Global/No_Cross.png"
                        anchors.left: btnConfirmSetAir.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }

                    ButtonPerso {
                        id: btnSkipSetAir
                        width: 100
                        height: 28
                        text: qsTr("Skip")
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        anchors.rightMargin: 20
                        onClicked: {
                            // Skip de la calibration en 100
                            //vFExitAirCalibration_GoTo0PCalibration();
                            // Restauration
                            // Remplacement des valeurs dphi0 et dphi100
                            //tO2Event.setCalibPointManually( 53000, m_uliPreviousDphi100 )
                            tO2Event.setCalibPointAirManually( m_uliPreviousDphi100 )
                            tO2Event.setAirCalibrationTemperature( parseInt(m_uliPreviousTemperature100) );
                            //tO2Event.set0CalibrationTemperature( parseInt(m_uliPreviousTemperature0) );
                            // Envoi de la première commande modbus
                            //tO2Event.sendConfToI2CSensor()
                            // Arrêt de la mesure
                            vFMAINStopMeasure();
                            //
                            timerSkipAirCalibration.uiState = 0
                            timerSkipAirCalibration.start();
                            skipAirDialogLabel.text = qsTr("Stopping measurement...");
                            skipAirDialog.open();
                            busyO2SkipAir.visible = true;
                        }
                    }
                    Timer {
                        id: timerSkipAirCalibration
                        //interval: function(){ return( uiMeasurePeriod * 1000 ) }; running: true; repeat: true
                        interval: 100; running: false; repeat: true
                        property int uiState: 0
                        property int uiCompteurSecondLast: 0
                        onTriggered: {
                            // Tant que le grafcet est en busy
                            if( tMainEvent.isModbusBusyState() ){
                                return;
                            }
                            // Selon l'état courant
                            switch( uiState ){
                                // Fin d'arrêt de la mesure
                                //-------------------------
                                // CMD Transfert configuration vers interface capteur
                                //-------------------------
                                case 0:
                                    // Remplacement des valeurs dphi0 et dphi100
                                    tO2Event.setCalibPointAirManually( m_uliPreviousDphi100 )
                                    tO2Event.setAirCalibrationTemperature( parseInt(m_uliPreviousTemperature100) );
                                    // Envoi de la première commande modbus
                                    tO2Event.sendConfToI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 1:
                                    // CMD d'envoi de la configuration
                                    tO2Event.sendConfigWriteCMD();
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // Read windows + delais
                                //-------------------------
                                case 2:
                                    // Modification du texte de la fenêtre
                                    skipAirDialogLabel.text = qsTr("Reading new configuration...")+"\r\n";
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers capteur pyro
                                //-------------------------
                                case 3:
                                    // CMD read
                                    tO2Event.sendReadCMD();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers PC
                                //-------------------------
                                case 4:
                                    // CMD read
                                    tO2Event.readConfFromI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // Fin grafcet
                                //-------------------------
                                case 5:
                                    // Stop du timer et du grafcet
                                    timerSkipAirCalibration.stop();
                                    // Incrément de l'état
                                    //uiState++;
                                break;
                                //-------------------------
                                // Default
                                //-------------------------
                                default:
                                    // Stop du timer et du grafcet
                                    timerSkipAirCalibration.stop();
                                    console.log("Error dans la gestion du grafcet");
                                break;
                            }
                            console.log("QML End of skip Air calibration grafcet");
                            // Relance de la mesure
                            vFMAINStartMeasure();
                            // Changement onglet
                            vFExitAirCalibration_GoTo0PCalibration();
                            // On fait disparaitre le busy
                            busyO2SkipAir.visible = false;
                            // Désactivation calibration set air
                            airCalibrationBloc.enabled = false;
                            // Activation calibration 0%
                            zeroCalib.enabled = true;
                            // Fermeture
                            skipAirDialog.close();
                            // Autorisation du click
                            vFAllowUserClick();
                        }
                    }
                    Dialog {
                        id: skipAirDialog
                        title: qsTr("Set air")
                        standardButtons: Dialog.NoButton

                        Label {
                            id: skipAirDialogLabel
                            text: qsTr("Stopping measurement...")
                        }
                    }
                    //--------------------
                    // Busy : calibration settings
                    //--------------------
                    BusyIndicatorPerso {
                        id: busyO2SkipAir
                        width: 31
                        height: 32
                        anchors.right: btnSkipSetAir.left
                        anchors.rightMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                }

                //--------------------------------------------------
                // Bloc : 0% calibration
                //--------------------------------------------------
                DropShadow {
                    color: "#20000000"
                    radius: 12
                    verticalOffset: 0
                    anchors.fill: zeroCalib
                    samples: 30
                    horizontalOffset: 0
                    source: zeroCalib
                }
                Rectangle {
                    //id: zeroPCalib2
                    id: zeroCalib
                    enabled: false
                    x: -18
                    width: o2PartID.width - 120
                    //height: 410
                    //height: 250
                    //height: 240
                    height: 220
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: airCalibrationBloc.bottom
                    anchors.topMargin: 20
                    anchors.horizontalCenterOffset: 0

                    Rectangle {
                        id: zeroPCalib4
                        x: 0
                        y: 0
                        width: parent.width
                        //height: 34
                        height: 34
                        //color: "#193d8a"
                        color: "#e4e3e9"
                        Text {
                            id: zeroPCalibTxt2
                            //color: "#ffffff"
                            color: "#000000"
                            font.weight: Font.Bold
                            text: qsTr("0% calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.bold: false
                            font.family: "Open Sans"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: (zeroCalib.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutZeroPCalib2
                        width: parent.width

                        anchors.top: zeroPCalib4.bottom
                        anchors.topMargin: 4
                        anchors.bottom: btnConfirmZeroPCalib.top
                        anchors.bottomMargin: 8

                        //%%AP - 2021.11.19 - Modification
                        //anchors.left: parent.left
                        //anchors.right: parent.right

                        // 1 - 0% point calibration condition
                        Rectangle {
                            id: rect0p
                            //width: parent.width
                            //height: 55
                            //height: 40
                            height: 30
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            Layout.preferredWidth: zeroCalib.width

                            ColumnLayout {
                                anchors.verticalCenter: parent.verticalCenter
                                //anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.rightMargin: 20
                                spacing: 8

                                anchors.left: parent.left
                                anchors.right: parent.right

                                Text {
                                    id: text15
                                    //y: 10
                                    //text: qsTr("1 - 0% point calibration condition")
                                    text: qsTr("1 - Immerse the sensor into anoxic water")
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    opacity: (zeroCalib.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                                    verticalAlignment: Text.AlignVCenter
                                    Layout.fillWidth: true
                                    wrapMode: Text.WordWrap
                                }
                            }

                        }
                        Rectangle {
                            id: cits315Line
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: zeroCalib.width
                        }
                        // 2 - Wait for steady condition in the graphical part
                        Rectangle {
                            id: cits315
                            //height: 34
                            height: 30
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            //width: parent.width
                            Layout.preferredWidth: zeroCalib.width
                            Text {
                                id: text35
                                text: qsTr("2 - Reach a steady-state condition")
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                opacity: (zeroCalib.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                            }
                        }
                        Rectangle {
                            id: cits4115Line
                            //width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: zeroCalib.width
                        }

                        // 3 - Press below button to calibrate
                        Rectangle {
                            id: cits4115
                            //height: 34
                            height: 30
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            Layout.preferredWidth: zeroCalib.width
                            Text {
                                id: text415
                                text: qsTr("3 - Press \"Set 0%\" button to calibrate")
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                opacity: (zeroCalib.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                            }
                        }
                        Rectangle {
                            id: btnConfirmZeroPCalibLine
                            height: 1
                            color: "#62888888"
                            Layout.preferredWidth: zeroCalib.width
                        }
                    }

                    ButtonPerso {
                        width: 100
                        height: 28
                        text: qsTr("Return")
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        onClicked: {
                            vFReturnFrom0PCalibration();
                        }
                    }

                    ButtonPerso {
                        id: btnConfirmZeroPCalib
                        width: 100
                        height: 28
                        text: qsTr("Set 0%")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        onClicked: {
                            console.log("0% calibration submit");

                            // On fait apparaître le busy
                            busyConfigZeroPCalib.visible  = true
                            // On désactive l'état d'erreur si présent
                            errorConfigZeroPCalib.visible = false

                            // Arrêt de la mesure
                            vFMAINStopMeasure();
                            // Start timer
                            console.log("zeroPCalibrationTimer.start();");
                            //zeroPCalibrationTimer.interval = 1000
                            zeroPCalibrationTimer.uiState = 0
                            zeroPCalibrationTimer.start()
                            set0DialogLabel.text = qsTr("Stopping measurement...");
                            set0Dialog.open();
                        }
                    }

                    Timer {
                        id: zeroPCalibrationTimer
                        interval: 100; running: false; repeat: true
                        property int uiState: 0
                        property int uiCompteurSecondLast: 0
                        onTriggered: {
                            // Tant que le grafcet est en busy
                            if( tMainEvent.isModbusBusyState() ){
                                return;
                            }
                            // Selon l'état courant
                            switch( uiState ){
                                // Fin d'arrêt de la mesure
                                //-------------------------
                                // CMD Transfert configuration vers interface capteur
                                //-------------------------
                                case 0:
                                    console.log("Valeur de temperature de calibration: "+liveviewID.fCh2Data);
                                    // Valeur de température
                                    tO2Event.set0CalibrationTemperature( parseInt(liveviewID.fCh2Data * 1000) );
                                    // Envoi de la première commande modbus
                                    tO2Event.sendConfToI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;

                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 1:
                                    // CMD d'envoi de la configuration
                                    tO2Event.sendConfigWriteCMD();
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Set 0% calibration CMD
                                //-------------------------
                                case 2:
                                    zeroPCalibrationTimer.interval = 100
                                    // CMD d'envoi de la configuration
                                    tO2Event.set0PCalibrationCMD();
                                    // Modification du délais
                                    uiCompteurSecondLast = 65; //60;
                                    // Modification du texte de la fenêtre
                                    set0DialogLabel.text = qsTr("Set 0% command occuring...");
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // Attente fin set 0%
                                //-------------------------
                                case 3:
                                    // Tant que délais CLO pas fini
                                    if( uiCompteurSecondLast > 0 ){
                                        // Décrément du délais
                                        uiCompteurSecondLast--;
                                        // Modification du texte de la fenêtre
                                        set0DialogLabel.text = qsTr("Set 0% command occuring...\r\n")+
                                        (parseFloat(uiCompteurSecondLast)/10)+
                                        qsTr(" seconds left.");
                                        return;
                                    }
                                    // On augmente l'interval
                                    zeroPCalibrationTimer.interval = 500
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // Read windows + delais
                                //-------------------------
                                case 4:
                                    // Modification du texte de la fenêtre
                                    set0DialogLabel.text = qsTr("Reading new configuration...")+"\r\n";
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers capteur pyro
                                //-------------------------
                                case 5:
                                    // CMD read
                                    tO2Event.sendReadCMD();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers PC
                                //-------------------------
                                case 6:
                                    // CMD read
                                    tO2Event.readConfFromI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 7:
                                    //%%AP - 2021.02.12 - Enregistrement ici?
                                    // CMD Save configuration permanently
                                    //tO2Event.saveRegisterPermanently();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // Fin grafcet
                                //-------------------------
                                case 8:
                                    // CMD read
                                    //tO2Event.readConfFromI2CSensor();
                                    // Stop du timer et du grafcet
                                    zeroPCalibrationTimer.stop();
                                    // Incrément de l'état
                                    //uiState++;
                                break;

                                //-------------------------
                                // Default
                                //-------------------------
                                default:
                                    // Stop du timer et du grafcet
                                    zeroPCalibrationTimer.stop();
                                    console.log("Error dans la gestion du grafcet");
                                break;
                            }

                            //%%AP - 2022.07.27-11.40
                            //vFMAINStartMeasure();
                            vFMAINStopMeasure();
                            // Changement d'onglet :
                            // End of calibration activation
                            vFEndCalibrationStart()
                            // On fait disparaitre le busy
                            busyConfigZeroPCalib.visible = false;
                            // Fermeture
                            set0Dialog.close()
                            // Autorisation du click
                            vFAllowUserClick()
                        }
                    }

                    Dialog {
                        id: set0Dialog
                        title: qsTr("Set 0%")
                        standardButtons: Dialog.NoButton
                        //x: ( parent.width - width ) / 2
                        //y: ( parent.height - height ) / 2
                        //x: ( window.width - width ) / 2
                        //y: ( window.height - height ) / 2
                        //anchors.centerIn: parent

                        Label {
                            id: set0DialogLabel
                            text: qsTr("Stopping measurement...")
                        }
                    }

                    /*
                    Timer {
                        id: 0PCalibrationTimer2
                        //interval: function(){ return( uiMeasurePeriod * 1000 ) }; running: true; repeat: true
                        interval: 6000; running: false; repeat: false
                        onTriggered: {

                        }
                    }
                    */
                    //--------------------
                    // Busy : calibration settings
                    //--------------------
                    BusyIndicatorPerso {
                        id: busyConfigZeroPCalib
                        width: 31
                        height: 32
                        anchors.left: btnConfirmZeroPCalib.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                    //--------------------
                    // Error : calibration settings
                    //--------------------
                    Image {
                        id: errorConfigZeroPCalib
                        width: 31
                        height: 31
                        source: "qrc:/Logo/Global/No_Cross.png"
                        anchors.left: btnConfirmZeroPCalib.right
                        anchors.leftMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }

                    //--------------------
                    // Skip 0P calibration
                    //--------------------
                    ButtonPerso {
                        id: btnSkipSet0P
                        width: 100
                        height: 28
                        text: qsTr("Skip")
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        onClicked: {
                            //vFReturnFrom0PCalibration();
                            // Skip 1st test
                            vFEndCalibrationStart();

                            // Skip de la calibration en 100
                            //vFExitAirCalibration_GoTo0PCalibration();
                            // Restauration
                            // Remplacement des valeurs dphi0 et dphi100
                            //tO2Event.setCalibPointManually( 53000, m_uliPreviousDphi100 )
                            tO2Event.setCalibPoint0PManually( m_uliPreviousDphi0 )
                            tO2Event.set0CalibrationTemperature( parseInt(m_uliPreviousTemperature0) );
                            // Envoi de la première commande modbus
                            //tO2Event.sendConfToI2CSensor()
                            // Arrêt de la mesure
                            vFMAINStopMeasure();
                            //
                            timerSkip0PCalibration.uiState = 0
                            timerSkip0PCalibration.start();
                            skip0PDialogLabel.text = qsTr("Stopping measurement...");
                            skip0PDialog.open();
                            busyO2Skip0P.visible = true;
                        }
                    }
                    Timer {
                        id: timerSkip0PCalibration
                        //interval: function(){ return( uiMeasurePeriod * 1000 ) }; running: true; repeat: true
                        interval: 100; running: false; repeat: true
                        property int uiState: 0
                        property int uiCompteurSecondLast: 0
                        onTriggered: {
                            // Tant que le grafcet est en busy
                            if( tMainEvent.isModbusBusyState() ){
                                return;
                            }
                            // Selon l'état courant
                            switch( uiState ){
                                // Fin d'arrêt de la mesure
                                //-------------------------
                                // CMD Transfert configuration vers interface capteur
                                //-------------------------
                                case 0:
                                    // Remplacement des valeurs dphi0
                                    tO2Event.setCalibPoint0PManually( m_uliPreviousDphi0 )
                                    tO2Event.set0CalibrationTemperature( parseInt(m_uliPreviousTemperature0) );
                                    // Envoi de la première commande modbus
                                    tO2Event.sendConfToI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 1:
                                    // CMD d'envoi de la configuration
                                    tO2Event.sendConfigWriteCMD();
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // Read windows + delais
                                //-------------------------
                                case 2:
                                    // Modification du texte de la fenêtre
                                    skip0PDialogLabel.text = qsTr("Reading new configuration...")+"\r\n";
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers capteur pyro
                                //-------------------------
                                case 3:
                                    // CMD read
                                    tO2Event.sendReadCMD();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers PC
                                //-------------------------
                                case 4:
                                    // CMD read
                                    tO2Event.readConfFromI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // Fin grafcet
                                //-------------------------
                                case 5:
                                    // Stop du timer et du grafcet
                                    timerSkip0PCalibration.stop();
                                    // Incrément de l'état
                                    //uiState++;
                                break;
                                //-------------------------
                                // Default
                                //-------------------------
                                default:
                                    // Stop du timer et du grafcet
                                    timerSkip0PCalibration.stop();
                                    console.log("Error dans la gestion du grafcet");
                                break;
                            }
                            console.log("QML End of skip Air calibration grafcet");
                            //%%AP - 2022.07.27 - Non
                            // Relance de la mesure
                            //vFMAINStartMeasure();
                            vFMAINStopMeasure();
                            // Changement de partie
                            vFEndCalibrationStart();
                            // On fait disparaitre le busy
                            busyO2Skip0P.visible = false;
                            // Désactivation calibration set air
                            //airCalibrationBloc.enabled = false;
                            // Activation calibration 0%
                            //zeroCalib.enabled = true;
                            // Fermeture
                            skip0PDialog.close();
                            // Autorisation du click
                            vFAllowUserClick();
                        }
                    }
                    Dialog {
                        id: skip0PDialog
                        title: qsTr("Set air")
                        standardButtons: Dialog.NoButton

                        Label {
                            id: skip0PDialogLabel
                            text: qsTr("Stopping measurement...")
                        }
                    }
                    //--------------------
                    // Busy : calibration settings
                    //--------------------
                    BusyIndicatorPerso {
                        id: busyO2Skip0P
                        width: 31
                        height: 32
                        anchors.right: btnSkipSet0P.left
                        anchors.rightMargin: 30
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                        visible: false
                    }
                }

                //--------------------------------------------------
                // Bloc : Save calibration
                //--------------------------------------------------
                DropShadow {
                    color: "#20000000"
                    radius: 12
                    verticalOffset: 0
                    anchors.fill: endCalibrationBloc
                    samples: 30
                    horizontalOffset: 0
                    source: endCalibrationBloc
                }
                Rectangle {
                    id: endCalibrationBloc
                    enabled: false
                    x: -2
                    width: o2PartID.width - 120
                    height: 88
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: zeroCalib.bottom
                    anchors.topMargin: 24
                    anchors.horizontalCenterOffset: 0
                    Rectangle {
                        id: sensorSettings5
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        color: "#e4e3e9"
                        Text {
                            id: sensorSettingsTxt2
                            text: qsTr("Save calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.weight: Font.DemiBold
                            font.bold: false
                            font.family: "Open Sans"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: (endCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutSensorSettings2
                        x: 0
                        width: parent.width
                        height: parent.height - 44
                        anchors.top: sensorSettings5.bottom
                        anchors.topMargin: 5
                        RowLayout {
                            id: rowSC5
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                            Rectangle {
                                id: rowSC4
                                //width: ( parent.width - 25 ) / 3
                                width: ( parent.width - 25 ) / 2
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                //Layout.preferredWidth: ( parent.width - 15 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 2

                                ButtonPerso {
                                    //id: btnConfirmZeroPCalib3
                                    width: 100
                                    height: 28
                                    text: qsTr("Return")
                                    anchors.verticalCenter: parent.verticalCenter
                                    //anchors.left: parent.left
                                    //anchors.leftMargin: 10
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    onClicked: {
                                        // Gestion de l'affichage
                                        vFReturnFromEndCalibration()
                                    }
                                }
                                /*
                                Image {
                                    id: imgSC2
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    opacity: (endCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                                }

                                Text {
                                    id: txtSC2
                                    height: parent.height
                                    text: qsTr("Save calibration")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgSC2.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                    opacity: (endCalibrationBloc.enabled?m_fTextOpacityFullValue:m_fTextOpacityMidValue)
                                }
                                */
                            }


                            /*
                            Rectangle {
                                id: rectangleSC6
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                                //--------------------
                                // Busy : Save Calibration
                                //--------------------
                                BusyIndicatorPerso {
                                    id: busySaveCalibration
                                    width: 31
                                    height: 32
                                    anchors.left: btnSaveCalibration.left
                                    anchors.leftMargin: 30
                                    anchors.bottom: btnSaveCalibration.bottom
                                    anchors.bottomMargin: 0
                                    visible: false
                                }
                                //--------------------
                                // Error : Save Calibration
                                //--------------------
                                Image {
                                    id: errorSaveCalibration
                                    width: 31
                                    height: 31
                                    source: "qrc:/Logo/Global/No_Cross.png"
                                    //anchors.leftMargin: 0
                                    anchors.left: btnSaveCalibration.left
                                    anchors.leftMargin: 30
                                    anchors.bottom: btnSaveCalibration.bottom
                                    anchors.bottomMargin: 0
                                    visible: false
                                }
                                ButtonPerso {
                                    id: btnSaveCalibration
                                    width: 100
                                    height: 28
                                    text: qsTr("Save")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: !busySaveCalibration.visible && !errorSaveCalibration.visible
                                    onClicked: {
                                        // Blocage du click utilisateur
                                        vFLockUserClick();
                                        // Affichage du busy
                                        busySaveCalibration.visible = true;
                                        // Démarrage du timer
                                        saveCalibrationTimer.uiState = 0;
                                        saveCalibrationTimer.start();
                                    }
                                }

                                Timer {
                                    id: saveCalibrationTimer
                                    interval: 100; running: false; repeat: true
                                    property int uiState: 0
                                    property int uiCompteurSecondLast: 0
                                    onTriggered: {
                                        // Tant que le grafcet est en busy
                                        if( tMainEvent.isModbusBusyState() ){
                                            return;
                                        }

                                        // Selon l'état courant
                                        switch( uiState ){
                                            // Fin d'arrêt de la mesure
                                            //-------------------------
                                            // CMD Sauvegarde permanente des paramètres
                                            //-------------------------
                                            case 0:
                                                // Save register permanently
                                                tO2Event.saveRegisterPermanently();
                                                // Incrément de l'état
                                                uiState++;
                                            // Quitte
                                            return;
                                            //-------------------------
                                            // Fin grafcet
                                            //-------------------------
                                            case 1:
                                                // Stop du timer et du grafcet
                                                saveCalibrationTimer.stop();
                                            break;
                                            //-------------------------
                                            // Default
                                            //-------------------------
                                            default:
                                                // Stop du timer et du grafcet
                                                saveCalibrationTimer.stop();
                                                console.log("Error dans la gestion du grafcet");
                                            break;
                                        }

                                        // Valide : Retour sur la slide précédente
                                        swipeViewO2Calibration.currentIndex = 0;
                                        // Activation de la slide 0
                                        firstCalibSwipe.enabled = true;
                                        // Désactivation end of calibration
                                        endCalibrationBloc.enabled = false;
                                        // Affichage du busy
                                        busySaveCalibration.visible = false;
                                        // Taille de fenêtre par défaut
                                        window.width = 1020
                                        // Autorisation du click
                                        vFAllowUserClick();
                                    }
                                }
                            }
                            */
                            Rectangle {
                                id: rectangleSCDummy2
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                //Layout.preferredWidth: ( parent.width - 15 ) / 3
                                Layout.preferredWidth: ( parent.width - 15 ) / 2

                                ButtonPerso {
                                    id: btnConfirmZeroPCalib3
                                    width: 100
                                    height: 28
                                    text: qsTr("Validate")
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    //anchors.right: parent.right
                                    //anchors.rightMargin: 10
                                    onClicked: {
                                        // Gestion de l'affichage
                                        //vFReturnFromEndCalibration()
                                        // Valide : Retour sur la slide précédente
                                        swipeViewO2Calibration.currentIndex = 0;
                                        // Activation de la slide 0
                                        firstCalibSwipe.enabled = true;
                                        // Désactivation end of calibration
                                        endCalibrationBloc.enabled = false;
                                        // Affichage du busy
                                        busySaveCalibration.visible = false;
                                        // Taille de fenêtre par défaut
                                        window.width = 1020
                                        // Autorisation du click
                                        vFAllowUserClick();
                                    }
                                }
                            }

                        }
                    }

                }
                // Bloc sensor settings
            }
            /*
            Item{
                id: thirdCalibSwipe
            }
            Item{
                id: fourthCalibSwipe
            }
            */
        }
        /*
        Button {
            x: 488
            y: 37
            width: 100
            height: 28
            text: "Refresh"
            onClicked: {
                console.log("TEST CLICK");
                tO2Event.sendReadCMD();
            }
        }
        */
    } // rectanglePartO2


/*
    Button {
        id: button
        text: qsTr("Start")
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 40
        anchors.bottomMargin: 20
        width: 80
        onClicked: {
            //animateOpacity2.start()
            var uiWidth = parseFloat( window.width )
            animateWindowWidth.from = uiWidth
            animateWindowWidth.start()
        }
    }
    Button {
        id: button1
        width: 80
        text: qsTr("Stop")
        anchors.left: button.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.leftMargin: 40
        onClicked: {
            //animateOpacity3.start()
            var uiWidth = parseFloat( window.width )
            animateWindowWidth2.from = uiWidth
            animateWindowWidth2.start()
        }
    }
    NumberAnimation {
        id: animateWindowWidth
        target: window
        properties: "width"
        from: window.width
        to: window.minimumWidth
        duration: 500
        //loops: Animation.Infinite
        //easing {type: Easing.OutBack; overshoot: 500}
    }

    NumberAnimation {
        id: animateWindowWidth2
        target: window
        properties: "width"
        from: window.width
        to: window.maximumWidth
        duration: 500
        //loops: Animation.Infinite
        //easing {type: Easing.OutBack; overshoot: 500}
    }
*/
}


