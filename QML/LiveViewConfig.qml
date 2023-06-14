import QtQuick 2.0
import QtQuick.Controls 2.13

import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.3

import "Component"
import "Windows"

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property int uiBlocWidthOffset: 60


    //=============================================================
    // Signal
    //=============================================================
    signal stopMeasureSignal()

    //=============================================================
    // Property
    //=============================================================
    property string m_sCh0Name: ""
    property string m_sCh1Name: ""
    property string m_sCh2Name: ""
    property int    m_uiAcquisitionTime: 0
    property bool   m_bAverageStarted: false
    property double m_dAverageSampleNumber: 0.0
    property double m_dAverageValue: 0.0
    property double m_dAverageValueResult: 0.0
    property int m_uiChannelNumber: 0


    //=============================================================
    // Connection
    //=============================================================
    Component.onCompleted:{
        console.log(" LIVEVIEW => Component.onCompleted ");
    }
    Connections {
        target:tLiveviewConfEvent
        //-------------------------------------------------
        // Affichage des donneés
        //-------------------------------------------------
        // Appelé seulement sur refresh général
        function onDisplayLiveViewConfSignal( sCh0Name,
                                              sCh1Name,
                                              sCh2Name,
                                              uiAcquisitionTime,
                                              uiChannelNumber ) {
            console.log("onDisplayLiveViewConfSignal")
            //console.log( "=========onDisplayLiveViewConfSignal=======" );
            //console.log( "=========onDisplayLiveViewConfSignal=======" );
            //console.log( "=========onDisplayLiveViewConfSignal=======" );
            //console.log( "sCh0Name : "+sCh0Name );
            //console.log( "sCh1Name : "+sCh1Name );
            //console.log( "sCh2Name : "+sCh2Name );
            m_sCh0Name = sCh0Name
            m_sCh1Name = sCh1Name
            m_sCh2Name = sCh2Name
            console.log( "uiAcquisitionTime = "+ uiAcquisitionTime );
            m_uiAcquisitionTime = uiAcquisitionTime
            m_uiChannelNumber = uiChannelNumber;

            var uiMinAcquisitionPeriod = 0;
            if( tMainData.bCOMSlowMode || tMainData.bNkeMode )
                //uiMinAcquisitionPeriod = ( parseInt( uiAcquisitionTime ) / parseInt( 1000 ) ) + parseInt( 4 )
                uiMinAcquisitionPeriod = ( parseInt( uiAcquisitionTime ) / parseInt( 1000 ) )
            else
                uiMinAcquisitionPeriod = ( parseInt( uiAcquisitionTime ) / parseInt( 1000 ) ) + parseInt( 1 )
            // Si cadence actuelle inférieur au minimum pour le capteur
            if( uiMainMeasurePeriod < uiMinAcquisitionPeriod ){
                // Modification de la cadence
                uiMainMeasurePeriod = uiMinAcquisitionPeriod
            }
            inputMP.from = uiMinAcquisitionPeriod
            // On remplit le combobox
            inputMP1Model.clear()
            inputMP1Model.append( {text:m_sCh0Name} )
            inputMP1.currentIndex = 0
            if( uiChannelNumber > 1 )
                inputMP1Model.append( {text:m_sCh1Name} )
            if( uiChannelNumber > 2 )
                inputMP1Model.append( {text:m_sCh2Name} )
        }
        //-------------------------------------------------
        // Réception d'une nouvelle donnée
        //-------------------------------------------------
        function onTransmitLiveViewDataSignal( fCh0Data,
                                               fCh1Data,
                                               fCh2Data ) {
            console.log("==onTransmitLiveViewDataSignal==")
            // Si Pas de demande de moyenne
            if( !m_bAverageStarted ) return;

            console.log( "=========onTransmitLiveViewDataSignal=======" );
            console.log( "=========onTransmitLiveViewDataSignal=======" );
            console.log( "=========onTransmitLiveViewDataSignal=======" );
            console.log( "fCh0Data : "+fCh0Data );
            console.log( "fCh1Data : "+fCh1Data );
            console.log( "fCh2Data : "+fCh2Data );
            // Incrément du nombre d'échantillon
            m_dAverageSampleNumber = m_dAverageSampleNumber + 1
            // Selon l'index
            switch( inputMP1.currentIndex )
            {
                // Voie 0
                case 0:
                    // Ajout de l'échantillon
                    m_dAverageValue = m_dAverageValue + fCh0Data
                break;
                // Voie 1
                case 1:
                    // Ajout de l'échantillon
                    m_dAverageValue = m_dAverageValue + fCh1Data
                break;
                // Voie 2
                case 2:
                    // Ajout de l'échantillon
                    m_dAverageValue = m_dAverageValue + fCh2Data
                break;
                // Default
                default:
                break;
            }
            // %%AP - 2021.10.11 - Calcul et affichage de la moyenne
            dFComputeAverage()
        }
    }

    //-------------------------------------------------------------
    // Démarrage mesure
    //-------------------------------------------------------------
    function vFStartMesure(){
        console.log("DEMARRAGE DE LA MESURE")
        // Au démarrage la moyenne est forcément désactivée
        m_bAverageStarted = false
    }
    function vFStartAverage(){
        console.log("vFStartAverage")
        // Activation du moyennage
        m_bAverageStarted = true
        // Mise à zero de la valeur moyenne
        m_dAverageValue = 0.0
        m_dAverageSampleNumber = 0;
    }

    //-------------------------------------------------------------
    // Stop mesure
    //-------------------------------------------------------------
    function vFStopMesure(){
        console.log("STOP DE LA MESURE")
        // Fin de la moyenne
        vFStopAverage(false);
    }
    function dFComputeAverage(){
        console.log("dFComputeAverage")
        // Calcul du résultat
        var m_dAverageValueTemp = m_dAverageValue / m_dAverageSampleNumber
        // Test si la voie est a une calibration d'activée
        if( tLiveviewConfEvent.bFIsCalibrationTypeActivated( inputMP1.currentIndex ) ){
            // C'est une valeur physique
            // Conservation de 3 chiffre après la virgule
            m_dAverageValueResult = parseFloat(parseInt( m_dAverageValueTemp * 1000 ))/1000
        }
        else{
            // Conservation de 1 chiffre après la virgule
            m_dAverageValueResult = parseFloat(parseInt( m_dAverageValueTemp * 10 ))/10
        }
    }
    function vFStopAverage(bWindowsOpen){
        // Si la moyenne n'est pas activée
        if( !m_bAverageStarted ) return;
        console.log("=> vFStopAverage")
        console.log( "m_dAverageValue = " + m_dAverageValue )
        console.log( "m_dAverageSampleNumber = " + m_dAverageSampleNumber )
        m_bAverageStarted = false
        // Calcul du résultat
        m_dAverageValue = m_dAverageValue / m_dAverageSampleNumber

        console.log( "tLiveviewConfEvent.bFIsCalibrationTypeActivated( m_uiChannelNumber ) = " );
        console.log( tLiveviewConfEvent.bFIsCalibrationTypeActivated( inputMP1.currentIndex ) );
        // Test si la voie est a une calibration d'activée
        if( tLiveviewConfEvent.bFIsCalibrationTypeActivated( inputMP1.currentIndex ) ){
            // C'est une valeur physique
            // Conservation de 3 chiffre après la virgule
            m_dAverageValueResult = parseFloat(parseInt( m_dAverageValue * 1000 ))/1000
        }
        else{
            // Conservation de 1 chiffre après la virgule
            m_dAverageValueResult = parseFloat(parseInt( m_dAverageValue * 10 ))/10
            // Ajout du résultat dans le quick add
            inputCalibPointRaw.text = m_dAverageValueResult
            if( bWindowsOpen ){
                // Ouverture de la fenêtre : "Do you want to stop measure to add calib point ?"
                windowStopAverageStopMeasure.open()
            }
        }
        // Affichage du résultat de la moyenne
        //txtResultACRaw.text = m_dAverageValueResult
    }
    //-------------------------------------------------------------
    // Fin du busy du add calibration point
    //-------------------------------------------------------------
    function vFEndOfBusyState(){
        busyCalibPoints.visible = false
    }

    //=============================================================
    // Objet
    //=============================================================

    //*************************************************************
    // Le code de l'objet
    //*************************************************************
    id: liveViewConfigID
    width: window.width - 228
    height: 750
    implicitWidth: 662
    implicitHeight: 800

    /*
    // Background color
    Rectangle {
        anchors.fill: parent
        color: "#efeef4"
        z: -9
    }
    */

    //-------------------------------
    // Bouton return
    //-------------------------------
    LiveviewASButtonReturn{
        id: m_buttonReturn
        anchors.top: parent.top
        anchors.topMargin: 28
        anchors.left: parent.left
        anchors.leftMargin: 2
        onClicked:{
            //m_liveviewID.returnClicked()
            liveViewStackView.decrementCurrentIndex()
        }
    }

    // Background
    Rectangle {
        id: liveViewConfigBackground
        width: parent.width
        //height: 693
        height: liveViewConfigID.height
        color: "#00efeef4"

        //-------------------------------
        // Titre H2
        //-------------------------------
        H2Item{
            id: m_H2ItemID
            sH2Title: "Liveview advanced settings"
            sH2Img: "qrc:/Logo/LiveView/Realtime.png"
            uiLogoWidth: 34
            anchors.top: parent.top
            z: 3
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 50
            //bHideTriangle: true
        }

        //-------------------------------------------------------------
        // Bloc : liveview acquisition settings
        //-------------------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: acquisitionSettingsBloc
            samples: 30
            horizontalOffset: 0
            source: acquisitionSettingsBloc
        }
        Rectangle {
            id: acquisitionSettingsBloc
            //x: 8
            y: 104

            width: liveViewConfigID.width - 120
            //height: 204
            height: 175
            color: "#ffffff"
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: acqSettingsTitleBgd
                x: 0
                y: 0
                width: parent.width
                height: 37
                color: "#e4e3e9"
                Text {
                    id: acqSettingsTitleTxt
                    x: 264
                    y: 12
                    text: qsTr("Acquisition settings")
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: openSansDemiBoldFont.name
                }
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
            }

            ColumnLayout {
                id: columnLayoutAcqSettings
                x: 0
                y: 41
                width: parent.width
                height: parent.height - 37 - 46

                RowLayout {
                    id: rowMeasurementPeriod
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    //Row {
                    Rectangle {
                        id: rowMeasurementPeriod2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgMP
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/LiveViewSettings/liveview-measurementPeriod.png"
                        }

                        Text {
                            id: txtMP
                            text: qsTr("Measurement period")
                            height: parent.height
                            anchors.left: rowMeasurementPeriod2.left
                            anchors.leftMargin: 35
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    SpinBox {
                        id: inputMP
                        height: 30
                        from: 1
                        to: 100
                        editable: true
                        //model: [ "1", "2", "5" ]
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        enabled: !bStarMeasureSwitchState
                        //%%AP - NOP - selectByMouse: true
                    }
                    Rectangle {
                        id: rectangleMPDummy
                        width: 200
                        height: 200
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            text: qsTr("seconds")
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                } // End row:

                // Ligne
                Rectangle {
                    id: measurementPeriodLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //-------------------------------------------------------------------
                // Graphical depth
                //-------------------------------------------------------------------
                RowLayout {
                    id: rowGraphicalDepth

                    width: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.preferredWidth: parent.width - 20
                    //Row {
                    Rectangle{
                        id: rowGraphicalDepth3
                        width: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Image {
                            id: imgGraphicalDepth
                            width: 30
                            height: 20
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Logo/LiveViewSettings/liveview-graphDepth.png"
                        }

                        Text {
                            id: txtGraphicalDepth
                            height: parent.height
                            text: qsTr("Graphical depth")
                            anchors.left: rowGraphicalDepth3.left
                            anchors.leftMargin: 35
                            font.pixelSize: 12
                            font.family: "open sans"
                            font.weight: Font.DemiBold
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    ComboBox {
                        id: inputGraphicalDepth
                        height: 30
                        model: [ "Unlimited", "32", "128", "512", "2048" ]
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        enabled: !bStarMeasureSwitchState
                    }

                    Rectangle {
                        id: rectangleGraphicalDepthDummy1
                        width: 200
                        height: 200
                        color: "#00000000"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                }

                // Ligne
                Rectangle {
                    id: graphicalDepthLine1
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

            } // Column layout end
            Button {
                id: btnConfirmAcqSettings
                width: 100
                height: 28
                text: qsTr("Submit")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                font.pointSize: 9
                font.family: "open sans"
                Layout.preferredHeight: 30
                Layout.minimumHeight: 0
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                enabled: !bStarMeasureSwitchState

                onClicked: {
                    var uiGraphicalDepth = parseInt(0);

                    // Affichage du logo busy
                    timerConfirmInfo.start()
                    busyConfirmInfo.visible = true
                    vFLockUserClick();
                    // Modification de la période de mesure
                    uiMainMeasurePeriod = parseInt( inputMP.value )

                    switch( inputGraphicalDepth.currentIndex ){
                        // 0 : "Unlimited"
                        case( 0 ):
                            uiGraphicalDepth = parseInt(0);
                        break;
                        // 1 : "32"
                        case( 1 ):
                            uiGraphicalDepth = parseInt(32);
                        break;
                        // 2 : "128"
                        case( 2 ):
                            uiGraphicalDepth = parseInt(128);
                        break;
                        // 3 : "512"
                        case( 3 ):
                            uiGraphicalDepth = parseInt(512);
                        break;
                        // 4 : "2048"
                        case( 4 ):
                            uiGraphicalDepth = parseInt(2048);
                        break;
                        // Default
                        default:
                        break;
                    }

                    // Modification de la profondeur d'affichage des graphiques
                    liveViewPart.vFLiveViewPart_UpdateGraphicalDepth( uiGraphicalDepth );
                }
            }
            Timer {
                id: timerConfirmInfo
                interval: 2200; running: false; repeat: false
                onTriggered: {
                    vFAllowUserClick();
                    busyConfirmInfo.visible = false
                }
            }
            BusyIndicatorPerso {
                id: busyConfirmInfo
                width: 30
                height: 30
                anchors.verticalCenter: btnConfirmAcqSettings.verticalCenter
                anchors.left: btnConfirmAcqSettings.right
                anchors.leftMargin: 18
                visible: false
            }
        }



        //-------------------------------------------------------
        // Bloc : Channel selection
        //-------------------------------------------------------
         DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: liveviewChannelSelectRect
            samples: 30
            horizontalOffset: 0
            source: liveviewChannelSelectRect
        }
        Rectangle {
            id: liveviewChannelSelectRect

            width: liveViewConfigID.width - 120
            height: 80
            color: "#ffffff"
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: acquisitionSettingsBloc.bottom
            anchors.topMargin: 28
            Rectangle {
                id: liveviewChannelSelectRectTitle
                x: 0
                y: 0
                width: parent.width
                height: 37
                color: "#e4e3e9"
                Text {
                    id: liveviewChannelSelectTitleTxt
                    x: 264
                    y: 12
                    text: qsTr("Working channel selection")
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: openSansDemiBoldFont.name
                }
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
            }

            ColumnLayout {
                id: columnLayoutLiveviewChannelSelect
                x: 0
                //y: 41
                width: parent.width
                height: parent.height - 37 /*- 46*/
                anchors.top: liveviewChannelSelectRectTitle.bottom
                //-------------------------------------------------------------------
                // Channel selection
                //-------------------------------------------------------------------
                RowLayout {
                    id: rowACChannel1

                    width: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.preferredWidth: parent.width - 20
                    //Row {
                    Rectangle{
                        id: rowACChannel3
                        width: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Image {
                            id: imgACChannel1
                            width: 28
                            height: 20
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Logo/LiveViewSettings/liveview-channelSelection.png"
                        }

                        Text {
                            id: txtACChannel1
                            height: parent.height
                            text: qsTr("Used channel")
                            anchors.left: rowACChannel3.left
                            anchors.leftMargin: 35
                            font.pixelSize: 12
                            font.family: "open sans"
                            font.weight: Font.DemiBold
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    ComboBox {
                        id: inputMP1
                        height: 30
                        model: ListModel {
                            id: inputMP1Model
                        }
                        //model: [ "Temperature", "Conductivity", "Pressure" ]
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        //enabled: bStarMeasureSwitchState && m_bStartAverage
                        enabled: ( m_bAverageStarted == false )
                    }

                    Rectangle {
                        id: rectangleMPDummy1
                        width: 200
                        height: 200
                        color: "#00000000"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                }
            }
        }

        //-------------------------------------------------------
        // Bloc : Average calculation
        //-------------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: averageCalculationBloc
            samples: 30
            horizontalOffset: 0
            source: averageCalculationBloc
        }
        Rectangle {
            id: averageCalculationBloc
            //x: -4
            width: liveViewConfigID.width - 120
            //height: 134
            height: 125
            color: "#ffffff"
            anchors.top: liveviewChannelSelectRect.bottom
            anchors.topMargin: 28
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: averageCalculationTitleBgd1
                x: 0
                y: 0
                width: parent.width
                height: 37
                color: "#e4e3e9"
                Text {
                    id: averageCalculationTitleTxt1
                    x: 264
                    y: 12
                    text: qsTr("Average calculation")
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: openSansDemiBoldFont.name
                }
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
            }

            ColumnLayout {
                id: columnLayoutACChannel1
                x: 0
                y: 42
                width: parent.width
                height: parent.height - 37 - 10

                //-------------------------------------------------------------------
                // Start/stop average calculation
                //-------------------------------------------------------------------
                RowLayout {
                    id: rowStartStopAC
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.preferredWidth: parent.width - 20
                    width: parent.width - 20
                    //Row {
                    Rectangle{
                        id: rowStartStopAC2
                        width: ( parent.width - 25 ) / 3
                        height: parent.height
                        Image {
                            id: imgStartStopAC
                            width: 23
                            height: 23
                            transformOrigin: Item.Center
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Logo/LiveViewSettings/liveview-average.png"
                        }

                        Text {
                            id: txtStartStopAC
                            height: parent.height
                            text: qsTr("Average calculation")
                            anchors.left: rowStartStopAC2.left
                            anchors.leftMargin: 35
                            font.pixelSize: 12
                            font.family: "open sans"
                            font.weight: Font.DemiBold
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }


                    Rectangle {
                        id: rectangleStartACDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        Button {
                            id: btnStartAC
                            width: 100
                            height: 28
                            text: qsTr("Start")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 9
                            Layout.minimumHeight: 0
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            font.family: "open sans"
                            Layout.preferredHeight: 30
                            enabled: bStarMeasureSwitchState && !m_bAverageStarted
                            onClicked: {
                                vFStartAverage();
                            }
                        }
                    }
                    Rectangle {
                        id: rectangleStopACDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Button {
                            id: btnStopAC
                            width: 100
                            height: 28
                            text: qsTr("Stop")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 9
                            Layout.minimumHeight: 0
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            font.family: "open sans"
                            Layout.preferredHeight: 30
                            enabled: bStarMeasureSwitchState && m_bAverageStarted

                            onClicked: {
                                vFStopAverage(true);
                            }
                        }
                    }

                }


                // Ligne
                Rectangle {
                    id: startStopACLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //-------------------------------------------------------------------
                // Average calculation result
                //-------------------------------------------------------------------
                RowLayout {
                    id: rowResultAC
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.preferredWidth: parent.width - 20
                    width: parent.width - 20

                    //Row {
                    Rectangle{
                        id: rowResultAC2
                        width: ( parent.width - 25 ) / 3
                        height: parent.height
                        Image {
                            id: imgResultAC
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Logo/LiveViewSettings/liveview-result.png"
                        }

                        Text {
                            id: txtResultAC
                            height: parent.height
                            text: qsTr("Result")
                            anchors.left: rowResultAC2.left
                            anchors.leftMargin: 35
                            font.pixelSize: 12
                            font.family: "open sans"
                            font.weight: Font.DemiBold
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }


                    Rectangle {
                        id: rectangleResultACDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        Text {
                            id: txtResultACRaw
                            width: 100
                            height: 28
                            text: (m_dAverageValueResult!=0.0)?(m_dAverageValueResult):qsTr("--")
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            anchors.verticalCenter: parent.verticalCenter
                            //font.pointSize: 9
                            Layout.minimumHeight: 0
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            Layout.preferredHeight: 30
                        }
                    }
                    Rectangle {
                        id: rectangleResultACDummy2
                        width: 200
                        height: 200
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }

                }
                Timer {
                    id: timerConfirmInfo1
                    interval: 2200
                    running: false
                    repeat: false
                }
            }
        }

        //-------------------------------------------------------
        // Bloc : Add to calibration point
        //-------------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: addToCalibPointBloc
            samples: 30
            horizontalOffset: 0
            source: addToCalibPointBloc
            visible: !bO2
        }
        Rectangle {
            id: addToCalibPointBloc
            x: 2
            width: liveViewConfigID.width - 120
            //height: 182
            height: 175
            color: "#ffffff"
            anchors.top: averageCalculationBloc.bottom
            anchors.topMargin: 28
            anchors.horizontalCenterOffset: 0
            visible: !bO2
            Rectangle {
                id: acqSettingsTitleBgd2
                x: 0
                y: 0
                width: parent.width
                height: 37
                color: "#e4e3e9"
                Text {
                    id: acqSettingsTitleTxt2
                    x: 264
                    y: 12
                    text: qsTr("Add to calibration point")
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: openSansDemiBoldFont.name
                }
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
            }

            ColumnLayout {
                id: columnLayoutACChannel2
                x: 0
                y: 41
                width: parent.width
                height: parent.height - 37 - 46
                //----------------------------------------------------------------
                // Calibration point - physical
                //----------------------------------------------------------------
                RowLayout {
                    id: rowCalibPointPhysical
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.preferredWidth: parent.width - 20
                    width: parent.width - 20
                    //Row {
                    Rectangle {
                        id: rowACChannel4
                        width: ( parent.width - 25 ) / 3
                        height: parent.height
                        Image {
                            id: imgCalibPointPhysical
                            width: 30
                            height: 20
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Logo/LiveViewSettings/liveview-physical.png"
                        }

                        Text {
                            id: txtCalibPointPhysical
                            height: parent.height
                            text: qsTr("Physical")
                            anchors.left: rowACChannel4.left
                            anchors.leftMargin: 35
                            font.pixelSize: 12
                            font.family: "open sans"
                            font.weight: Font.DemiBold
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }

                    TextField {
                        id: inputCalibPointPhysical
                        height: 30
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        selectByMouse: true
                        ToolTip {
                            id: tooltipCalibPointPhysical
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
                        id: rectangleCalibPointPhysicalDummy2
                        width: 200
                        height: 200
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                }

                Rectangle {
                    id: calibPointPhysicalRawLine2
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //----------------------------------------------------------------
                // Calibration point - raw
                //----------------------------------------------------------------
                RowLayout {
                    id: rowStartStopAC1
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.preferredWidth: parent.width - 20
                    width: parent.width - 20
                    //Row {
                    Rectangle{
                        id: rowCalibPointRaw
                        width: ( parent.width - 25 ) / 3
                        height: parent.height
                        Image {
                            id: imgCalibPointRaw
                            width: 30
                            height: 20
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Logo/LiveViewSettings/liveview-raw.png"
                        }

                        Text {
                            id: txtCalibPointRaw
                            height: parent.height
                            text: qsTr("Raw")
                            anchors.left: rowCalibPointRaw.left
                            anchors.leftMargin: 35
                            font.pixelSize: 12
                            font.family: "open sans"
                            font.weight: Font.DemiBold
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }


                    TextField {
                        id: inputCalibPointRaw
                        height: 30
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        selectByMouse: true
                        ToolTip {
                            id: tooltipCalibPointRaw
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
                        id: rectangleCalibPointRawDummy
                        width: 200
                        height: 200
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                }

                // Ligne séparation
                Rectangle {
                    id: calibPointRawLine1
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }



                Timer {
                    id: timerConfirmInfo2
                    interval: 2200
                    running: false
                    repeat: false
                }
            }
            anchors.horizontalCenter: parent.horizontalCenter
            //----------------------------------------------------------------------------
            // Calibration point - Submit button
            //----------------------------------------------------------------------------
            Button {
                id: btnConfirmCalibPoint
                width: 100
                height: 28
                text: qsTr("Submit")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                font.pointSize: 9
                Layout.minimumHeight: 0
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.family: "open sans"
                Layout.preferredHeight: 30
                enabled: !bStarMeasureSwitchState

                onClicked:{
                    // Passage en busy
                    busyCalibPoints.visible   = true

                    var fRaw = parseFloat(inputCalibPointRaw.text);
                    var fPhy = parseFloat(inputCalibPointPhysical.text);
                    var sRaw = inputCalibPointRaw.text.toString();
                    var sPhy = inputCalibPointPhysical.text.toString();
                    // Pour vérification du nombre de chiffre après la virgule
                    var fphyRounded = parseFloat(parseInt(fPhy * 1000))/1000;
                    var fRawRounded = parseFloat(parseInt(fRaw * 10))/10;

                    // Phy : Test si max step est bien à 0.001
                    //%%AP - 2020.11.24 - Ajout test de sécurité
                    // Test si nombre décimal
                    if( sPhy.split('.').length == 2 ){
                        // Test si 0.001 max
                        if(  ( sPhy.split('.')[1].length <= 0 )
                          || ( sPhy.split('.')[1].length >  3 ) ){
                            // Passage en erreur
                            errorCalibPoints.visible  = true
                            //calibrationID.bErrorState = true
                            tooltipCalibPointPhysical.show(qsTr("Please enter a value with a maximum of 3 decimal places"))
                            return;
                        }
                    }
                    // Phy : Test si max step est bien à 0.001
                    if(  ( fPhy > 6000 )
                      || ( fPhy < -6000 ) ){
                        // Passage en erreur
                        errorCalibPoints.visible  = true
                        //calibrationID.bErrorState = true
                        tooltipCalibPointPhysical.show(qsTr("Value must be between -6000 and 6000"))
                        return;
                    }
                    // Raw : Test si max step est bien à 0.1
                    //%%AP - 2020.11.24 - Ajout test de sécurité
                    // Test si nombre décimal
                    if( sRaw.split('.').length == 2 ){
                        // Raw : Test si max step est bien à 0.1
                        if(  ( sRaw.split('.')[1].length <= 0 )
                          || ( sRaw.split('.')[1].length >  1 ) ){
                            // Passage en erreur
                            errorCalibPoints.visible  = true
                            //calibrationID.bErrorState = true
                            tooltipCalibPointRaw.show(qsTr("Please enter a value with a maximum of 1 decimal places"))
                            return;
                        }
                    }
                    // Raw : Test si max step est bien à 0.001
                    if(  ( fRaw > 262143 )
                      || ( fRaw < 0      ) ){
                        // Passage en erreur
                        errorCalibPoints.visible  = true
                        //calibrationID.bErrorState = true
                        tooltipCalibPointRaw.show(qsTr("Value must be between 0 and 262143"))
                        return;
                    }
                    // Avant vérification : définie dans CalibrationPart la voie de mesure courante
                    calibrationPart1.vFSetCurrentChannel( inputMP1.currentIndex )
                    //--------------------
                    // Vérification que le point n'existe pas déjà
                    //--------------------
                    if( calibrationPart1.bFIsPhyPointAlreadyExist( fPhy, inputMP1.currentIndex ) ){
                        // Passage en erreur
                        errorCalibPoints.visible  = true
                        //calibrationID.bErrorState = true
                        tooltipCalibPointPhysical.show(qsTr("The point already exists"))
                        return;
                    }
                    if( calibrationPart1.bFIsRawPointAlreadyExist( fRaw, inputMP1.currentIndex ) ){
                        // Passage en erreur
                        errorCalibPoints.visible  = true
                        //calibrationID.bErrorState = true
                        tooltipCalibPointRaw.show(qsTr("The point already exists"))
                        return;
                    }
                    // Quitte le popup
                    tooltipCalibPointPhysical.hide();
                    tooltipCalibPointRaw.hide();

                    // Test
                    var uiPointNumber = calibrationPart1.uiFGetCalibrationPointNumber( inputMP1.currentIndex );
                    // Si on a atteint le nombre max de point de calibration
                    if( uiPointNumber >= 14 ){
                        // Affichage tooltip d'erreur
                        tooltipCalibPointRaw.show(qsTr("Max calibration point number reached"))
                        // Affichage logo erreur
                        errorCalibPoints.visible  = true
                        // On quitte
                        return;
                    }

                    // Appel d'une fonction CPP pour enregistrement des points
                    //calibrationID.addCalibPoint( calibrationID.currentChannel, fphy, fRaw );
                    calibrationPart1.vFAddCalibrationPoint( inputMP1.currentIndex, fPhy, fRaw )
                    // Sauvegarde de la structure de configuration vers le capteur
                    //inputDateTime.text = calibrationID.sSaveCurrentCalibrationDateTime( calibrationID.currentChannel );
                    calibrationPart1.vFSaveCurrentCalibrationDateTime( inputMP1.currentIndex )
                    // Sauvegarde de la configuration
                    calibrationPart1.vFSaveConfiguration( inputMP1.currentIndex );
                    // Lancement du timer
                    timerSubmitAddCalibrationPoint.start();
                }
            }

            // Timer pour le fonctionnement du submit add calibration point
            Timer {
                id: timerSubmitAddCalibrationPoint
                //%%AP - 2022.11.09 - 100ms par défaut
                interval: 100;
                //interval: 1;
                running: false;
                repeat: true;
                property bool bWriteDone: false
                property bool bReadDone: false
                onTriggered: {
                    // Tant que le grafcet est en busy
                    if( tMainEvent.isModbusBusyState() ){
                        return;
                    }
                    // Debug
                    console.log( "==timerSubmitCalibrationConfig==" );
                    console.log( "inputMP1.currentIndex = "+inputMP1.currentIndex );
                    if( !timerSubmitAddCalibrationPoint.bWriteDone ){
                        timerSubmitAddCalibrationPoint.bWriteDone = true;
                        tMainEvent.getChannelConfiguration( inputMP1.currentIndex );
                        return;
                    }
                    // Stop du timer
                    timerSubmitAddCalibrationPoint.stop();
                    bWriteDone = false;
                    bReadDone  = false;
                    // Passage en busy
                    busyCalibPoints.visible   = false;
                    //calibrationID.bBusyState  = false;
                }
            }

            //--------------------
            // Busy : calibration settings
            //--------------------
            BusyIndicatorPerso {
                id: busyCalibPoints
                width: 31
                height: 32
                anchors.left: btnConfirmCalibPoint.right
                anchors.leftMargin: 19
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                visible: false
                onVisibleChanged: {
                    if( visible ) errorCalibPoints.visible = false
                }
            }
            //--------------------
            // Error : calibration settings
            //--------------------
            //onErrorCalibPointsChanged:{
            //}
            Image {
                id: errorCalibPoints
                width: 31
                height: 31
                source: "qrc:/Logo/Global/No_Cross.png"
                anchors.left: btnConfirmCalibPoint.right
                anchors.leftMargin: 19
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                visible: false
                onVisibleChanged: {
                    if( visible ) busyCalibPoints.visible = false
                }
            }
        }

    }

    //---------------------------------------------------------
    // Fenêtre arrêt mesure
    //---------------------------------------------------------
    MessageDialog {
        id: windowStopAverageStopMeasure
        title: qsTr("Stop average")
        //icon: StandardIcon.Question
        text: qsTr("Would you like to add current calibration point ?")+
         "\r\n"+qsTr("(mesure will be stopped)")
        //detailedText: "Current version : "+sVersionNumber+
        //   "Do you want to update now ?"
        standardButtons: StandardButton.Yes | StandardButton.No
        modality: Qt.ApplicationModal

        onYes: {
            // On stoppe la mesure
            console.log("windowStopAverageStopMeasure > Stop measure")
            // Fin d'un fichier
            //vFMAINStopMeasureWindowsQwt();
            // Stop de la mesure
            liveViewConfigID.stopMeasureSignal()
            // Stop
            bStarMeasureSwitchState = false;
        }
        onNo: {
            console.log("Don't start submit")
        }
        onRejected: {
            console.log("Rejected")
        }
    }

}



/*##^##
Designer {
    D{i:41;anchors_y:270}D{i:81;anchors_x:423;anchors_y:0}D{i:63;anchors_y:488}
}
##^##*/
