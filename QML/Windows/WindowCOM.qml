import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12

import "../Component"
import "../Windows"
import "../"

Window {
    id: windowCOM
    visible: true
    width: 200
    height: 120

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    property bool bAuto: true
    property string m_sPortComName: ""
    property bool bBusyRefreshAutoDetection: false
    property bool bClickOnManualWindow: false

    color: "#efeef4"
    title: qsTr("Port COM selection")

    // @disable-check M16
    onClosing: {
        console.log("Child COM window is closing");

        // Récupération du nom du port COM
        m_sPortComName = tCOMEvent.sFGetPortComName();

        // Test si le COM a été initialisé
        if( m_sPortComName != "" ){
            // Tentative d'ouverture du port précédent
            if( tCOMEvent.bFOpenCOMPort( m_sPortComName ) ){
                // Fermeture acceptée
            }
            // Echec de l'ouverture du COM précédent
            else{
                windowOpacity.visible = false
                //windowCOMManualMain.modality = Qt.NonModal
                windowCOM.modality = Qt.NonModal
            }
        }
        // Pas de COM initialisé
        else{
            // Si c'est une demande d'affichage de la fenêtre manual
            if( bClickOnManualWindow ){
                bClickOnManualWindow = false
            }
            else{
                // Fermeture de l'application au complet
                // si pas de port COM de sélectionné
                Qt.quit()
            }
        }


        // Réouverture du port com précédent

        // Clear
        //vFMAIN_ClearData();


        //%%TODO - Á dégager au profit de la disparition du refresh sensor
        //tCloseEvent.exitHandler();
        //windowCOMMain.modality = Qt.NonModal
        //vFMAIN_CloseWindows();
        //window.close()
    }

    //=============================================================
    // Connection
    //=============================================================
    //--------------------------------
    // Ouverture de la fenêtre COM port
    //--------------------------------
    function vFOpenCOMPortListSignal() {
        console.log( "inputCOMPortModel.count: "+inputCOMPortModel.count )
        console.log( "inputPortCom.count: "+inputSelectPortCom.count )
        // Si des lignes
        if( inputCOMPortModel.count > 0 ){
            console.log("inputPortCom: Index 0 selected")
            //inputCOMPortModel.
            // Index à 0
            inputSelectPortCom.currentIndex = 0
        }
    }
    //--------------------------------
    // Clear de la liste COM port
    //--------------------------------
    function vFClearCOMPortListSignal() {
        // Effacement de la liste des ports COM
        inputCOMPortModel.clear()
    }
    //--------------------------------
    // Ajout de la liste COM port
    //--------------------------------
    function vFAddCOMPortSignal( sCOMPort, iPortCom, iSN_Y, uliSN_N, sSN ) {
        console.log("==vFAddCOMPortSignal==")
        console.log( "iPortCom = "+ iPortCom );
        console.log( "iSN_Y = "+ iSN_Y );
        console.log( "uliSN_N = "+ uliSN_N );
        console.log( "sCOMPort = "+sCOMPort);
        inputCOMPortModel.append({text: sCOMPort, num: iPortCom, sn:sSN });
    }

    //===================================================================
    //
    // Fenêtre sélection AUTOMATIQUE capteur
    //
    //===================================================================
    Text {
        id: textAuto
        visible:bAuto
        x: 19
        y: 11
        text: qsTr("Select your sensor interface")
        anchors.horizontalCenter: parent.horizontalCenter
        font.weight: Font.Medium
        font.bold: false
        font.family: "Open Sans"
        font.pixelSize: 12
    }
    ComboBox {
        id: inputSelectPortCom
        visible:bAuto
        y: 37
        width: 120
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: 15
        // Index utilisé pour remplir la partie visible du combobox
        textRole: "text"
        model: ListModel {
            id: inputCOMPortModel
        }
        //model: [ "Temperature", "Conductivity", "Pressure" ]
        Layout.preferredHeight: 30
        Layout.preferredWidth: ( parent.width - 15 ) / 3
        //enabled: bStarMeasureSwitchState && m_bStartAverage
        //enabled: m_bStartAverage

        // Touche ENTER (gauche)
        Keys.onEnterPressed:{
            console.log( "Press enter" );
            btnConfirmPortCOM.clicked();
        }
        // Touche ENTER (droite)
        Keys.onReturnPressed:{
            console.log( "Press onReturnPressed" );
            btnConfirmPortCOM.clicked();
        }
    }
    // Bouton refresh
    Button {
        id: btnRefreshComPort
        x: 151
        width: 35
        height: 30
        text: qsTr("Refresh")
        anchors.top: inputSelectPortCom.top
        anchors.topMargin: 0
        display: AbstractButton.IconOnly
        icon.source: "qrc:/Logo/Factory/factory-factory.png"
        icon.height: 30
        icon.width: 22
        anchors.right: parent.right
        anchors.rightMargin: 15
        visible: ( bAuto && !bBusyRefreshAutoDetection )
        //visible: false
        onClicked: {
            console.log("==tCOMEvent.uiFDetectionCOMFtdiInterfaceNke==");
            // Affichage du refresh
            bBusyRefreshAutoDetection = true;
            // Détection
            tCOMEvent.uiFDetectionCOMFtdiInterfaceNke();
            //uiFDetectionCOMFtdiInterfaceNke
            tCOMEvent.vFFillComPortAutomaticWindow();
            // Sélection de l'index 0 dans le ComboBox
            tCOMEvent.vFOpenCOMPortListSignal();
        }
    }
    Timer {
        interval: 500; running: bBusyRefreshAutoDetection; repeat: false
        onTriggered: {
            // Cache l'affichage du refresh
            bBusyRefreshAutoDetection = false;
        }
    }
    BusyIndicatorPerso {
        id: busyRefreshAutoDetection
        width: 36
        height: 37
        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.verticalCenter: parent.verticalCenter
        visible: ( bAuto && bBusyRefreshAutoDetection )
        anchors.verticalCenter: inputSelectPortCom.verticalCenter
        //running: false

        running: bBusyRefreshAutoDetection
        x: 151
        y: 79
        anchors.right: parent.right
        anchors.rightMargin: 14
    }
    // Ouverture fenêtre sélection manuel du port COM
    Button {
        id: btnGoToManualPortCOM
        visible:bAuto
        y: 79
        width: 75
        height: 28
        text: qsTr("Manual...")
        anchors.left: inputSelectPortCom.left
        anchors.leftMargin: 0
        // Si validation
        onClicked: {
            console.log("==btnGoToManualPortCOM==");
            // On va ouvrir la fenêtre manuel
            bClickOnManualWindow = true;
            // Fermeture de la fenêtre
            windowCOM.close();
            vFMAINWindowCOM_OpenManualWindow();
        }
    }
    Button {
        id: btnConfirmPortCOM
        visible:bAuto
        x: 106
        width: 75
        height: 28
        text: qsTr("Submit")
        anchors.top: btnGoToManualPortCOM.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 15
        // Si validation
        onClicked: {
            console.log("==btnConfirmPortCOM==");
            console.log( "inputPortCom.currentIndex = "+inputSelectPortCom.currentIndex )
            console.log( "Port COM = "+inputCOMPortModel.get( inputSelectPortCom.currentIndex ).num );

            //
            //tMainData.sInterfaceSN
            tMainData.sInterfaceSN = inputCOMPortModel.get( inputSelectPortCom.currentIndex ).sn

            // On fait remonter l'index du port COM utilisé
            vFMAINWindowCOM_ValidateCOMPort( inputCOMPortModel.get( inputSelectPortCom.currentIndex ).num )
        }
    }

    //===================================================================
    //
    // Fenêtre sélection MANUEL capteur
    //
    //===================================================================
    Text {
        id: textManual
        visible:!bAuto
        x: 19
        y: 11
        height: 17
        text: qsTr("Enter your sensor interface")
        anchors.horizontalCenter: parent.horizontalCenter
        font.weight: Font.Medium
        font.bold: false
        font.family: "Open Sans"
        font.pixelSize: 12
    }
    Text {
        id: element1
        visible:!bAuto
        x: 19
        y: 44
        //width: 56
        height: 17
        text: qsTr("COM")
        anchors.right: inputComPortManual.left
        anchors.rightMargin: 15
        font.pixelSize: 12
        font.bold: false
        font.family: "Open Sans"
        font.weight: Font.Medium
    }
    TextField {
        id: inputComPortManual
        visible:!bAuto
        x: 81
        y: 37
        width: 72
        height: 30
        //width: parent.width / 3 - 5
        text: qsTr("")
        //height: 30
        activeFocusOnPress: true
        selectByMouse: true
        //color: "#ffffff"
        //%%AP - Test
        //ToolTip.visible: right
        //ToolTip.text: qsTr("Invalide value")

        // Touche ENTER (gauche)
        Keys.onEnterPressed:{
            console.log( "Press enter" );
            btnConfirmPortCOMManual.clicked();
        }
        // Touche ENTER (droite)
        Keys.onReturnPressed:{
            console.log( "Press onReturnPressed" );
            btnConfirmPortCOMManual.clicked();
        }


        ToolTip {
            id: tooltipInputManual
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

//    // Touche ENTER (gauche)
//    Keys.onEnterPressed:{
//        console.log( "Press enter" );
//        btnConfirmPortCOMManual.clicked();
//    }
//    // Touche ENTER (droite)
//    Keys.onReturnPressed:{
//        console.log( "Press onReturnPressed" );
//        btnConfirmPortCOMManual.clicked();
//    }

    Button {
        id: btnConfirmPortCOMManual
        visible:!bAuto
        x: 44
        y: 79
        width: 100
        height: 28
        text: qsTr("Submit")
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter

//        // Touche ENTER (gauche)
//        Keys.onEnterPressed:{
//            console.log( "Press enter" );
//            btnConfirmPortCOMManual.clicked();
//        }
//        // Touche ENTER (droite)
//        Keys.onReturnPressed:{
//            console.log( "Press onReturnPressed" );
//            btnConfirmPortCOMManual.clicked();
//        }
        // Si validation
        onClicked: {
            console.log("==btnConfirmPortCOMManual==");
            console.log( "inputComPortManual = "+inputComPortManual.text )

            var uiPortComIndex = parseInt( inputComPortManual.text );

            if(  ( uiPortComIndex < 0 )
              || ( uiPortComIndex > 255 ) )
            {
                // On désactive l'état occupé
                //calibrationID.bBusyState  = false
                //busyConfigSettings.visible  = false
                //errorConfigSettings.visible = true
                tooltipInputManual.show("Value must be between 0 and 255")
                return;
            }
            // Vérification si bien un entier
            var fComPort        = parseFloat( inputComPortManual.text );
            var fComPortRounded = parseFloat(parseInt(fComPort));
            // Vérification fonctionnement avec un entier
            if( fComPort !== fComPortRounded )
            {
                // On désactive l'état occupé
                //calibrationID.bBusyState  = false
                //busyConfigSettings.visible  = false
                //errorConfigSettings.visible = true
                tooltipInputManual.show("Value must be an integer")
                return;
            }
            // On fait remonter l'index du port COM utilisé
            vFMAINWindowCOM_ValidateManualCOMPort( uiPortComIndex )
        }
    }
}




