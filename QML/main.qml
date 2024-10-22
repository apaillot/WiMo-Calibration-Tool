import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Fusion 2.12
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

import LibWiper 1.0

import "Component"
import "Windows"

Window {
    //==============================================================
    // Déclaration des types
    //==============================================================
    property int eTAB_DASHBOARD:           0
    property int eTAB_GENERIC_LIVEVIEW:    1
    property int eTAB_GENERIC_CALIBRATION: 2
    property int eTAB_WIPER:               3
    property int eTAB_FACTORY:             4
    property int eTAB_O2:                  5

    // Variable de taille pour les logos capteurs
    property var ptLogoScale: {
        0:0.4, // S00 - Pressure static
        1:0.4, // S01 - Pressure keller
        2:0.4, // S02 - Temperature keller
        3:0.4, // S03 - Temperature
        4:0.4, // S04 - Conductivity

        5:0.4, // S05 - Turbidity
        6:0.4, // S06 - Oxygen concentration
        7:0.4, // S07 - Oxygen saturation
        8:0.4, // S08 - pH

        9:0.5, // S09 - Turner Chla
        10:0.5, // S10 - Turner phycoC
        11:0.5, // S11 - Turner phycoE
        12:0.5, // S12 - CDOM

        13:0.5, // S13 - Fluo Chla
        14:0.5, // S14 - PAR
        15:0.5, // S15 - Fluo phycoC
        16:0.5, // S16 - Fluo phycoE
        17:0.5, // S17 - Fluo CDOM
        18:0.5, // S18 - Fluo Trypto
        19:0.5, // S19 - Turner Trypto
        20:0.5, // S20 - Redox
        21:0.5, // S21 - Ammonium
        22:0.5, // S22 - Potassium
        23:0.5, // S23 - Nitrate
        24:0.5, // S24 - ISE-5
        25:0.5, // S25 - O2 temperature
        60:0.4,
        "unknow":0.4,
        "wiper":0.4,
        "cleaner":0.4
    }
    property var ptColor: {
        0:"#03A9F4", // S00 - Pressure static
        1:"#03A9F4", // S01 - Pressure keller
        2:"#F44336", // S02 - Temperature keller
        3:"#F44336", // S03 - Temperature
        4:"#26A69A", // S04 - Conductivity

        5:"#048B9A", // S05 - Turbidity
        6:"#193D8A", // S06 - Oxygen concentration
        //7:"#193D8A", // S07 - Oxygen saturation
        7:"#4F6CAB", // S07 - Oxygen saturation
        8:"#B0C0E0", // S08 - pH

        9:"#8BC34A", // S09 - Turner Chla
        10:"#285C8C", // S10 - Turner phycoC
        11:"#9B0015", // S11 - Turner phycoE
        12:"#A1CF62", // S12 - CDOM

        13:"#8BC34A", // S13 - Fluo Chla
        14:"#FFB300", // S14 - PAR
        15:"#285C8C", // S15 - Fluo phycoC
        16:"#9B0015", // S16 - Fluo phycoE
        17:"#A1CF62", // S17 - Fluo CDOM
        18:"#CC3399", // S18 - Fluo Trypto
        19:"#CC3399", // S19 - Turner Trypto
        20:"#E23E42", // S20 - Redox
        21:"#1A4973", // S21 - Ammonium
        22:"#77807D", // S22 - Potassium
        23:"#339966", // S23 - Nitrate
        24:"#0099FF", // S24 - ISE-5
        25:"#FE1B00", // S25 - O2 temperature
        26:"#4F463F", // S26 - Crude oil Turner
        27:"#4F463F", // S27 - Crude oil nke
        60:"#B0C0E0",
        "unknow":"#DA4624",
        "wiper":"#A45883",
        "cleaner":"#590098",
        "disabled":"#c0c5ce"
    }

    //==============================================================
    // Déclaration des variables
    //==============================================================
    // Version du programme
    property string sVersionNumber: qsTr("error")
    // Page affiché par défaut à l'ouverture du logiciel
    property int uiPageSelected: eTAB_DASHBOARD
    // Couleur
    property color windowsBackgroundColor: "#efeef4"
    property color liNavColor: "#11337b"
    property color liNavBorderColor: "#11337b"
    property color liNavHoverColor: "#1D459B"
    property color liNavBorderHoverColor: "white"
    ///%%AP - 2020.09.21 - DEBUG forcé à true
    property bool  bGenericSensor: false;
    //property bool  bGenericSensor: true;
    property bool  bNkeMode: tMainData.bNkeMode;
    property bool  bNkeProdMode: tMainData.bProdMode;
    //property bool  bNkeMode: true;
    property bool  bWiper: false;
    property bool  bO2: false;
    // Hauteur de la page factory
    property int uiFactoryHeight: 890
    // Port COM utilisé
    property string sPortCOMMain: tDataCom.sComPort


    //---------------------------------------------
    // Déclaration des variables
    //---------------------------------------------
    property double dCh0Value: 0
    property double dCh1Value: 0
    property double dCh2Value: 0

    //=============================================================
    // Déclaration des polices de caractère
    //=============================================================
    FontLoader { id: openSansBoldFont; source: "qrc:/Font/OpenSans-Bold.ttf" }
    FontLoader { id: openSansBoldItalicFont; source: "qrc:/Font/OpenSans-BoldItalic.ttf" }
    FontLoader { id: openSansExtraBoldFont; source: "qrc:/Font/OpenSans-ExtraBold.ttf" }
    FontLoader { id: openSansExtraBoldItalicFont; source: "qrc:/Font/OpenSans-ExtraBoldItalic.ttf" }
    FontLoader { id: openSansItalicFont; source: "qrc:/Font/OpenSans-Italic.ttf" }
    FontLoader { id: openSansLightFont; source: "qrc:/Font/OpenSans-Light.ttf" }
    FontLoader { id: openSansLightItalicFont; source: "qrc:/Font/OpenSans-LightItalic.ttf" }
    FontLoader { id: openSansFont; source: "qrc:/Font/OpenSans-Regular.ttf" }
    FontLoader { id: openSansDemiBoldFont; source: "qrc:/Font/OpenSans-Semibold.ttf" }
    FontLoader { id: openSansDemiBoldItalicFont; source: "qrc:/Font/OpenSans-SemiboldItalic.ttf" }

    //=============================================================
    // Connection
    //=============================================================
    Connections {
        target:tDashboardControler
        // Signal qui permet de débloquer l'affichage des onglets après refresh dans QML
        function onDisplayGenericSensor() {
            console.log("==DISPLAY GENERIC SENSOR==");
            // On affiche les onglets
            // Mode production nke
            if( bNkeProdMode ){
                li_Liveview.visible    = false
                li_Calibration.visible = true
                li_Calibration_O2.visible = false
                li_Wiper.visible = false
                // Placement des onglets
                //li_Liveview.anchors.top       = li_Dashboard.bottom
                li_Calibration.anchors.top    = li_Dashboard.bottom
                li_Factory.anchors.top        = li_Calibration.bottom
            }
            // Mode modbus 9600
            else if( tMainData.bCOMSlowMode ){
                li_Liveview.visible    = true
                li_Calibration.visible = false
                li_Calibration_O2.visible = false
                li_Wiper.visible = false
                // Placement des onglets
                li_Liveview.anchors.top       = li_Dashboard.bottom
                //li_Calibration.anchors.top    = li_Dashboard.bottom
                li_Factory.anchors.top        = li_Liveview.bottom
            }
            // Mode normal
            else{
                li_Liveview.visible    = true
                li_Calibration.visible = true
                li_Calibration_O2.visible = false
                li_Wiper.visible = false
                // Placement des onglets
                li_Liveview.anchors.top       = li_Dashboard.bottom
                li_Calibration.anchors.top    = li_Liveview.bottom
                li_Factory.anchors.top        = li_Calibration.bottom
            }

            bGenericSensor = true
            bWiper         = false
            bO2            = false

            // Refresh bloc size
            //factoryPart.vFFactoryPartRefreshBlocSize()
        }
        // Signal qui permet de débloquer l'affichage des onglets après refresh dans QML
        function onDisplayWiper() {
            console.log("==DISPLAY WIPER==");
            // On affiche les onglets
            li_Liveview.visible    = false
            li_Calibration.visible = false
            li_Calibration_O2.visible = false
            li_Wiper.visible = true

            bWiper         = true
            bGenericSensor = false
            bO2            = false
            //li_Factory.bFactoryChange = !li_Factory.bFactoryChange

            // Placement des onglets
            li_Wiper.anchors.top    = li_Dashboard.bottom
            li_Factory.anchors.top  = li_Wiper.bottom
            // Refresh bloc size
            //factoryPart.vFFactoryPartRefreshBlocSize()
        }
        // Signal qui permet de débloquer l'affichage des onglets après refresh dans QML
        function onDisplayO2() {
            console.log("==DISPLAY O2==");
            // On affiche les onglets
            li_Liveview.visible    = true
            li_Calibration.visible = false

            //%%AP - 2021.11.19 - Ajout d'un test pour uniquement les O2 3 voies (TDO ou mode nke)
            //%%AP - calibrationID n'existe pas!!!!!!
            if(  ( calibrationID.ch2ID      == 25 )
              && ( liveviewID.channelNumber == 3  ) ){
                //%%AP 2021.10.26 - Remis temporairement
                li_Calibration.visible = true
                //%%AP - 2021.10.25 - Remise temporairement
                li_Calibration.anchors.top    = li_Calibration_O2.bottom
                li_Factory.anchors.top        = li_Calibration.bottom
            }
            else{
                //%%AP - 2021.10.25 - Retiré temporairement
                li_Factory.anchors.top        = li_Calibration_O2.bottom
            }
            li_Calibration_O2.visible = true
            li_Wiper.visible = false

            bO2            = true
            bWiper         = false
            //bGenericSensor = false
            bGenericSensor = true

            // Placement des onglets
            li_Liveview.anchors.top       = li_Dashboard.bottom
            li_Calibration_O2.anchors.top = li_Liveview.bottom

            //txt_calibration.text = "txt_calibration"

            // Refresh bloc size
            //factoryPart.vFFactoryPartRefreshBlocSize()
        }
        // Signal emit à la fin du refresh du dashboard
        function onEndOfRefreshSignal() {
            console.log("QML : onEndOfRefreshSignal BEGIN")
            // On débloque le click
            vFAllowUserClick()
            // Propagation du signal
            liveViewPart.vFEnfOfRefreshSignal();
            //
            console.log("QML : onEndOfRefreshSignal END")
        }
    }

    //----------------------------------------------------
    // Ouverture
    //----------------------------------------------------
    MessageDialog {
        id: softwareDevVersionWarning
        title: qsTr("Warning")
        text: "<center>"+qsTr("Warning: the current software is ")+
        "<br/>"+qsTr("a non-commercial/developpement version")+"<br/>"+qsTr("(cannot be sold)")+".</center>"
    }
    Timer {
        id: softwareDevVersionWarningTimer
        interval: 500; running: true; repeat: false
        onTriggered: {
            // Si en debug
            if( bNonCommercial ){
                // Ouverture de la fenêtre
                softwareDevVersionWarning.open()
            }
        }
    }
    //----------------------------------------------------
    // Fermeture
    //----------------------------------------------------
    // @disable-check M16
    onClosing:{
        if( bStarMeasureSwitchState || bStartMeasureO2 ){
            liveViewPart.vFLiveViewStopMeasureFile()
        }
        console.log("Window is being closed")
        tCloseEvent.exitHandler();
    }


    Connections {
        target:tMainEvent

        // Lecture du numéro de version logiciel
        function onSetSoftwareVersionSignal( sVersion ) {
            console.log("onSetSoftwareVersionSignal: "+sVersion)
            sVersionNumber = sVersion
            //txtSoftWareVersion.text = "Version "+sVersionNumber
        }
        // Ouverture de la fenêtre principale
        function onMainWindowOpen() {
            //window.visible = true
        }
        // Ouverture de la fenêtre d'erreur
        function onMainWindowErrorOpen( sErrorMsg ) {
            // On débloque le click
            vFAllowUserClick()
            // Affichage de la fenêtre d'erreur
            windowError.text = sErrorMsg
            windowError.open()
        }
        // Ouverture de la fenêtre d'erreur
        function onMainWindowInfoOpen( sMsg ) {
            // Affichage de la fenêtre d'erreur
            windowInfo.text = sMsg
            windowInfo.open()
        }
        // Ouverture de la fenêtre de mise à jour
        function onMainWindowUpdateDetectedOpen( sNewVersionNumber ){
            console.log( "onMainWindowUpdateDetectedOpen > New version : "+sNewVersionNumber );
            // Ouverture de la fenêtre
            windowUpdateDetected.open()
        }
        function onVFMain_ClearDataSignal(){
            vFMAIN_ClearData();
        }
    }

    onUiPageSelectedChanged: {
        li_Dashboard.color        = (uiPageSelected==eTAB_DASHBOARD)?liNavHoverColor:liNavColor
        li_Dashboard.border.color = (uiPageSelected==eTAB_DASHBOARD)?liNavBorderHoverColor:liNavBorderColor
        li_Liveview.color        = (uiPageSelected==eTAB_GENERIC_LIVEVIEW)?liNavHoverColor:liNavColor
        li_Liveview.border.color = (uiPageSelected==eTAB_GENERIC_LIVEVIEW)?liNavBorderHoverColor:liNavBorderColor
        li_Calibration.color        = (uiPageSelected==eTAB_GENERIC_CALIBRATION)?liNavHoverColor:liNavColor
        li_Calibration.border.color = (uiPageSelected==eTAB_GENERIC_CALIBRATION)?liNavBorderHoverColor:liNavBorderColor
        li_Wiper.color        = (uiPageSelected==eTAB_WIPER)?liNavHoverColor:liNavColor
        li_Wiper.border.color = (uiPageSelected==eTAB_WIPER)?liNavBorderHoverColor:liNavBorderColor
        li_Factory.color        = (uiPageSelected==eTAB_FACTORY)?liNavHoverColor:liNavColor
        li_Factory.border.color = (uiPageSelected==eTAB_FACTORY)?liNavBorderHoverColor:liNavBorderColor
        li_Calibration_O2.color        = (uiPageSelected==eTAB_O2)?liNavHoverColor:liNavColor
        li_Calibration_O2.border.color = (uiPageSelected==eTAB_O2)?liNavBorderHoverColor:liNavBorderColor
    }

    //==============================================================
    // Déclaration des fonctions
    //==============================================================
    // Démarrage calibration O2
    function vFStartO2CalibrationLiveview_MAIN(){
        liveViewPart.vFStartO2CalibrationLiveview();
    }
    // Fermeture calibration O2
    function vFStopO2CalibrationLiveview_MAIN(){
        liveViewPart.vFStopO2CalibrationLiveview();
    }
    function vFMAIN_CloseWindows(){
        window.close();
    }
    /*
    // Fermeture calibration O2
    function vFStopO2CalibrationLiveview_MAIN(){
        liveViewPart.vFStopO2CalibrationLiveview();
    }
    */
    // Reset/Clear des données du logiciel
    function vFMAIN_ClearData(){
        // Reset des variables internes
        bO2            = false
        bWiper         = false
        bGenericSensor = false
        // Placement des onglets en état par défaut
        li_Liveview.anchors.top       = li_Dashboard.bottom
        li_Factory.anchors.top        = li_Dashboard.bottom
        li_Calibration.anchors.top    = li_Dashboard.bottom
        li_Calibration_O2.anchors.top = li_Dashboard.bottom
        li_Wiper.anchors.top          = li_Dashboard.bottom

        // On affiche les onglets
        li_Liveview.visible       = false
        li_Calibration.visible    = false
        li_Calibration_O2.visible = false
        li_Wiper.visible          = false
        // Retour à la fenêtre normale
        //windowOpacity.visible = false
        //windowCOMManualMain.visible = false
        //windowCOMManualMain.modality = Qt.NonModal
    }

    //**************************************************************
    // Déclaration du composant
    //**************************************************************
    id: window
    visible: true
    width: 1020
    height: 780
    //color: "#efeef4"
    color: windowsBackgroundColor
    //title: "WiMo Calibration Tool"+ ( (tMainData.sInterfaceSN!=""||sPortCOMMain!="")?" ("+tMainData.sInterfaceSN+" - "+sPortCOMMain+")":"" )
    title: "WiMo Calibration Tool"+ ( (sPortCOMMain!="")?((tMainData.sInterfaceSN!="")? (" ("+tMainData.sInterfaceSN+" - "+sPortCOMMain+")" ):" ("+sPortCOMMain+")" ):"") + ( (tMainData.bNkeMode)?" - nke":((tMainData.bMetroMode)?" - Metro":((tMainData.bProdMode?" - Production":""))  ) )

    maximumWidth: 1200
    minimumWidth: 800
    maximumHeight: 900
    minimumHeight: 550

    //--------------------------------------
    // Opacification de la fenêtre
    //--------------------------------------
    Rectangle {
        id: windowOpacity
        visible: false
        //color: "#55888888"
        color: "#55000000"
        width: window.width
        height: window.height
        anchors.fill: parent
        z: 1

        onVisibleChanged: {
            console.log("=======onVisibleChanged=======");
            console.log("======="+ windowOpacity.visible +"=======");
            console.log("==============================");
        }
    }

    //--------------------------------------
    // Blocage du click quand opération RS232 en cours
    //--------------------------------------
    function vFLockUserClick(){
        console.log("vFLockUserClick");
        mouseAreaWindowBlockClick.visible = true }
    function vFAllowUserClick(){
        console.log("vFAllowUserClick");
        mouseAreaWindowBlockClick.visible = false }
    MouseArea {
        id: mouseAreaWindowBlockClick
        visible: false
        anchors.fill: parent

        propagateComposedEvents: false;
        width: window.width
        height: window.height
        z: 1
        hoverEnabled: false

        Rectangle {
            visible: false;
            anchors.fill: parent
            width: parent.width
            height: parent.height
            color: "#55000000"
        }
    }



    //--------------------------------------
    // Nav menu
    //--------------------------------------
    DropShadow {
        anchors.fill: rectangle11
        horizontalOffset: 0
        verticalOffset: 0
        radius: 12.0
        samples: 30
        color: "#20000000"
        source: rectangle11
        clip: true
    }
    Rectangle {
        id: rectangle11
        x: 0
        y: 0
        width: 228
        height: window.height
        color: "#ffffff"

        Rectangle {
            id: rectangle6
            x: 0
            y: 0
            width: 223
            height: window.height
            color: "#132644"


            Rectangle {
                id: rectangle7
                x: 0
                y: 0
                width: rectangle6.width
                height: 93
                color: "#ffffff"

                Image {
                    id: image
                    x: 29
                    y: 21
                    width: 220
                    height: 52
                    antialiasing: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/Logo/Global/wimo_2.png"
                }
            }
            Column {
                id: columnNav
                x: 0
                width: 222
                //height: 200
                //height: 186
                anchors.top: rectangle7.bottom
                anchors.topMargin: 38
                //spacing: -140
                //spacing: 5
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                //-----------------------
                // NAV : dashboard
                //-----------------------
                Rectangle {
                    id: li_Dashboard
                    width: parent.width
                    height: 50
                    color: (uiPageSelected==eTAB_DASHBOARD)?liNavHoverColor:liNavColor
                    border.color: (uiPageSelected==eTAB_DASHBOARD)?liNavBorderHoverColor:liNavBorderColor
                    clip: false

                    Image {
                        id: img_dashboard
                        anchors.left: parent.left
                        anchors.leftMargin: 18
                        anchors.verticalCenter: parent.verticalCenter
                        clip: false
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/Logo/Nav/nav-Dashboard-low.png"
                    }

                    Text {
                        id: txt_dashboard
                        anchors.left: parent.left
                        anchors.leftMargin: 59
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#ffffff"
                        text: qsTr("DASHBOARD")
                        //anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 13
                        font.family: "Open Sans"
                    }

                    MouseArea {
                        id: mouseAreaLiDashboard
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            // Si mesure
                            //if( bStarMeasureSwitchState ){
                            //    windowMesureOccuring.open()
                            //}
                            //else{
                            //    uiPageSelected = eTAB_DASHBOARD
                            //}
                            uiPageSelected = eTAB_DASHBOARD
                        }
                        // #1D459B
                        onEntered: {
                            parent.color        = liNavHoverColor
                            parent.border.color = liNavBorderHoverColor
                        }
                        onExited: {
                            if( uiPageSelected != eTAB_DASHBOARD ){
                                parent.color        = liNavColor
                                parent.border.color = liNavBorderColor
                            }
                        }
                    }
                }

                //-----------------------
                // NAV : Liveview
                //-----------------------
                Rectangle {
                    id: li_Liveview
                    //x: 0
                    //y: 0
                    width: parent.width
                    height: 50
                    color: (uiPageSelected==eTAB_GENERIC_LIVEVIEW)?liNavHoverColor:liNavColor
                    border.color: (uiPageSelected==eTAB_GENERIC_LIVEVIEW)?liNavBorderHoverColor:liNavBorderColor

                    anchors.top: li_Dashboard.bottom
                    anchors.topMargin: 20
                    //visible: ( bGenericSensor || bO2 )
                    visible: false

                    Image {
                        id: img_liveView
                        anchors.left: parent.left
                        anchors.leftMargin: 18
                        anchors.verticalCenter: parent.verticalCenter
                        width: 27
                        height: 25
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/Logo/Nav/nav-Realtime-low.png"
                    }

                    Text {
                        id: txt_liveview
                        anchors.left: parent.left
                        anchors.leftMargin: 59
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#ffffff"
                        text: qsTr("LIVE VIEW")
                        font.family: "Open Sans"
                        font.pixelSize: 13
                    }
                    MouseArea {
                        id: mouseArea_liveview
                        anchors.fill: parent
                        onClicked: {
                            uiPageSelected = eTAB_GENERIC_LIVEVIEW
                        }
                        hoverEnabled: true
                        onEntered: {
                            parent.color        = liNavHoverColor
                            parent.border.color = liNavBorderHoverColor
                        }
                        onExited:  {
                            if( uiPageSelected != eTAB_GENERIC_LIVEVIEW ){
                                parent.color        = liNavColor
                                parent.border.color = liNavBorderColor
                            }
                        }
                    }
                }
                //-----------------------
                // NAV : calibration
                //-----------------------
                Rectangle {
                    id: li_Calibration
                    //y: 0
                    width: parent.width
                    height: 50
                    //Layout.height: 49
                    //color: "#11337b"
                    color: (uiPageSelected==eTAB_GENERIC_CALIBRATION)?liNavHoverColor:liNavColor
                    border.color: (uiPageSelected==eTAB_GENERIC_CALIBRATION)?liNavBorderHoverColor:liNavBorderColor
                    //anchors.top: li_Liveview.bottom
                    anchors.top: li_Dashboard.bottom

                    anchors.topMargin: 20
                    //Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    //Layout.preferredHeight: 50
                    //Layout.preferredWidth: parent.width
                    //Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    //visible: bGenericSensor
                    visible: false

                    Image {
                        id: img_calibration
                        width: 27
                        height: 25
                        anchors.left: parent.left
                        anchors.leftMargin: 18
                        anchors.verticalCenter: parent.verticalCenter
                        smooth: true
                        antialiasing: false
                        clip: false
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/Logo/Nav/nav-calibration-low.png"
                    }
                    Text {
                        id: txt_calibration
                        anchors.left: parent.left
                        anchors.leftMargin: 59
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#ffffff"
                        text: (bO2)?qsTr("T° CALIBRATION"):qsTr("CALIBRATION")
                        font.pixelSize: 13
                        font.family: "Open Sans"
                    }
                    MouseArea {
                        id: mouseAreaLiCalibration
                        anchors.fill: parent
                        onClicked: {
                            // Si mesure
                            //if( bStarMeasureSwitchState ){
                            //    windowMesureOccuring.open()
                            //}
                            //else{
                                uiPageSelected = eTAB_GENERIC_CALIBRATION
                                calibrationPart1.vFDisplayCalibrationNav()
                            //}
                        }
                        hoverEnabled: true
                        onEntered: {
                            parent.border.color = liNavBorderHoverColor
                            parent.color        = liNavHoverColor
                        }
                        onExited:  {
                            if( uiPageSelected != eTAB_GENERIC_CALIBRATION ){
                                parent.border.color = liNavBorderColor
                                parent.color        = liNavColor
                            }
                        }
                    }
                }

                //-----------------------
                // NAV : Factory
                //-----------------------
                Rectangle {
                    id: li_Factory
                    width: parent.width
                    height: 49
                    //color: "#11337b"
                    color: (uiPageSelected==eTAB_FACTORY)?liNavHoverColor:liNavColor
                    border.color: (uiPageSelected==eTAB_FACTORY)?liNavBorderHoverColor:liNavBorderColor
                    //visible: bWiper || bGenericSensor
                    //visible: true
                    visible: false

                    //property bool bFactoryChange: false
                    //anchors.top: ( bWiper || bGenericSensor ) ? li_Calibration.bottom: li_Dashboard.bottom
                    anchors.top: li_Dashboard.bottom
                    anchors.topMargin: 20

                    Text {
                        id: element4
                        x: 59
                        y: 16
                        color: "#ffffff"
                        text: qsTr("FACTORY")
                        font.pixelSize: 13
                        anchors.verticalCenter: parent.verticalCenter
                        font.family: "Open Sans"
                    }

                    Image {
                        id: image4
                        x: 19
                        y: 13
                        width: 26
                        height: 25
                        anchors.verticalCenterOffset: 0
                        clip: false
                        source: "qrc:/Logo/Nav/nav-updateW2-low.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        smooth: true
                        antialiasing: false
                    }

                    MouseArea {
                        id: mouseAreaLiFactory
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            // Si mesure
                            //if( bStarMeasureSwitchState ){
                            //    windowMesureOccuring.open()
                            //}
                            //else{
                                uiPageSelected = eTAB_FACTORY
                            //}
                        }
                        onEntered: {
                            parent.border.color = liNavBorderHoverColor
                            parent.color        = liNavHoverColor
                        }
                        onExited:  {
                            if( uiPageSelected != eTAB_FACTORY ){
                                parent.border.color = liNavBorderColor
                                parent.color        = liNavColor
                            }
                        }
                    }
                }

                //-----------------------
                // NAV : wiper
                //-----------------------
                Rectangle {
                    id: li_Wiper
                    //y: 0
                    width: parent.width
                    height: 50
                    //Layout.height: 49
                    //color: "#11337b"
                    color: (uiPageSelected==eTAB_WIPER)?liNavHoverColor:liNavColor
                    border.color: (uiPageSelected==eTAB_WIPER)?liNavBorderHoverColor:liNavBorderColor
                    anchors.top: li_Dashboard.bottom
                    anchors.topMargin: 20
                    //Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    //Layout.preferredHeight: 50
                    //Layout.preferredWidth: parent.width
                    //Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    //visible: bWiper
                    visible: false

                    Image {
                        id: img_wiper
                        width: 27
                        height: 25
                        anchors.left: parent.left
                        anchors.leftMargin: 18
                        anchors.verticalCenter: parent.verticalCenter
                        smooth: true
                        antialiasing: false
                        clip: false
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/Logo/Nav/nav-calibration-low.png"
                    }
                    Text {
                        id: txt_wiper
                        anchors.left: parent.left
                        anchors.leftMargin: 59
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#ffffff"
                        text: qsTr("WIPER")
                        font.pixelSize: 13
                        font.family: "Open Sans"
                    }
                    MouseArea {
                        id: mouseAreaLiWiper
                        anchors.fill: parent
                        onClicked: {
                            uiPageSelected = eTAB_WIPER
                        }
                        hoverEnabled: true
                        onEntered: {
                            parent.border.color = liNavBorderHoverColor
                            parent.color        = liNavHoverColor
                        }
                        onExited:  {
                            if( uiPageSelected != eTAB_WIPER ){
                                parent.border.color = liNavBorderColor
                                parent.color        = liNavColor
                            }
                        }
                    }
                }
                //-----------------------
                // NAV : Calibration O2
                //-----------------------
                Rectangle {
                    id: li_Calibration_O2
                    //y: 0
                    width: parent.width
                    height: 50
                    //Layout.height: 49
                    //color: "#11337b"
                    color: (uiPageSelected==eTAB_O2)?liNavHoverColor:liNavColor
                    border.color: (uiPageSelected==eTAB_O2)?liNavBorderHoverColor:liNavBorderColor
                    //anchors.top: li_Liveview.bottom
                    anchors.top: li_Dashboard.bottom
                    anchors.topMargin: 20
                    //Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    //Layout.preferredHeight: 50
                    //Layout.preferredWidth: parent.width
                    //Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    //visible: bO2
                    visible: false

                    Image {
                        id: img_calibration_O2
                        width: 27
                        height: 25
                        anchors.left: parent.left
                        anchors.leftMargin: 18
                        anchors.verticalCenter: parent.verticalCenter
                        smooth: true
                        antialiasing: false
                        clip: false
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/Logo/Nav/nav-calibration-low.png"
                    }
                    Text {
                        id: txt_calibration_O2
                        anchors.left: parent.left
                        anchors.leftMargin: 59
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#ffffff"
                        //text: qsTr("O2 CALIBRATION")
                        text: qsTr("O2 CALIBRATION")
                        font.pixelSize: 13
                        font.family: "Open Sans"
                    }
                    MouseArea {
                        id: mouseAreaLiCalibrationO2
                        anchors.fill: parent
                        onClicked: {
                            uiPageSelected = eTAB_O2
                        }
                        hoverEnabled: true
                        onEntered: {
                            parent.border.color = liNavBorderHoverColor
                            parent.color        = liNavHoverColor
                        }
                        onExited:  {
                            if( uiPageSelected != eTAB_O2 ){
                                parent.border.color = liNavBorderColor
                                parent.color        = liNavColor
                            }
                        }
                    }
                }
            }

            //------------------------------------------
            // Logo nke avec numéro de version
            //------------------------------------------
            Rectangle {
                id: rectLogoNkeQtContainer
                //y: 502
                width: parent.width * 90 / 100
                height: 70
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                //anchors.horizontalCenter: parent.horizontalCenter
                anchors.left: parent.left
                anchors.leftMargin:10
                color: "#000000FF";
                MouseArea {
                    anchors.fill: rectLogoNke
                    //propagateComposedEvents: false;
                    z: 1
                    //hoverEnabled: false
                    onClicked: { tMainEvent.displayScreen() }
                }
                Rectangle {
                    id: rectLogoNke
                    //width: parent.width * 70 / 100
                    width: parent.width
                    height: parent.height
                    color: "#00000000"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12
                    // Logo nke
                    Image {
                        id: imgLogoNke
                        width: parent.width * 90 / 100
                        //height: 71
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/Logo/Global/nke-instrumentation-low3.png"
                        //anchors.bottom: txtSoftWareVersion.top
                        //anchors.bottomMargin: 5
                    }
                    // Numéro de version
                    Text {
                        id: txtSoftWareVersion
                        width: parent.width * 80 / 100
                        //height: 14
                        color: "#f0eded"
                        //text: qsTr("Version 1.0.0")
                        text: "Version "+sVersionNumber
                        //anchors.bottom: parent.bottom
                        anchors.top: imgLogoNke.bottom
                        anchors.topMargin: 3
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.weight: Font.DemiBold
                        font.pixelSize: 11
                    }
                }
            }
        }
    }


    //----------------------------------
    // Dashboard container
    //----------------------------------
    ScrollView {
        id: dashboardScrollView
        x: 229
        y: 0
        width: window.width - 228
        height: window.height
        visible: (uiPageSelected==eTAB_DASHBOARD)

        //contentHeight: (window.height<=600)?600:window.height
        contentHeight: 780
        //contentHeight: 600
        contentWidth: 662
        clip: true
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        DashBoardPart {
            id: dashBoardPart
        }
    }


    //----------------------------------
    // Liveview container
    //----------------------------------
    SwipeView{
        id: liveViewStackView

        currentIndex: 0
        //initialItem: liveViewPart
        x: 229
        y: 0
        width: window.width - 228
        height: window.height
        clip: true
        visible: (uiPageSelected==eTAB_GENERIC_LIVEVIEW)

        Item{
            id: firstItem
            LiveViewPart {
                id: liveViewPart
                anchors.fill: parent
            }
        }

        ScrollView {
            id: liveViewConfig
            contentHeight: 770
            contentWidth: 662
            //id: liveViewConfig
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            LiveViewConfig {
                id: liveViewConfigID_MAIN
                onStopMeasureSignal: {
                    liveViewPart.vFLiveviewPartStopMeasure_ClickSwitch()
                }
            }
        }
    }

    //----------------------------------
    // Calibration container
    //----------------------------------
    ScrollView {
        id: calibScrollView1
        x: 229
        y: 0
        width: window.width - 228
        height: window.height
        contentHeight: 950
        contentWidth: 662
        visible: (uiPageSelected==eTAB_GENERIC_CALIBRATION && bGenericSensor)

        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        CalibrationPart {
            id: calibrationPart1
            width: window.width - 228
            height: parent.height
        }
    }

    //----------------------------------
    // O2 container
    //----------------------------------
    // Taille du bloc O2 calibration
    ScrollView {
        id: calibScrollViewO2
        x: 229
        y: 0
        width: window.width - 228
        height: window.height
        contentHeight: 820
        contentWidth: 662

        visible: (uiPageSelected==eTAB_O2 && bO2)
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        O2Part {
            id: calibrationPartO2
            width: window.width - 228
            //height: 635
            height: parent.height
        }
    }


    //----------------------------------
    // Wiper container
    //----------------------------------
    ScrollView {
        id: wiperScrollView1
        x: 229
        y: 0
        width: window.width - 228
        height: window.height
        contentHeight: 850
        contentWidth: 662
        visible: (uiPageSelected==eTAB_WIPER)
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        WiperPart {
            id: wiperPart
            width: window.width - 228
            height: 850
        }

    }

    //----------------------------------
    // Factory container
    //----------------------------------
    Image {
        id: image1
        x: 754
        y: 0
        width: 250
        height: 600
        fillMode: Image.PreserveAspectFit
        source: "qrc:/Logo/Dashboard/Interface-PC-Modbus.png"
        opacity: 0.08
        visible: false
    }
    // Ajout
    ScrollView {
        id: factoryScrollView
        x: 229
        y: 0
        width: window.width - 228
        height: window.height
        contentHeight: uiFactoryHeight
        contentWidth: 662
        visible: (uiPageSelected==eTAB_FACTORY)
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        FactoryPart {
            id: factoryPart
            width: window.width - 228
            //height: 750
            height: uiFactoryHeight
        }

    }

    //-----------------
    // Onglet grisé quand en mesure
    //-----------------
    Rectangle{
        visible: bStarMeasureSwitchState && (uiPageSelected!=eTAB_GENERIC_LIVEVIEW) && (uiPageSelected!=eTAB_O2)
        x: 229
        y: 0
        z: 100000
        width: window.width - 228
        height: window.height
        color: "#000000"
        opacity: 0.2
        MouseArea {
            id: mouseAreaLiveviewOnmeasure
            anchors.fill: parent
            enabled: bStarMeasureSwitchState && (uiPageSelected!=eTAB_GENERIC_LIVEVIEW) && (uiPageSelected!=eTAB_O2)
            onClicked: {
                // Si mesure
                if( bStarMeasureSwitchState ){
                    windowMesureOccuring.open()
                }
            }
        }
    }

    //----------------------------------------------------
    // Simulation pour affichage graphique
    //----------------------------------------------------
    property bool bStarMeasureSwitchState: false
    property bool bStartMeasureO2: false
    property int  uiMainMeasurePeriod: 1

    // Modification de la cadence
    function vFMAINSetMeasurePeriod( uiPeriod ){
        uiMainMeasurePeriod = uiPeriod
    }
    // Démarrage de la mesure
    function vFMAINStartMeasureO2Calib(){
        bStartMeasureO2 = true
        // Ouverture du fichier de mesure
        liveViewPart.vFLiveViewStartMeasureFile()
    }
    // Stop de la mesure
    function vFMAINStopMeasureO2Calib(){
        bStartMeasureO2 = false
        // Fermeture du fichier de mesure
        liveViewPart.vFLiveViewStopMeasureFile()
    }
    // Démarrage de la mesure
    function vFMAINStartMeasure(){
        console.log( "uiMainMeasurePeriod = "+ uiMainMeasurePeriod * 1000 );
        liveviewAcqTimer.interval = uiMainMeasurePeriod * 1000
        liveviewAcqTimer.start();
        //%%AP - 2020.12.10 - Ajout
        liveViewConfigID_MAIN.vFStartMesure();
        // Start du timer
        //vFMAINStartMeasure();
        // Indique au CPP le démarrage mesure
        tMainEvent.beginAcquisition()
    }
    // Stop de la mesure
    function vFMAINStopMeasure(){
        liveViewConfigID_MAIN.vFStopMesure();
        liveviewAcqTimer.stop();
        //%%AP - 2020.12.10 - Ajout
        // Stop timer
        //vFMAINStopMeasure();
    }

    //----------------------------------------------------
    // Timer acquisition
    //----------------------------------------------------
    Timer {
        id: liveviewAcqTimer
        //interval: function(){ return( uiMeasurePeriod * 1000 ) }; running: true; repeat: true
        interval: uiMainMeasurePeriod * 1000; running: false; repeat: true
        onTriggered: {
            //%%AP - 22021.02.17 - DEBUG
            //return;

            console.log( "** liveviewAcqTimer Triggered **")
            // Si switch mesure est activé
            if( bStarMeasureSwitchState || bStartMeasureO2 ){
                console.log( Qt.formatDateTime(new Date(), "yyMMdd HHmmss") )
                // Démarrage d'une nouvelle mesure
                liveViewPart.vFCallStartMeasure();
            }
        }
    }

    property variant win;  // you can hold this as a reference..
    property bool bQwtDefined: false;

    //-----------------------------------
    // Validation du port COM
    //-----------------------------------
    function vFMAINWindowCOM_ValidateCOMPort( uiComPortIndex ){
        console.log( "==vFMAINWindowCOM_ValidateCOMPort==" );
        console.log( "COM"+uiComPortIndex );
        tCOMEvent.vFPortComSubmit( "COM"+uiComPortIndex, uiComPortIndex )
        // Retour à la fenêtre normale
        windowOpacity.visible = false
        windowCOMMain.visible = false
    }
    function vFMAINWindowCOM_ValidateManualCOMPort( uiComPortIndex ){
        tCOMEvent.vFPortComSubmit( "COM"+uiComPortIndex, uiComPortIndex )
        // Retour à la fenêtre normale
        windowOpacity.visible = false
        windowCOMManualMain.visible = false
        //windowCOMManualMain.modality = Qt.NonModal
        // Fatal?
        //windowCOMMain.close()
    }
    //-----------------------------------
    // Ouverture de la fenêtre manuel
    //-----------------------------------
    function vFMAINWindowCOM_OpenManualWindow(){
        console.log("==vFMAINWindowCOM_OpenManualWindow==");
        // Retour à la fenêtre normale
        windowOpacity.visible = true

        //windowCOMManualMain.close()
        //windowCOMManualMain.modality = Qt.NonModal
        windowCOMManualMain.visible = true;
    }
    // Connexion
    Connections {
        target:tCOMEvent
        //--------------------------------
        // Ouverture de la fenêtre COM port
        //--------------------------------
        function onVFOpenCOMPortListSignal() {
            console.log("==onVFOpenCOMPortListSignal==");
            //windowCOMMain.modality = Qt.ApplicationModal
            // Affichage de la fenêtre
            windowCOMMain.visible = true
            windowOpacity.visible = true
            // Transmission du signal
            windowCOMMain.vFOpenCOMPortListSignal()
        }
        //--------------------------------
        // Clear de la liste COM port
        //--------------------------------
        function onVFClearCOMPortListSignal() {
            // Effacement de la liste des ports COM
            //inputCOMPortModel.clear()
            windowCOMMain.vFClearCOMPortListSignal()
        }
        //--------------------------------
        // Un port COM a été ouvert avec succès
        //--------------------------------
        function onVFComPortSuccefullyOpenSignal() {
            // On affiche l'onglet factory
            li_Factory.visible = true;
            //
            windowOpacity.visible = false
            windowCOMManualMain.visible = false
            //windowCOMManualMain.modality = Qt.NonModal
        }
        //--------------------------------
        // Un port COM a été ouvert avec succès
        //--------------------------------
        function onVFComPortSuccefullyCloseSignal() {
            // On cache l'onglet factory
            li_Factory.visible = false;
        }
        //--------------------------------
        // Ajout de la liste COM port
        //--------------------------------
        function onVFAddCOMPortSignal( sCOMPort, iPortCom, iSN_Y, uliSN_N, sSN ) {
            console.log("==onVFAddCOMPortSignal==");
            console.log( "iPortCom = "+ iPortCom );
            console.log( "iSN_Y = "+ iSN_Y );
            console.log( "uliSN_N = "+ uliSN_N );
            //inputCOMPortModel.append({text: "COM"+iPortCom+" - SN"+iSN_Y+"_"+uliSN_N })
            windowCOMMain.vFAddCOMPortSignal( sCOMPort, iPortCom, iSN_Y, uliSN_N, sSN );
        }
        //--------------------------------
        // Ouverture de la fenêtre COM port
        //--------------------------------
        function onVFOpenCOMPortManualWindowSignal() {
        //onVFOpenCOMPortManualSignal: {
            console.log("--onVFOpenCOMPortManualSignal--")
            //windowCOMManualMain.modality = Qt.ApplicationModal;
            // Affichage de la fenêtre
            windowCOMManualMain.visible = true;
            windowOpacity.visible = true;
            // Transmission du signal
            windowCOMManualMain.vFOpenCOMPortListSignal();
        }
    }
    //----------------------------------------------------
    // Fenêtre COM Port
    //----------------------------------------------------
    WindowCOM {
        id:windowCOMMain
        visible: false
        flags: Qt.Dialog
        // Fenêtre de sélecion automatique
        bAuto: true
        //%%AP - 2020.10.09 - Remmettre
        //modality: Qt.ApplicationModal
    }
    //----------------------------------------------------
    // Fenêtre COM Port Manuel
    //----------------------------------------------------
    WindowCOM {
        id:windowCOMManualMain

        visible: false
        flags: Qt.Dialog
        // Fenêtre de sélecion manuel
        bAuto: false
        modality: Qt.ApplicationModal
    }

    //----------------------------------------------------
    // Fenêtre Debug
    //----------------------------------------------------
    WindowDebug {
        id:windowDebugMain

        visible: bDebugQML
        y: window.y;
        x: window.x + window.width;

        // Connexion
        Connections {
            target:tDebugWindow
            //--------------------------------
            // Clear de la liste COM port
            //--------------------------------
            function onVFAddDebugLineSignal( sDebug ) {
                windowDebugMain.vFWindowDebug_AddALine( sDebug );
            }
            //--------------------------------
            // Ajout de la liste COM port
            //--------------------------------
            //onVFAddCOMPortSignal: {}
        }
    }


    //---------------------------------------------------------
    // Fenêtre d'erreur
    //---------------------------------------------------------
    MessageDialog {
        id: windowError
        title: qsTr("Error")
        //icon: StandardIcon.Question
        text: qsTr("Communication error")
        //detailedText: "To replace a file means that its existing contents will be lost. " +
        //   "The file that you are copying now will be copied over it instead."
        standardButtons: StandardButton.Ok
        //Component.onCompleted: visible = true
        onYes: console.log("copied")
        onNo: console.log("didn't copy")
        onRejected: console.log("aborted")
    }
    //---------------------------------------------------------
    // Fenêtre d'info
    //---------------------------------------------------------
    MessageDialog {
        id: windowInfo
        title: qsTr("Info")
        text: qsTr("Information")
        standardButtons: StandardButton.Ok
    }

    //---------------------------------------------------------
    // Fenêtre de bocage durant la mesure
    //---------------------------------------------------------
    MessageDialog {
        id: windowMesureOccuring
        title: qsTr("Warning")
        //icon: StandardIcon.Question
        text: qsTr("Please stop measure to change page")
        //detailedText: "To replace a file means that its existing contents will be lost. " +
        //   "The file that you are copying now will be copied over it instead."
        standardButtons: StandardButton.Ok
        //Component.onCompleted: visible = true
        //onYes: console.log("copied")
        //onNo: console.log("didn't copy")
        //onRejected: console.log("aborted")
    }
    //---------------------------------------------------------
    // Fenêtre mise à jour détecté
    //---------------------------------------------------------
    MessageDialog {
        id: windowUpdateDetected
        title: qsTr("Update")
        //icon: StandardIcon.Question
        text: qsTr("New update detected")+
         "\r\n"+qsTr("Do you want to update now ?")
        //detailedText: "Current version : "+sVersionNumber+
        //   "Do you want to update now ?"
        standardButtons: StandardButton.Yes | StandardButton.No
        //Component.onCompleted: visible = true
        //Component.onCompleted: visible = true
        onYes: {
            console.log("Start update")
            // Appel de la fonction de mise à jour
            tMainEvent.vFMainWindowStartUpdate()
        }
        onNo: {
            console.log("Don't start update")
        }
        onRejected: {
            console.log("Rejected")
        }
    }


    //---------------------------------------------------------
    // Fenêtre d'affichage de la configuration
    //---------------------------------------------------------
    WindowsParamDisplay {
        id: windowConfigDisplay
        visible: false
    }
    function vFMAINWindowConfigDisplayOpen(){
        //tMainEvent.vFMainEventDisplayParamWindow()
        windowConfigDisplay.vFDisplayWindow();
        windowConfigDisplay.visible = true;
    }

    //---------------------------------------------------------
    // Fenêtre mise à jour capteur et interface
    //---------------------------------------------------------
    WindowsSensorUpdate {
        id: windowsSensorUpdate
    }
    WindowsInterfaceUpdate {
        id: windowsInterfaceUpdate
    }
    // Fenêtre main grisé quand mise à jour
    Rectangle {
        visible: windowsSensorUpdate.visible || windowsInterfaceUpdate.visible
        z: 100000
        width: window.width
        height: window.height
        color: "#000000"
        opacity: 0.2
    }
    // MessageDialog - No sensor update
    MessageDialogPerso {
        id: messageDialogNoSensorAvailableUpdate
        title: qsTr("Sensor update")
        text: qsTr("The sensor already has the latest update.")
    }
    // MessageDialog - No interface update
    MessageDialogPerso {
        id: messageDialogNoInterfaceAvailableUpdate
        title: qsTr("Interface update")
        text: qsTr("The interface already has the latest update.")
    }
    // MessageDialog - Update sensor success
    MessageDialogPerso {
        id: messageDialogSensorUpdateSuccess
        title: qsTr("Sensor update")
        text: qsTr("Sensor updated succefully.")
        onClosed: {
            windowsSensorUpdate.close()
        }
    }
    // MessageDialog - Update interface success
    MessageDialogPerso {
        id: messageDialogInterfaceUpdateSuccess
        title: qsTr("Interface update")
        text: qsTr("Interface updated succefully.")
        onClosed: {
            windowsInterfaceUpdate.close()
        }
    }
    MessageDialog {
        id: messageDialogUpdateInterfaceError
        title: ((windowsSensorUpdate.visible)?qsTr("Sensor"):qsTr("Interface"))+qsTr(" update")
        text: qsTr("Update ")+ ((windowsSensorUpdate.visible)?qsTr("sensor"):qsTr("interface"))+qsTr(" error")
    }

    property bool bBackgroundOpaciteVisible: false
    // Fenêtre main grisé
    Rectangle {
        visible: bBackgroundOpaciteVisible
        //x: 229
        //y: 0
        z: 100000
        width: window.width
        height: window.height
        color: "#000000"
        opacity: 0.2
    }
}

