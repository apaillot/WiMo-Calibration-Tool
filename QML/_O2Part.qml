import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

import QtQuick.Dialogs 1.3
import QtQuick.Extras 1.4

//import QtQml.Models 2.15


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

    // Sauvegarde de point de calibration
    property int m_uliPreviousDphi100: 0
    property int m_uliPreviousDphi0: 0

    property int m_uiTextOpacityFullValue: 1.0
    property int m_uiTextOpacityMidValue: 0.6

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
                                          // Phase shift with calibration
                                          uliCalibDphi0,
                                          uliCalibDphi100,
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

            console.log( "uliCalibDphi0 = "+uliCalibDphi0 );
            console.log( "uliCalibDphi100 = "+uliCalibDphi100 );
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

            // Settings
            inputSC0.text = ucSensorType.toString()+ucIntensity.toString()+ucAmp.toString()
            inputSC1.text = parseInt(uliCodeDphi0 / 100)
            inputSC2.text = parseInt(uliCodeDphi100 / 100)

            // Condition in the sample
            switchExternalTemperature.checked = bExternalTemperature
            inputFixedTemperature.text        = parseFloat( liTemperature ) / 1000
            inputFixedPressure.text           = parseFloat( uliPressure ) / 1000
            // Air calibration
            //switchExternalTemperature100.checked = bExternalTemperature100
            //inputFixedTemperature100.text        = parseFloat( liTemperature100 ) / 1000
            //inputFixedPressure100.text           = parseFloat( uliPressure100 ) / 1000

            // 0% calibration
            //switchExternalTemperature0.checked = bExternalTemperature0
            //inputFixedTemperature0.text        = parseFloat( liTemperature0 ) / 1000

            // ShowPoint popup
            showPointDphi0.text   = parseFloat( uliCodeDphi0 ) / 1000;
            showPointDphi100.text = parseFloat( uliCodeDphi100 ) / 1000;
            elementDphi0Display.text = parseFloat( uliCodeDphi0 ) / 1000;
            elementDphi100Display.text = parseFloat( uliCodeDphi100 ) / 1000;
            // SetPoint popup
            setPointDphi0.text   = parseFloat( uliCodeDphi0 ) / 1000
            setPointDphi100.text = parseFloat( uliCodeDphi100 ) / 1000

            // External temperature - Si activation
            if( switchExternalTemperature.checked ){
                inputFixedTemperature.enabled = false
            }
            // Désactivation
            else{
                inputFixedTemperature.enabled = true
            }
            // External temperature air - Si activation
            //if( switchExternalTemperature100.checked ){
            //    inputFixedTemperature100.enabled = false
            //}
            // Désactivation
            //else{
            //    inputFixedTemperature100.enabled = true
            //}
            // External temperature 0 - Si activation
            //if( switchExternalTemperature0.checked ){
            //    inputFixedTemperature0.enabled = false
            //}
            // Désactivation
            //else{
            //    inputFixedTemperature0.enabled = true
            //}
        }
    }

    //=============================================================
    // Fonctions
    //=============================================================
/*
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
  */  
    //-------------------------------------------------------------
    // Démarrage d'une nouvelle calibration
    //-------------------------------------------------------------
    function vFStartNewCalibration(){
        //%%AP - A remettre
/*
        var uiWindowWidth = parseFloat( window.width )
        startNewCalibrationWindowWidth.from = uiWindowWidth
        startNewCalibrationWindowWidth.start()
*/

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

        // Désactivation de la premier page
        firstCalibSwipe.enabled = true
        // Activation du bloc
        airCalibrationBloc.enabled = false
        //
        swipeViewO2Calibration.currentIndex = 0
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
        swipeViewO2Calibration.currentIndex = 2
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
        swipeViewO2Calibration.currentIndex = 1
    }

    //=============================================================
    // Objet
    //=============================================================

    //*************************************************************
    // Le code de l'objet
    //*************************************************************
    id: o2PartID
    width: 662
    height: 635
    property alias swipeViewO2Calibration: swipeViewO2Calibration
    implicitWidth: 662
    implicitHeight: 1080
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
                    height: 330
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle {
                        id: sensorSettings2
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        color: "#e4e3e9"
                        Text {
                            id: sensorSettingsTxt
                            //x: 63
                            //y: 14
                            text: qsTr("Sensor settings")
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
                        height: parent.height - 44 - 44 + 4
                        anchors.top: sensorSettings2.bottom
                        anchors.topMargin: 7

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
                                text: "Oxygen sensor calibration value"
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
                                text: qsTr("O2 sensor calibration is determined by a 2-point calibration, "
                                +"where dphi0 and dphi100 represent the raw value mesured in a 0%-standard (anoxic) "
                                +"and in a 100%-standard (type 20.95%O2), respectively."
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
                                text: "Factory mode"
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
                                    text: qsTr("Calibration type")
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
                                ListElement { text:"Factory mode"; }
                                /*ListElement { text:"2 points: 0%-100%"; }*/
                                // Réautorisation pour debug
                                ListElement { text:"2 points: 0%-100%"; }
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
                                    text: qsTr(" - ")
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
                                    text: qsTr(" - ")
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
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
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
                                tooltipSC0.show("Sensor code must begin with Sxx")
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
                                tooltipSC0.show("Second caracter must be between A and H")
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
                                tooltipSC0.show("Third caracter must be between 4 and 7")
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
                                tooltipSC1.show("Value must be an integer")
                                return;
                            }
                            if( fCodeDphi100 !== fCodeDphi100Rounded ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC2.show("Value must be an integer")
                                return;
                            }
                            if(  ( fCodeDphi0 > 900 )
                              || ( fCodeDphi0 < 0     ) ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC1.show("Value must be between 0 and 900")
                                return;
                            }
                            if(  ( fCodeDphi100 > 900 )
                              || ( fCodeDphi100 < 0     ) ){
                                // On désactive l'état occupé
                                busyO2Settings.visible  = false
                                errorO2Settings.visible = true
                                tooltipSC2.show("Value must be between 0 and 900")
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
                    //x: 60
                    //y: 99
                    width: o2PartID.width - 120
                    visible: (!bFactorycalibration)

                    height: 213
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: sensorSettings.bottom
                    anchors.topMargin: 20
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
                            text: qsTr("Condition in the sample")
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
                        id: columnLayoutCits
                        x: 0
                        //y: 39
                        width: parent.width
                        height: parent.height - 44 - 35
                        anchors.top: cits2.bottom
                        anchors.topMargin: 4

                        //------------------------------
                        // External temperature
                        //------------------------------
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
                                    text: qsTr("External temperature")
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
                                        }
                                        // Désactivation
                                        else{
                                            inputFixedTemperature.enabled = true
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
                                    text: qsTr("Fixed temperature (°C)")
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
                                    text: qsTr("Fixed pressure (mbar)")
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

                            var bExternalTemperature = switchExternalTemperature.checked
                            var liTemperature = parseInt( inputFixedTemperature.text )
                            var uliPressure   = parseInt( inputFixedPressure.text )

                            // On désactive l'état d'erreur si présent
                            errorO2ConditionInTheSample.visible = false
                            // On fait apparaître le busy
                            busyO2ConditionInTheSample.visible = true

                            //--------------
                            // External temperature
                            //--------------
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
                                    tooltipFixedTemperature.show("Please enter a value with a maximum of 3 decimal places")
                                    return;
                                }
                                if(  ( fFixedTemperature > 300 )
                                  || ( fFixedTemperature < -300 ) ){
                                    // On désactive l'état occupé
                                    busyO2ConditionInTheSample.visible  = false
                                    errorO2ConditionInTheSample.visible = true
                                    tooltipFixedTemperature.show("Value must be between -300 and 300")
                                    return;
                                }
                                liTemperature = parseInt( fFixedTemperature * 1000 )
                            }
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
                                tooltipFixedPressure.show("Please enter a value with a maximum of 3 decimal places")
                                return;
                            }
                            if(  ( fFixedPressure > 10000 )
                              || ( fFixedPressure < 0.001 ) ){
                                // On désactive l'état occupé
                                busyO2ConditionInTheSample.visible  = false
                                errorO2ConditionInTheSample.visible = true
                                tooltipFixedPressure.show("Value must be between 0.001 and 10000")
                                return;
                            }
                            uliPressure   = parseInt( fFixedPressure * 1000 )

                            console.log( "== Après validation ==" );
                            console.log( "bExternalTemperature = "+bExternalTemperature );
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
                    visible: (!bFactorycalibration)
                }
                Rectangle {
                    id: startCalibrationBloc
                    x: 1
                    width: o2PartID.width - 120
                    //height: 121
                    height: 160
                    color: "#ffffff"
                    anchors.top: conditionInTheSamples.bottom
                    anchors.topMargin: 20
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: (!bFactorycalibration)
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
                                            text: qsTr("--")
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
                                            text: qsTr("--")
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
                                    id: busyO2StartCalibration
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
                                    id: btnStartCalibration
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
                                MessageDialog {
                                    id: msgDialStartCalibration
                                    title: "Warning"
                                    text:  "Start new calibration: last one will be reset.\r\n"+
                                           "Please keep in note previous dphi point if you are not sure."
                                    standardButtons: StandardButton.Ok | StandardButton.Cancel

                                    function vFStartCalibrationAfterResize(){
/*
                                        // Sauvegarde des points précédents en cas de return
                                        m_uliPreviousDphi100 = m_uliCodeDphi100
                                        m_uliPreviousDphi0   = m_uliCodeDphi0

                                        // Remplacement des valeurs dphi0 et dphi100
                                        tO2Event.setCalibPointManually( 53000, 20000 )
                                        // Envoi de la première commande modbus
                                        tO2Event.sendConfToI2CSensor()
                                        // Réinit des paramètres du timer
                                        timerSubmitStartCalibration.ucState = 0
                                        // Démarrage du timer de gestion
                                        timerSubmitStartCalibration.start()
*/
                                    }
                                    onAccepted:{
                                        if( bSetSensorMsg && StandardButton.Ok ){
                                            bSetSensorMsg = false
                                            console.log("OK");
                                            busyO2StartCalibration.visible = true
                                            btnStartCalibration.visible    = false
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
                                            //case 1:
                                                // CMD Save configuration permanently
                                                //tO2Event.saveRegisterPermanently();
                                                // Incrément de l'état
                                               // ucState++;
                                            //break;
                                            //-------------------------
                                            // Default
                                            //-------------------------
                                            default: break;
                                        }
                                        // Stop du timer
                                        timerSubmitStartCalibration.stop();
                                        // On fait disparaitre le busy
                                        busyO2StartCalibration.visible  = false
                                        // On désactive l'état d'erreur si présent
                                        errorO2StartCalibration.visible = false
                                        // Réactivation du bouton start calibration
                                        btnStartCalibration.visible     = true
                                        //%%AP - A remettre
                                        // Affichage du graphique
                                        //vFStartO2CalibrationLiveview_MAIN();
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

                                ButtonPerso {
                                    id: btnSetPointManually
                                    width: 122
                                    height: 28
                                    text: qsTr("Set point manually")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
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
                                            placeholderText: qsTr("--")
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
                                            placeholderText: qsTr("--")
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
                                                    popupSetPointDphi0.show("Please enter a value with a maximum of 3 decimal places")
                                                    return;
                                                 }
                                                // Phy : Test si max step est bien à 0.001
                                                if(  ( fDphi0 > 90000 )
                                                  || ( fDphi0 < 0 ) )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi0.show("Value must be between 0 and 90000")
                                                    return;
                                                 }
                                                // Raw : Test si max step est bien à 0.1
                                                if( fDphi100 !== fDphi100Rounded )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi100.show("Please enter a value with a maximum of 1 decimal places")
                                                    return;
                                                 }
                                                // Raw : Test si max step est bien à 0.001
                                                if(  ( fDphi100 > 90000 )
                                                  || ( fDphi100 < 0 ) )
                                                 {
                                                    // Passage en erreur
                                                    busyO2SetPoints = false
                                                    errorO2SetPoints.visible  = true
                                                    popupSetPointDphi100.show("Value must be between 0 and 90000")
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
                    x: -9
                    width: o2PartID.width - 120
                    //height: 440
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
                            text: qsTr("Air calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.bold: false
                            font.family: "Open Sans"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                            //opacity: (airCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutCits1
                        x: 0
                        width: parent.width
                        //height: 350
                        height: airCalibrationBloc.height - cits3.height - btnConfirmSetAir.height - btnConfirmSetAir.bottom - 8

                        anchors.top: cits3.bottom
                        //anchors.topMargin: 10
                        anchors.topMargin: 8

                        // 1 - 100% point calibration condition
                        Rectangle {
                            id: cits4
                            height: 55
                            transformOrigin: Item.Center
                            clip: false
                            anchors.left: parent.left
                            anchors.right: parent.right

                            ColumnLayout {
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8
                                width: parent.width - 40

                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 20

                                Text {
                                    id: text1
                                    //y: 10
                                    text: qsTr("1 - 100% point calibration condition")
                                    width: parent.width
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    //opacity: (airCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                                }
                                Text {
                                    id: text5
                                    text: qsTr("Please place the sensor in a 100% saturated solution.")
                                    anchors.leftMargin: 20
                                    font.pixelSize: 11
                                    styleColor: "#545454"
                                    //opacity: (airCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                                }
                            }
                        }
                        /*
                        Rectangle {
                            id: cits41
                            x: 0
                            y: 52
                            width: parent.width
                            height: 34
                            color: "#ffffff"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                id: text5
                                text: qsTr("Please place the sensor in a 100% saturated solution.")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 11
                                styleColor: "#545454"
                                anchors.leftMargin: 20
                            }
                        }
                        */
                        /*
                        // 2 - Adjust Calibration Conditions
                        Rectangle {
                            id: cits5
                            width: parent.width
                            height: 34
                            color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                id: text2

                                text: qsTr("2 - Adjust Calibration Conditions")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                            }
                        }
                        Rectangle {
                            id: externalTemperatureFirstLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        //-------------------------------------------
                        // Air calib - External temperature
                        //-------------------------------------------
                        RowLayout {
                            id: rowExternalTemperature1
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Rectangle {
                                id: rowExternalTemperature3
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgExternalTemperature1
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtExternalTemperature1
                                    height: parent.height
                                    text: qsTr("External temperature")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgExternalTemperature1.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleExternalTemperature5
                                Layout.preferredHeight: 30
                                SwitchButtonPerso {
                                    id: switchExternalTemperature100
                                    height: 42
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    scale: 0.9
                                    onClicked: {
                                        console.log( " switchExternalTemperature100 = "+ switchExternalTemperature100.checked )
                                        // Si activation
                                        if( switchExternalTemperature100.checked ){
                                            inputFixedTemperature100.enabled = false
                                        }
                                        // Désactivation
                                        else{
                                            inputFixedTemperature100.enabled = true
                                        }
                                    }
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleExternalTemperatureDummy1
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                        }

                        Rectangle {
                            id: externalTemperatureLine1
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }

                        //-------------------------------------------
                        // Air calib - Fixed temperature
                        //-------------------------------------------
                        RowLayout {
                            id: rowFixedTemperature1
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Rectangle {
                                id: rowFixedTemperature3
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgFixedTemperature1
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtFixedTemperature0
                                    height: parent.height
                                    text: qsTr("Fixed temperature (°C)")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgFixedTemperature1.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleFixedTemperature5
                                Layout.preferredHeight: 30
                                TextField {
                                    id: inputFixedTemperature100
                                    width: parent.width - 30
                                    height: 30
                                    text: qsTr("")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    selectByMouse: true
                                    ToolTip {
                                        id: tooltipFixedTemperature100
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
                                    activeFocusOnPress: true
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleFixedTemperatureDummy1
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                        }

                        Rectangle {
                            id: fixedTemperatureLine1
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }

                        //-------------------------------------------
                        // Air calib - Fixed pressure
                        //-------------------------------------------
                        RowLayout {
                            id: rowFixedPressure1
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Rectangle {
                                id: rowFixedPressure3
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgFixedPressure1
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtFixedPressure1
                                    height: parent.height
                                    text: qsTr("Fixed pressure (mbar)")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgFixedPressure1.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleFixedPressure5
                                Layout.preferredHeight: 30
                                TextField {
                                    id: inputFixedPressure100
                                    width: parent.width - 30
                                    height: 30
                                    text: qsTr("")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    selectByMouse: true
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
                                    activeFocusOnPress: true
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleFixedPressureDummy1
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                        }

                        Rectangle {
                            id: fixedPressureLine1
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        */

                        Rectangle {
                            id: cits31Line
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            anchors.left: parent.left
                            anchors.right: parent.right
                        }
                        // 2 - Wait for steady condition in the graphical part
                        Rectangle {
                            id: cits31
                            width: parent.width
                            height: 34
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            anchors.left: parent.left
                            anchors.right: parent.right
                            Text {
                                id: text3
                                //text: qsTr("3 - Wait for steady condition in the graphical part")
                                text: qsTr("2 - Wait for steady condition in the graphical part")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                //opacity: (airCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                            }
                        }
                        Rectangle {
                            id: cits411Line
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            anchors.left: parent.left
                            anchors.right: parent.right
                        }
                        // 4 - Press below button to calibrate
                        Rectangle {
                            id: cits411
                            width: parent.width
                            height: 34
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            anchors.left: parent.left
                            anchors.right: parent.right
                            Text {
                                id: text41
                                //text: qsTr("4 - Press below button to calibrate")
                                text: qsTr("3 - Press below button to calibrate")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                //opacity: (airCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                            }
                        }

                        Rectangle {
                            id: btnConfirmSetAirLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            anchors.left: parent.left
                            anchors.right: parent.right
                        }
                    }
                    ButtonPerso {
                        id: btnConfirmSetAir
                        width: 100
                        height: 28
                        text: qsTr("Set air")
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

                            /*
                            var bExternalTemperature100 = switchExternalTemperature100.checked
                            var liTemperature100 = parseInt( inputFixedTemperature100.text )
                            var uliPressure100   = parseInt( inputFixedPressure100.text )

                            console.log( "bExternalTemperature100 = "+bExternalTemperature100 );
                            console.log( "liTemperature100 = "+liTemperature100 );
                            console.log( "uliPressure100 = "+uliPressure100 );

                            //--------------
                            // External temperature
                            //--------------
                            if( bExternalTemperature100 ){
                                liTemperature100 = parseInt( -300000 )
                            }
                            else{
                                //-------------------------
                                // Fixed temperature
                                //-------------------------
                                var fFixedTemperature        = parseFloat( inputFixedTemperature100.text );
                                var fFixedTemperatureRounded = parseFloat(parseInt(fFixedTemperature*1000))/1000;
                                // Vérification fonctionnement avec un entier
                                if( fFixedTemperature !== fFixedTemperatureRounded ){
                                    // On désactive l'état occupé
                                    busyO2SetAir.visible  = false
                                    errorO2SetAir.visible = true
                                    tooltipFixedTemperature100.show("Please enter a value with a maximum of 3 decimal places")
                                    vFAllowUserClick()
                                    return;
                                }
                                if(  ( fFixedTemperature > 300 )
                                  || ( fFixedTemperature < -300 ) ){
                                    // On désactive l'état occupé
                                    busyO2SetAir.visible  = false
                                    errorO2SetAir.visible = true
                                    tooltipFixedTemperature100.show("Value must be between -300 and 300")
                                    vFAllowUserClick()
                                    return;
                                }
                                liTemperature100 = parseInt( fFixedTemperature * 1000 )
                            }
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
                                tooltipFixedPressure100.show("Please enter a value with a maximum of 3 decimal places")
                                vFAllowUserClick()
                                return;
                            }
                            if(  ( fFixedPressure > 10000 )
                              || ( fFixedPressure < 0.001 ) ){
                                // On désactive l'état occupé
                                busyO2SetAir.visible  = false
                                errorO2SetAir.visible = true
                                tooltipFixedPressure100.show("Value must be between 0.001 and 10000")
                                vFAllowUserClick()
                                return;
                            }
                            uliPressure100   = parseInt( fFixedPressure * 1000 )

                            console.log( "== Après validation ==" );

                            console.log( "bExternalTemperature100 = "+bExternalTemperature100 );
                            console.log( "liTemperature100 = "+liTemperature100 );
                            console.log( "uliPressure100 = "+uliPressure100 );

                            // Sauvegarde des paramètres et déclenchement de la commande Set air
                            tO2Event.setAirCalibrationSettings( bExternalTemperature100,
                                                                liTemperature100,
                                                                uliPressure100 );
                            */

                            // Arrêt de la mesure
                            vFMAINStopMeasure();
                            // Start timer
                            console.log("airCalibrationTimer.start();");
                            //timerAirCalibration.interval = 1000
                            timerAirCalibration.uiState = 2
                            timerAirCalibration.start();
                            setAirDialogLabel.text = "Stopping measurement...";
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
                                        setAirDialogLabel.text = "Set air command occuring..."
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
                                            setAirDialogLabel.text = "Set air command occuring...\r\n"+
                                            (parseFloat(uiCompteurSecondLast)/10)+
                                            " seconds left.";
                                            return;
                                        }
                                        // Incrément de l'état
                                        uiState++;
                                    // Quitte
                                    return;
                                    //-------------------------
                                    // CMD Lecture configuration vers capteur pyro
                                    //-------------------------
                                    case 4:
                                        // Modification du texte de la fenêtre
                                        setAirDialogLabel.text = "Reading new configuration...\r\n";
                                        // CMD read
                                        tO2Event.sendReadCMD();
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // CMD Lecture configuration vers PC
                                    //-------------------------
                                    case 5:
                                        // CMD read
                                        tO2Event.readConfFromI2CSensor();
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // CMD Envoi configuration vers capteur pyro
                                    //-------------------------
                                    case 6:
                                        //%%AP - 2021.02.12 - Enregistrement ici?
                                        // CMD Save configuration permanently
                                        //tO2Event.saveRegisterPermanently();
                                        // Incrément de l'état
                                        uiState++;
                                    return;
                                    //-------------------------
                                    // Fin grafcet
                                    //-------------------------
                                    case 7:
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
                        //x: ( parent.width - width ) / 2
                        //y: ( parent.height - height ) / 2
                        //x: ( window.width - width ) / 2
                        //y: ( window.height - height ) / 2
                        //anchors.centerIn: parent

                        Label {
                            id: setAirDialogLabel
                            text: "Stopping measurement..."
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
                        id: btnConfirmCalib100
                        x: -8
                        y: 6
                        width: 100
                        height: 28
                        text: qsTr("Return")
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        anchors.rightMargin: 10
                        onClicked: {
                            // Retour de la calibration
                            vFReturnFromAirCalibration();
                        }
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
                    height: 250
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
                            //opacity: (zeroCalib.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                        }
                        transformOrigin: Item.Center
                        clip: false
                    }

                    ColumnLayout {
                        id: columnLayoutZeroPCalib2
                        //x: 0
                        //width: parent.width
                        //height: parent.height - 44 - 35
                        height: parent.height - 44 - 40

                        anchors.top: zeroPCalib4.bottom
                        anchors.topMargin: 4
                        anchors.bottom: zeroCalib.bottom
                        anchors.bottomMargin: 40

                        anchors.left: parent.left
                        anchors.right: parent.right

                        // 1 - 0% point calibration condition
                        Rectangle {
                            id: rect0p
                            //width: parent.width
                            height: 55
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false

                            anchors.left: parent.left
                            anchors.right: parent.right

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
                                    text: qsTr("1 - 0% point calibration condition")
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    //opacity: (zeroCalib.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                                }
                                Text {
                                    id: text5511
                                    text: qsTr("Please place the sensor in a water mixed with sodium sulfite,"
                                              +"or in water with nitrogen gas for several minutes, or directly in nitrogen gas.")
                                    // Fonctionne bien
                                    //anchors.left: parent.left
                                    //anchors.right: parent.right
                                    Layout.fillWidth: true
                                    font.pixelSize: 11
                                    verticalAlignment: Text.AlignVCenter
                                    wrapMode: Text.WordWrap
                                    styleColor: "#545454"
                                    //opacity: (zeroCalib.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                                }
                            }

                        }
                        /*
                        // 2 - Adjust Calibration Conditions
                        Rectangle {
                            id: cits50p
                            width: parent.width
                            height: 34
                            color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            Text {
                                id: text20p

                                text: qsTr("2 - Adjust Calibration Conditions")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20

                            }
                        }
                        Rectangle {
                            id: externalTemperatureFirstLine0p
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }


                        //-------------------------------------------
                        // 0% calib - External temperature
                        //-------------------------------------------
                        RowLayout {
                            id: rowExternalTemperature5
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Rectangle {
                                id: rowExternalTemperature4
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgExternalTemperature2
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtExternalTemperature2
                                    height: parent.height
                                    text: qsTr("External temperature")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgExternalTemperature2.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleExternalTemperature6
                                Layout.preferredHeight: 30
                                color: "#00ffffff"

                                SwitchButtonPerso {
                                    id: switchExternalTemperature0
                                    height: 42
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    scale: 0.9
                                    onClicked: {
                                        console.log( " switchExternalTemperature0 = "+ switchExternalTemperature0.checked )
                                        // Si activation
                                        if( switchExternalTemperature0.checked ){
                                            inputFixedTemperature0.enabled = false
                                        }
                                        // Désactivation
                                        else{
                                            inputFixedTemperature0.enabled = true
                                        }
                                    }
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleExternalTemperatureDummy2
                                width: 200
                                height: 200
                                color: "#00ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                        }

                        Rectangle {
                            id: externalTemperatureLine2
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }

                        //-------------------------------------------
                        // 0% calib - Fixed temperature
                        //-------------------------------------------
                        RowLayout {
                            id: rowFixedTemperature5
                            width: parent.width - 20
                            Layout.preferredHeight: 30
                            Rectangle {
                                id: rowFixedTemperature4
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgFixedTemperature2
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                }

                                Text {
                                    id: txtFixedTemperature2
                                    height: parent.height
                                    text: qsTr("Fixed temperature (°C)")
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.left: imgFixedTemperature2.right
                                    font.family: "open sans"
                                    font.pixelSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 10
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

                            Rectangle {
                                id: rectangleFixedTemperature6
                                Layout.preferredHeight: 30
                                color: "#00ffffff"
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                TextField {
                                    id: inputFixedTemperature0
                                    width: parent.width - 30
                                    height: 30
                                    text: qsTr("")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    selectByMouse: true
                                    ToolTip {
                                        id: tooltipFixedTemperature0
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
                                        opacity: 0.95
                                        font.family: "Open Sans"
                                        enter: Transition {
                                            NumberAnimation {
                                                property: "opacity"
                                                from: 0
                                                to: 1
                                            }
                                        }
                                    }
                                    activeFocusOnPress: true
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }

                            Rectangle {
                                id: rectangleFixedTemperatureDummy2
                                width: 200
                                height: 200
                                color: "#00ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }
                            Layout.preferredWidth: parent.width - 20
                            Layout.leftMargin: 10
                        }

                        Rectangle {
                            id: fixedTemperatureLine2
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            Layout.fillWidth: true
                        }
                        */
                        Rectangle {
                            id: cits315Line
                            //width: parent.width
                            height: 1
                            color: "#62888888"
                            anchors.left: parent.left
                            anchors.right: parent.right
                        }
                        // 2 - Wait for steady condition in the graphical part
                        Rectangle {
                            id: cits315
                            height: 34
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false

                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.leftMargin: 0
                            Text {
                                id: text35
                                text: qsTr("2 - Wait for steady condition in the graphical part")
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                //opacity: (zeroCalib.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                            }
                        }
                        Rectangle {
                            id: cits4115Line
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            anchors.left: parent.left
                            anchors.right: parent.right
                        }

                        // 3 - Press below button to calibrate
                        Rectangle {
                            id: cits4115
                            x: 0
                            y: 52
                            width: parent.width
                            height: 34
                            //color: "#e4e3e9"
                            transformOrigin: Item.Center
                            clip: false
                            anchors.left: parent.left
                            anchors.right: parent.right
                            Text {
                                id: text415
                                text: qsTr("3 - Press below button to calibrate")
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                font.pixelSize: 12
                                anchors.leftMargin: 20
                                //opacity: (zeroCalib.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                            }
                        }
                        Rectangle {
                            id: btnConfirmZeroPCalibLine
                            width: parent.width
                            height: 1
                            color: "#62888888"
                            anchors.left: parent.left
                            anchors.right: parent.right
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

                            var bExternalTemperature0 = switchExternalTemperature0.checked
                            var liTemperature0 = parseInt( inputFixedTemperature0.text )

                            console.log( "bExternalTemperature0 = "+bExternalTemperature0 );
                            console.log( "liTemperature0 = "+liTemperature0 );

                            // On fait apparaître le busy
                            busyConfigZeroPCalib.visible  = true
                            // On désactive l'état d'erreur si présent
                            errorConfigZeroPCalib.visible = false

                            //--------------
                            // External temperature
                            //--------------
                            if( bExternalTemperature0 ){
                                liTemperature0 = parseInt( -300000 )
                            }
                            else{
                                //-------------------------
                                // Fixed temperature
                                //-------------------------
                                var fFixedTemperature        = parseFloat( inputFixedTemperature0.text );
                                var fFixedTemperatureRounded = parseFloat(parseInt(fFixedTemperature*1000))/1000;
                                // Vérification fonctionnement avec un entier
                                if( fFixedTemperature !== fFixedTemperatureRounded ){
                                    // On désactive l'état occupé
                                    busyConfigZeroPCalib.visible  = false
                                    errorConfigZeroPCalib.visible = true
                                    tooltipFixedTemperature0.show("Please enter a value with a maximum of 3 decimal places")
                                    return;
                                }
                                if(  ( fFixedTemperature > 300 )
                                  || ( fFixedTemperature < -300 ) ){
                                    // On désactive l'état occupé
                                    busyConfigZeroPCalib.visible  = false
                                    errorConfigZeroPCalib.visible = true
                                    tooltipFixedTemperature0.show("Value must be between -300 and 300")
                                    return;
                                }
                                liTemperature0 = parseInt( fFixedTemperature * 1000 )
                            }

                            console.log( "== Après validation ==" );

                            console.log( "bExternalTemperature0 = "+bExternalTemperature0 );
                            console.log( "liTemperature0 = "+liTemperature0 );



                            // Entrée des numéros de série
                            //tFactoryEvent.saveConfigSettings( uiSN_Y, uiSN_N );

                            /*
                            //calibrationPart1.uiFGetCurrentChannelID()
                            updatePartID.uiChannelID = calibrationPart1.uiFGetCurrentChannelID()
                            // Si c'est un Tbd / Chla_nke / PhycoC_nke / PhycoE_nke / CDOM_nke / Trypto
                            if( bEvosensParam ){
                                //nop();
                                var fIntegrationTime, fRange, fAverage;
                                // Vérification intégration time
                                fIntegrationTime = parseFloat( inputIntegrationTime.text)

                                // Vérification range
                                fRange = parseFloat( inputIntegrationTime.text)
                                // Vérification average
                                fAverage = parseFloat( inputIntegrationTime.text)
                            }
                            */

                            // Set 0% calibration
                            tO2Event.set0PCalibrationSettings( bExternalTemperature0,
                                                               liTemperature0 );
                            // Arrêt de la mesure
                            vFMAINStopMeasure();
                            // Start timer
                            console.log("zeroPCalibrationTimer.start();");
                            //zeroPCalibrationTimer.interval = 1000
                            zeroPCalibrationTimer.uiState = 0
                            zeroPCalibrationTimer.start()
                            set0DialogLabel.text = "Stopping measurement...";
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
                                    set0DialogLabel.text = "Set 0% command occuring...";
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
                                        set0DialogLabel.text = "Set 0% command occuring...\r\n"+
                                        (parseFloat(uiCompteurSecondLast)/10)+
                                        " seconds left.";
                                        return;
                                    }
                                    // Incrément de l'état
                                    uiState++;
                                // Quitte
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers capteur pyro
                                //-------------------------
                                case 4:
                                    // Modification du texte de la fenêtre
                                    set0DialogLabel.text = "Reading new configuration...\r\n";
                                    // CMD read
                                    tO2Event.sendReadCMD();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Lecture configuration vers PC
                                //-------------------------
                                case 5:
                                    // CMD read
                                    tO2Event.readConfFromI2CSensor();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // CMD Envoi configuration vers capteur pyro
                                //-------------------------
                                case 6:
                                    //%%AP - 2021.02.12 - Enregistrement ici?
                                    // CMD Save configuration permanently
                                    //tO2Event.saveRegisterPermanently();
                                    // Incrément de l'état
                                    uiState++;
                                return;
                                //-------------------------
                                // Fin grafcet
                                //-------------------------
                                case 7:
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

                            vFMAINStartMeasure();
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
                            text: "Stopping measurement..."
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

                    ButtonPerso {
                        id: btnConfirmZeroPCalib1
                        x: -8
                        y: 6
                        width: 100
                        height: 28
                        text: qsTr("Return")
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8
                        onClicked: {
                            vFReturnFrom0PCalibration();
                        }
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
                    Rectangle {
                        id: sensorSettings5
                        x: 0
                        y: 0
                        width: parent.width
                        height: 34
                        color: "#e4e3e9"
                        Text {
                            id: sensorSettingsTxt2
                            text: qsTr("End calibration")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.weight: Font.DemiBold
                            font.bold: false
                            font.family: "Open Sans"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                            //opacity: (endCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
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
                                width: ( parent.width - 25 ) / 3
                                height: parent.height
                                Layout.preferredHeight: parent.height
                                Image {
                                    id: imgSC2
                                    width: 30
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Logo/Calibration/calib-average.png"
                                    //opacity: (endCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
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
                                    //opacity: (endCalibrationBloc.enabled?m_uitextOpacityFullValue:m_uiTextOpacityMidValue)
                                }
                                Layout.preferredWidth: ( parent.width - 15 ) / 3
                            }

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
                                    anchors.left: btnSaveCalibration.right
                                    anchors.leftMargin: 30
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 6
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
                                    anchors.left: btnSaveCalibration.right
                                    anchors.leftMargin: 30
                                    anchors.bottom: btnSaveCalibration.top
                                    anchors.bottomMargin: 0
                                    visible: false
                                    //anchors.left: parent.left
                                }
                                ButtonPerso {
                                    id: btnSaveCalibration
                                    width: 100
                                    height: 28
                                    text: qsTr("Save")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    onClicked: {
                                        // Save register permanently
                                        tO2Event.saveRegisterPermanently();
                                        // Valide : Retour sur la slide précédente
                                        swipeViewO2Calibration.currentIndex = 0;
                                        // Activation de la slide 0
                                        firstCalibSwipe.enabled = true
                                        // Désactivation end of calibration
                                        endCalibrationBloc.enabled = false
                                    }
                                }
                            }

                            Rectangle {
                                id: rectangleSCDummy2
                                width: 200
                                height: 200
                                color: "#ffffff"
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: ( parent.width - 15 ) / 3

                                ButtonPerso {
                                    id: btnConfirmZeroPCalib3
                                    x: -8
                                    y: 6
                                    width: 100
                                    height: 28
                                    text: qsTr("Return")
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    anchors.rightMargin: 0
                                    onClicked: {
                                        // Gestion de l'affichage
                                        vFReturnFromEndCalibration()
                                    }
                                }
                            }

                        }
                    }
                    anchors.horizontalCenterOffset: 0
                } // Bloc sensor settings
            }
            Item{
                id: thirdCalibSwipe

            }
            Item{
                id: fourthCalibSwipe

            }
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


