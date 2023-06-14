import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import QtQuick.Extras 1.4

import "Component"
import "Windows"

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Numéro de version interface
    property int uiInterfaceMajor: tMainData.uiInterfaceVersionMajor
    property int uiInterfaceMinor: tMainData.uiInterfaceVersionMinor
    property int uiInterfaceBuild: tMainData.uiInterfaceVersionBuild
    // Mise à jour interface
    property string m_sInterfaceInfo:        interfaceUpdateData.sInterfaceInfo
    property string m_sUpdateVersion:        interfaceUpdateData.sUpdateVersion
    property int    m_uiUpdateProgression:   interfaceUpdateData.uiUploadProgression
    // Mise à jour capteur
    property int m_uiUpdateProgressBar: 0
    property bool m_bUpdateBusyRefresh: false
    property bool m_bUpdateBtnRefreshVisible: true
    // Reset capteur
    property int m_uiResetSensorProgressBar: 0
    property bool m_bResetSensorBusyRefresh: false
    property bool m_bResetSensorBtnVisible: true
    // Compensation CT
    property string m_sTCompensationA:     tDataSensorCT.sTCompensationA
    property string m_sTCompensationB:     tDataSensorCT.sTCompensationB
    property string m_sTCompensationC:     tDataSensorCT.sTCompensationC
    property string m_sTCompensationRaw20: tDataSensorCT.sTCompensationRaw20

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property int uiBlocWidthOffset: 60
    property string m_sInterfaceVersion: (uiInterfaceMajor!=0)?"V"+uiInterfaceMajor+"."+uiInterfaceMinor+"."+uiInterfaceBuild:""

    //=============================================================
    // Property
    //=============================================================
    property bool bUpdate: false
    property int  uiChannelID: 1
    property bool bEvosensParam: true
    property bool bCTParam: true
    property bool bUpdateInterface: false
    // Le numéro de version a été demandé
    property bool bInterfaceVersionOnRequest: false

    //==============================================================
    // Déclaration des signaux
    //==============================================================


    //=============================================================
    // Connection
    //=============================================================
    Connections {
        target: tModbusControler
        function onRequestInterfaceVersionNumberEndSignal(){
            console.log( "Request interface END QML" );
            // Refresh not busy
            bInterfaceVersionOnRequest = false
        }
    }
    // Signaux mise à jour interface
    Connections {
        target: tInterfaceUpdateControler
        function onManualUpdateSuccessSignal(){
            // La mise à jour est fini ici
            // Enleve la protection
            vFAllowUserClick();
            // Ouverture du messagde dialog
            messageDialogInterfaceUpdateSuccess.open();
        }
        // Fin de la demande de numéro de version
        function onUpdateSuccessSignal(){
            // Enleve la protection
            vFAllowUserClick();
            // Ouverture du messagde dialog
            messageDialogInterfaceUpdateSuccess.open();
        }
    }

    Connections {
        target:tFactoryEvent
        function onUpdateLoaderValueSignal( uiLoaderValue ) {
            console.log("onUpdateLoaderValueSignal EXIT")
            console.log( "bUpdate : "+ bUpdate );
            //return;
            // Si mise à jour
            if( !bUpdate ) return;
            console.log("==onUpdateLoaderValue==");
            console.log("uiLoaderValue = "+uiLoaderValue);
            // Mise à jour en cours
            if( parseInt(uiLoaderValue) < 100 ){
                //updateProgressBar.value = uiLoaderValue;
                //updateLoadNumber.text   = uiLoaderValue+" %";
                m_uiUpdateProgressBar = uiLoaderValue
            }
            // Si fin de mise à jour
            if( parseInt(uiLoaderValue) == 100 ){
                // Update
                m_uiUpdateProgressBar = 0
                m_bUpdateBusyRefresh = false
                // Bouton
                updatePartID.m_bUpdateBtnRefreshVisible = true
                // Autorisation du clique utilisateur
                vFAllowUserClick();
                // Affichage de la fenêtre
                updateSuccess.open();
                // Réinit
                bUpdate = false;
            }
        }
        function onResetLoaderValueSignal( uiLoaderValue ) {
            console.log("==onResetLoaderValueSignal==");
            console.log("uiLoaderValue = "+uiLoaderValue);

            // Mise à jour en cours
            if( parseInt(uiLoaderValue) < 100 ){
                resetProgressBar.value = uiLoaderValue;
                //updateLoadNumber.text = uiLoaderValue+" % (update)";
                resetLoadNumber.text = uiLoaderValue+" %";
            }
            // Si fin de mise à jour
            if( parseInt(uiLoaderValue) == 100 ){
                // Réinit
                resetProgressBar.value = 0;
                resetLoadNumber.text   = "0 %";
                // Cache les progress bar et number
                resetProgressBar.visible = false;
                resetLoadNumber.visible  = false;
                // Affichage du bouton
                btnResetRect.visible   = true;
                btnResetShadow.visible = true;
                // Autorisation du click utilisateur
                vFAllowUserClick();
                // Affichage de la fenêtre
                resetSuccess.open();
            }
        }

        function onConfigSettingsSignal( uiSN_Y, uiSN_N ) {
            console.log("uiSN_Y = "+uiSN_Y);
            console.log("uiSN_N = "+uiSN_N);
            inputSN_N.text = uiSN_N;
            inputSN_Y.text = uiSN_Y;

            //%%AP - 2020.05.19 -
            //inputRange.text =
        }
        function onConfigSettingsDisplayEvosensSignal( uiSN_Y,
                                                       uiSN_N,
                                                       uiIntegrationTime,
                                                       ucRange,
                                                       uiAverageSamples ) {
            console.log("== onConfigSettingsDisplayEvosensSignal ==");
            console.log("uiSN_Y = "+uiSN_Y);
            console.log("uiSN_N = "+uiSN_N);
            inputSN_N.text = uiSN_N;
            inputSN_Y.text = uiSN_Y;

            //%%AP - 2020.05.19 -
            inputIntegrationTime.text = uiIntegrationTime
            inputRange.text = ucRange
            inputAverageSample.text = uiAverageSamples
        }
        // Fin de l'enregistrement du numéro de série
        function onEndOfSNSubmitSignal() {
            //console.log()
            // Ajouter la sortie de la protection contre vFAllowUserClick()
            vFAllowUserClick();
            //
            busyConfigSettings.visible = false;
        }
        // Fin du refresh de donnée
        function onEndOfRefreshDataSignal() {
            // Uniquement en mode nke NON-prod
            if( bNkeMode || tMainData.bMetroMode ){
                // Apparition du busy à la place du bouton
                busyWriteConfig.visible   = false
                buttonWriteConfig.visible = true
            }
        }
    }

    //=============================================================
    // Message dialog
    //=============================================================
    MessageDialogPerso {
        id: updateSuccess
        title: qsTr("Update success")
        text:  qsTr("Sensor update finished successfully")
    }
    MessageDialogPerso {
        id: resetSuccess
        title: qsTr("Reset success")
        text:  qsTr("Sensor reset finished successfully")
    }

    //=============================================================
    // Fonction
    //=============================================================
    // Positionnement des différents blocs
    function vFFactoryPartRefreshBlocSize(){
        console.log("==vFFactoryPartRefreshBlocSize==");
        console.log("Etat: "+ (bO2 || bWiper || bGenericSensor || tMainData.bErrorSensorNeedUpdate ) )
        //
        if( bO2 || bWiper || bGenericSensor || tMainData.bErrorSensorNeedUpdate ){
            // Bloc update sensor
            m_sensorSoftwareBlocID.visible = true
            // Bloc update interface
            //updateInterface.anchors.top       = updateSensor.bottom
            //updateInterface.anchors.topMargin = 28
            // Si mode nke
            if( bNkeMode || tMainData.bMetroMode ){
                //
                if( bEvosensParam ){
                    // Taille de la fenêtre
                    uiFactoryHeight = 830
                    updatePartID.height = 900
                    //
                    paramOpen.height = 278
                }
                else if( bCTParam ){
                    // Taille de la fenêtre
                    uiFactoryHeight = 760
                    updatePartID.height = 800
                    //
                    paramOpen.height = 170
                }
                else{
                    // Taille de la fenêtre
                    uiFactoryHeight = 750
                    updatePartID.height = 750
                    //
                    paramOpen.height = 125
                }
            }
            else{
                // Taille de la fenêtre
                uiFactoryHeight = 600
                updatePartID.height = 600
                //
                paramOpen.height = 278
            }
        }
        else{
            // Taille de la fenêtre
            uiFactoryHeight = 600
            updatePartID.height = 600
            // Bloc update sensor
            m_sensorSoftwareBlocID.visible = false
            //
            paramOpen.height = 278
            // Bloc update interface
            //updateInterface.anchors.top       = triangleTitleFactory.bottom
            //updateInterface.anchors.topMargin = -15
        }

        console.log("==END vFFactoryPartRefreshBlocSize==");
    }

    // Quand affichage de l'onglet
    onVisibleChanged: {
        console.log("FACTORYPART - onVisibleChanged")
        // Si caché
        if(!visible){
            console.log("FACTORYPART - onVisibleChanged Hidden")
            // On quitte
            return;
        }
        // Si montré
        else{
            console.log("FACTORYPART - onVisibleChanged Visible")
        }
        updatePartID.uiChannelID = calibrationPart1.uiFGetCurrentChannelID()
        //
        // Si c'est un Tbd / Chla_nke / PhycoC_nke / PhycoE_nke / CDOM_nke / Trypto
        if(  ( uiChannelID ==  5 )
          || ( uiChannelID == 13 )
          || ( uiChannelID == 15 )
          || ( uiChannelID == 16 )
          || ( uiChannelID == 17 )
          || ( uiChannelID == 18 ) ){
            //
            bEvosensParam = true
        }
        else{
            //
            bEvosensParam = false
        }
        //
        // Si c'est un CT
        if(  ( uiChannelID == 4 )
          || ( uiChannelID == 3 ) ){
            //
            bCTParam = true
        }
        else{
            //
            bCTParam = false
        }
        // Refresh taille de bloc + positionnement
        vFFactoryPartRefreshBlocSize()
        console.log("== Factory.onVisibleChanged END ==")
    }

    //==============================================================
    // QML
    //==============================================================
    id: updatePartID
    implicitWidth: 662
    implicitHeight: 830
    height: 830

    /*
    // Background color
    Rectangle {
        anchors.fill: parent
        color: "#efeef4"
        z: -9
    }
    */

    Rectangle {
        id: rectangleTitleUpdate
        x: 0
        y: 0
        width: parent.width
        //height: 600
        height: parent.height
        color: "#efeef4"

        //-------------------------------
        // Titre H2
        //-------------------------------
        H2Item{
            id: m_H2ItemID
            sH2Title: "Factory"
            sH2Img: "qrc:/Logo/Factory/factory-factory.png"
            uiLogoWidth: 34
            anchors.top: parent.top
            z: 2
            anchors.topMargin: 20
        }

        //--------------------------------------------------
        // Bloc : Update interface usb
        //--------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: updateInterface
            samples: 30
            horizontalOffset: 0
            source: updateInterface
        }
        Rectangle {
            id: updateInterface

            width: updatePartID.width - 120
            //height: (tMainData.bCOMSlowMode)?80:126
            height: 126
            color: "#ffffff"

            //anchors.topMargin: 28
            //anchors.top: updateSensor.bottom
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            //anchors.topMargin: -15
            anchors.top: m_H2ItemID.bottom

            Rectangle {
                id: updateInterface2
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: updateInterfaceTxt
                    //x: 63
                    //y: 14
                    text: qsTr("USB tool software")
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
                id: columnLayoutInterface
                x: 0
                //y: 39
                width: parent.width
                height: parent.height - 44
                anchors.top: updateInterface2.bottom
                anchors.topMargin: 4

                //--------------------------------------------------
                // Bouton update open file
                //--------------------------------------------------
                RowLayout {
                    id: rowUpdateInterfaceOpen
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    //visible: !(tMainData.bCOMSlowMode)

                    //Row {
                    Rectangle {
                        id: rowUpdateInterfaceOpen2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgUpdateInterfaceOpen
                            width: 30
                            height: 18
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                        }

                        Text {
                            id: txtUpdateInterfaceOpen
                            text: qsTr("Update")
                            height: parent.height
                            anchors.left: imgUpdateInterfaceOpen.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    //Row {
                    Rectangle {
                        id: rowUpdateInterfaceOpen3
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height


                        ////////////////////////////////////////
                        //-------------
                        // Bouton
                        //-------------
                        DropShadow {
                            id: btnUpdateInterfaceOpenShadow
                            color: "#20000000"
                            radius: 10
                            verticalOffset: 0
                            anchors.fill:btnUpdateInterfaceOpenRect
                            samples: 25
                            horizontalOffset: 0
                            source: btnUpdateInterfaceOpenRect
                            visible: (m_uiUpdateProgression==0)
                        }

                        BlueButtonPerso{
                            id: btnUpdateInterfaceOpenRect
                            width: 130
                            height: 28
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Check for update")
                            visible: (m_uiUpdateProgression==0)
                            onButtonClicked: {
                                console.log(" clicked" )
                                tInterfaceUpdateControler.checkUpdateAvailability(true)
                            }
                        }

                        ProgressBarPerso {
                            id: updateInterfaceProgressBar
                            //width: 200
                            width: parent.width
                            height: 8
                            anchors.verticalCenterOffset: 7
                            anchors.horizontalCenterOffset: 0
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            to: 100
                            value: m_uiUpdateProgression
                            //visible: true
                            visible: (m_uiUpdateProgression!=0)
                        }
                        Text {
                            id: updateInterfaceLoadNumber
                            y: 10
                            //x: 249
                            //y: 62
                            text: m_uiUpdateProgression+" %"
                            anchors.bottom: updateInterfaceProgressBar.top
                            anchors.bottomMargin: 1
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 12
                            //visible: true
                            visible: (m_uiUpdateProgression!=0)
                        }
                    }
                    ////////////////////////////////
                    Rectangle {
                        id: rowUpdateInterfaceOpenDummy
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        color: "#ffffffff"
                        ButtonPerso {
                            id: interfaceUpdateCheckBtn
                            height: 28
                            width: 100
                            text: qsTr("Open file...")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked: {
                                fileDialogInterface.open()
                            }
                        }
                    }
                }

                Rectangle {
                    id: updateInterfaceLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                    //visible: !(tMainData.bCOMSlowMode)
                }

                //--------------------------------------------------
                // Affichage version interface
                //--------------------------------------------------
                RowLayout {
                    id: rowInterfaceVersion
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30

                    //Row {
                    Rectangle {
                        id: rowInterfaceVersion2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgInterfaceVersion
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-manufacturer.png"
                        }

                        Text {
                            id: txtInterfaceVersion
                            text: qsTr("Version")
                            height: parent.height
                            anchors.left: imgInterfaceVersion.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    ////////////////////////////////
                    Rectangle {
                        id: rowInterfaceVersionDummy
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        color: "#ffffffff"
                        Text {
                            id: interfaceVersionTxt
                            //x: 63
                            //y: 14
                            //text: qsTr("--")
                            text: m_sInterfaceVersion
                            horizontalAlignment: Text.AlignHCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.DemiBold
                            font.family: "Open Sans"
                            font.bold: false
                            font.pixelSize: 13
                        }
                    }
                    Rectangle {
                        id: interfaceVersionrefresh
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        color: "#ffffffff"
                        ButtonPerso {
                            id: interfaceVersionrefreshBtn
                            height: 28
                            width: 100
                            text: qsTr("Refresh")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            visible: !bInterfaceVersionOnRequest

                            onClicked: {
                                // Requête numéro version interface
                                tModbusControler.requestInterfaceVersionNumber()
                                // Demande de la version
                                bInterfaceVersionOnRequest = true
                            }
                        }
                        BusyIndicatorPerso {
                            id: busyRefreshInterfaceUpdate
                            width: 31
                            height: 32
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: bInterfaceVersionOnRequest
                        }
                    }
                }

            }

        } // Fin bloc update interface

        //--------------------------------------------------
        // Bloc : Update Sensor
        //--------------------------------------------------
        FactorySensorSoftwareBloc{
            id:m_sensorSoftwareBlocID
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.leftMargin: uiBlocWidthOffset
            anchors.rightMargin: uiBlocWidthOffset
            anchors.top: updateInterface.bottom
            anchors.topMargin: 28
            // Update
            m_uiUpdateProgressBar:  updatePartID.m_uiUpdateProgressBar
            m_bBusyRefresh:         updatePartID.m_bUpdateBusyRefresh
            m_bBtnRefreshVisible:   updatePartID.m_bUpdateBtnRefreshVisible
            // Reset
            m_uiResetProgressBar:  updatePartID.m_uiResetSensorProgressBar
            m_bResetBusyRefresh:   updatePartID.m_bResetSensorBusyRefresh
            m_bResetBtnVisible:    updatePartID.m_bResetSensorBtnVisible

            // Signaux
            onCheckUpdateButtonClicked:{
                console.log(" clicked" )
                tSensorUpdateControler.checkUpdateAvailability()
            }
            onOpenFileComButtonClicked:{
                fileDialog.open()
            }
            onTriggerResetButtonClicked:{
                // Disparition du bouton
                //btnResetShadow.visible = false
                //btnResetRect.visible   = false
                // Apparition du pourcentage et load bar
                //resetProgressBar.visible = true
                //resetLoadNumber.visible  = true
                // Protection contre click
                vFLockUserClick();
                // Reset
                tFactoryEvent.startReset();
            }
        }

        /*
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: updateSensor
            samples: 30
            horizontalOffset: 0
            source: updateSensor
            visible: ( bWiper || bGenericSensor || bO2 || tMainData.bErrorSensorNeedUpdate )
        }
        Rectangle {
            id: updateSensor
            width: updatePartID.width - 120
            height: (tMainData.bCOMSlowMode)?80:126
            color: "#ffffff"
            visible: ( bWiper || bGenericSensor || bO2 || tMainData.bErrorSensorNeedUpdate )

            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.topMargin: -15
            //anchors.top: triangleTitleFactory.bottom
            anchors.topMargin: 28
            anchors.top: updateInterface.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: updateSensor2
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: updateSensorTxt
                    text: qsTr("WiMo sensor software")
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
                id: columnLayoutRefresh
                x: 0
                //y: 39
                width: parent.width
                height: parent.height - 44
                anchors.top: updateSensor2.bottom
                anchors.topMargin: 4

                //--------------------------------------------------
                // Bouton update open file
                //--------------------------------------------------
                RowLayout {
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    visible: !(tMainData.bCOMSlowMode)
                    //visible: false

                    Rectangle {
                        id: rowupdateSensor2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgupdateSensor
                            width: 30
                            height: 18
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                        }

                        Text {
                            id: txtupdateSensor
                            text: qsTr("Update")
                            height: parent.height
                            anchors.left: imgupdateSensor.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }

                    Rectangle {
                        id: rowupdateSensor3
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        ////////////////////////////////////////
                        //-------------
                        // Bouton
                        //-------------
                        DropShadow {
                            id: btnupdateSensorShadow
                            color: "#20000000"
                            radius: 10
                            verticalOffset: 0
                            anchors.fill:btnupdateSensorRect
                            samples: 25
                            horizontalOffset: 0
                            source: btnupdateSensorRect
                            //visible: false
                        }

                        BlueButtonPerso{
                            id: btnupdateSensorRect
                            width: 130
                            height: 28
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Check for update")
                            onButtonClicked: {
                                console.log(" clicked" )
                                tSensorUpdateControler.checkUpdateAvailability()
                            }
                        }

                        ProgressBarPerso {
                            id: updateProgressBar
                            //width: 200
                            width: parent.width
                            height: 8
                            anchors.verticalCenterOffset: 7
                            anchors.horizontalCenterOffset: 0
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            to: 100
                            value: 0
                            //visible: true
                            visible: false
                        }
                        Text {
                            id: updateLoadNumber
                            y: 10
                            //x: 249
                            //y: 62
                            text: "0 %"
                            anchors.bottom: updateProgressBar.top
                            anchors.bottomMargin: 3
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 12
                            //visible: true
                            visible: false
                        }
                    }
                    ////////////////////////////////
                    Rectangle {
                        id: rowupdateSensorDummy
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        color: "#ffffffff"

                        ButtonPerso {
                            id: sensorUpdateCheckBtn
                            height: 28
                            width: 100
                            text: qsTr("Open file...")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked: {
                                fileDialog.open()
                            }
                        }
                    }
                }

                Rectangle {
                    id: updateLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                    visible: !(tMainData.bCOMSlowMode)
                }

                //--------------------------------------------------
                // Bouton reset
                //--------------------------------------------------
                RowLayout {
                    id: rowReset
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30

                    //Row {
                    Rectangle {
                        id: rowReset2
                        width: ( parent.width - 25 ) / 4
                        Layout.preferredWidth: ( parent.width - 15 ) / 4
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgReset
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source:  "qrc:/Logo/Factory/factory-reset.png"
                        }

                        Text {
                            id: txtReset
                            text: qsTr("Reset")
                            height: parent.height
                            anchors.left: imgReset.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rowReset3
                        width: ( parent.width - 25 ) / 2
                        Layout.preferredWidth: ( parent.width - 15 ) / 2
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        ////////////////////////////////////////
                        //-------------
                        // Bouton
                        //-------------
                        DropShadow {
                            id: btnResetShadow
                            color: "#20000000"
                            radius: 10
                            verticalOffset: 0
                            anchors.fill:btnResetRect
                            samples: 25
                            horizontalOffset: 0
                            source: btnResetRect
                        }


                        BlueButtonPerso{
                            id: btnResetRect
                            width: 130
                            height: 28
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Trigger")
                            onButtonClicked: {
                                // Disparition du bouton
                                btnResetShadow.visible = false
                                btnResetRect.visible   = false
                                // Apparition du pourcentage et load bar
                                resetProgressBar.visible = true
                                resetLoadNumber.visible  = true
                                // Protection contre click
                                vFLockUserClick();
                                // Reset
                                tFactoryEvent.startReset();
                            }
                        }

                        ProgressBarPerso {
                            //ProgressBar {
                            id: resetProgressBar
                            width: 200
                            height: 8
                            anchors.verticalCenterOffset: 7
                            anchors.horizontalCenterOffset: 0
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            to: 100
                            value: 0
                            //visible: true
                            visible: false
                        }
                        Text {
                            id: resetLoadNumber
                            y: 10
                            //x: 249
                            //y: 62
                            text: "0 %"
                            anchors.bottom: resetProgressBar.top
                            anchors.bottomMargin: 3
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 12
                            //visible: true
                            visible: false
                        }
                    }
                    ////////////////////////////////
                    Rectangle {
                        id: rowResetDummy
                        width: ( parent.width - 25 ) / 4
                        Layout.preferredWidth: ( parent.width - 15 ) / 4
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        color: "#ffffffff"
                    }
                }
            }

        } // Fin bloc refresh
        */
        //--------------------------------------------------
        // Bloc : Save/write config
        //--------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: configOpen
            samples: 30
            horizontalOffset: 0
            source: configOpen
            visible: ( bWiper || bGenericSensor || bO2 ) && ( bNkeMode || bNkeProdMode || tMainData.bMetroMode )
        }
        Rectangle {
            id: configOpen
            width: updatePartID.width - 120
            //height: 84
            height: 80
            color: "#ffffff"
            //anchors.top: updateInterface.bottom
            //anchors.top: updateSensor.bottom
            anchors.top: m_sensorSoftwareBlocID.bottom
            anchors.topMargin: 28
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            visible: ( bWiper || bGenericSensor || bO2 ) && ( bNkeMode || bNkeProdMode || tMainData.bMetroMode )
            Rectangle {
                id: configOpen2
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: configOpenTxt
                    //x: 63
                    //y: 14
                    text: qsTr("Configuration")
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
                id: columnLayoutCOnfig
                x: 0
                //y: 39
                width: parent.width
                height: parent.height - configOpen2.height
                anchors.top: configOpen2.bottom

                //--------------------------------------------------
                // Bouton config save/write file
                //--------------------------------------------------
                RowLayout {
                    id: rowconfigOpen
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30

                    //Row {
                    Rectangle {
                        id: rowConfigOpen2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgConfigOpen
                            width: 30
                            height: 22
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-version.png"
                        }

                        Text {
                            id: txtConfigOpen
                            text: qsTr("Config")
                            height: parent.height
                            anchors.left: imgConfigOpen.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rowConfigOpen3
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        ButtonPerso {
                            id: buttonSaveConfig
                            height: 28
                            width: 100
                            text: qsTr("Export config")
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked: {
                                //tFactoryEvent.startSaveConfig("","Config");
                                //messageDialog.open()
                                //messageDialogSaveConfig.open()
                                //windowConfigDisplay.open()
                                vFMAINWindowConfigDisplayOpen()
                            }
                        }
                    }
                    ////////////////////////////////
                    Rectangle {
                        id: rowConfigOpenDummy
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        color: "#ffffffff"
                        ButtonPerso {
                            id: buttonWriteConfig
                            //x: 452
                            //y: 430
                            height: 28
                            width: 100
                            text: qsTr("Write config")
                            font.pointSize: 8
                            font.family: "Open Sans"
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            visible: ( ( bNkeMode || tMainData.bMetroMode ) && bNkeProdMode == false )

                            onClicked: {
                                fileDialogWriteConfig.open();
                            }
                        }
                        BusyIndicatorPerso {
                            id: busyWriteConfig
                            width: 31
                            height: 32
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: false
                        }

                    }
                }
            }
        }

        //--------------------------------------------------
        // Bloc : Param specifique
        //--------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: paramOpen//configOpen
            samples: 30
            horizontalOffset: 0
            source: paramOpen
            visible: ( bWiper || bGenericSensor || bO2 ) && ( bNkeMode || tMainData.bMetroMode )
        }
        Rectangle {
            id: paramOpen
            width: updatePartID.width - 120
            //height: ( bEvosensParam ) ? 278:85
            height: 278
            color: "#ffffff"
            anchors.top: configOpen.bottom
            anchors.topMargin: 28
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            visible: ( bWiper || bGenericSensor || bO2 ) && ( bNkeMode || tMainData.bMetroMode )
            Rectangle {
                id: paramOpen2
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: paramOpenTxt
                    //x: 63
                    //y: 14
                    text: qsTr("Sensor parameters")
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
                id: columnLayoutParameter
                x: 0
                //y: 39
                width: parent.width
                height: parent.height - 44 - 38
                anchors.top: paramOpen2.bottom
                anchors.topMargin: 5

                //------------------------------
                // SN
                //------------------------------
                RowLayout {
                    id: rowSN
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowSN2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image {
                            id: imgSN
                            source: "qrc:/Logo/Dashboard/dashboard-SN2.png"
                            width: 30
                            //height: 23
                            height: 20
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtSN
                            text: qsTr("Serial number")
                            height: parent.height
                            anchors.left: imgSN.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleSN4
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        TextField {
                            id: inputSN_Y
                            width: parent.width / 3 - 5
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true

                            ToolTip {
                                id: tooltipSN_Y
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
                            id: txtSNSpace
                            text: qsTr(" - ")
                            anchors.left: inputSN_Y.right
                            anchors.leftMargin: 0
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                        TextField {
                            id: inputSN_N
                            width: parent.width * 2 / 3 - 5
                            text: qsTr("")
                            renderType: Text.QtRendering
                            anchors.left: txtSNSpace.right
                            anchors.leftMargin: 0
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true

                            ToolTip {
                                id: tooltipSN_N
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
                        id: rectangleAverageDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                } // RowLayout Average
                //--- Ligne ---
                Rectangle {
                    id: serialNoLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }
                //------------------------------
                // Intégration time
                //------------------------------
                RowLayout {
                    id: rowIntegrationTime
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20
                    visible: bEvosensParam

                    Rectangle {
                        id: rowIntegrationTime2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgIntegrationTime
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtIntegrationTime
                            text: qsTr("Integration time")
                            height: parent.height
                            anchors.left: imgIntegrationTime.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleIntegrationTime4
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        TextField {
                            id: inputIntegrationTime
                            width: parent.width
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true

                            ToolTip {
                                id: tooltipIntegrationTime
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
                        id: rectangleIntegrationTimeDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                } // RowLayout Integration time
                //--- Ligne ---
                Rectangle {
                    id: integrationTimeLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                    visible: bEvosensParam
                }
                //------------------------------
                // Range
                //------------------------------
                RowLayout {
                    id: rowRange
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20
                    visible: bEvosensParam

                    Rectangle {
                        id: rowRange2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgRange
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtRange
                            text: qsTr("Range")
                            height: parent.height
                            anchors.left: imgRange.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleRange4
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        TextField {
                            id: inputRange
                            width: parent.width
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true

                            ToolTip {
                                id: tooltipRange
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
                        id: rectangleRangeDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                } // RowLayout range
                //--- Ligne ---
                Rectangle {
                    id: rangeLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                    visible: bEvosensParam
                }
                //------------------------------
                // Average Sample
                //------------------------------
                RowLayout {
                    id: rowAverageSample
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20
                    visible: bEvosensParam

                    Rectangle {
                        id: rowAverageSample2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgAverageSample
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtAverageSample
                            text: qsTr("Average sample")
                            height: parent.height
                            anchors.left: imgAverageSample.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleAverageSample4
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        TextField {
                            id: inputAverageSample
                            width: parent.width
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true

                            ToolTip {
                                id: tooltipAverageSample
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
                        id: rectangleAverageSampleDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                } // RowLayout range
                //--- Ligne ---
                Rectangle {
                    id: averageSampleLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                    visible: bEvosensParam
                }
                //------------------------------
               // CT Compensation coefficient
               //------------------------------
               RowLayout {
                   id: rowCTCompensation
                   Layout.preferredWidth: parent.width - 20
                   Layout.preferredHeight: 30
                   Layout.leftMargin: 10
                   width: parent.width - 20
                   visible: bCTParam

                   Rectangle {
                       id: rowCTCompensation2
                       width: ( parent.width - 25 ) / 3
                       Layout.preferredWidth: ( parent.width - 15 ) / 3
                       height: parent.height
                       Layout.preferredHeight: parent.height
                       Image{
                           id: imgCTCompensation
                           source: "qrc:/Logo/Calibration/calib-average.png"
                           width: 30
                           height: 16

                           fillMode: Image.PreserveAspectFit
                           anchors.verticalCenter: parent.verticalCenter
                       }
                       Text {
                           id: txtCTCompensation
                           text: qsTr("Compensation T°")
                           height: parent.height
                           anchors.left: rowCTCompensation2.left
                           anchors.leftMargin: 40
                           anchors.verticalCenter: parent.verticalCenter
                           verticalAlignment: Text.AlignVCenter
                           font.family: "open sans"
                           font.pixelSize: 12
                           font.weight: Font.DemiBold
                       }
                   }

                   TextField {
                       id: inputCTCompensationA1
                       //text: qsTr("")
                       text: updatePartID.m_sTCompensationA
                       height: 30
                       activeFocusOnPress: true
                       Layout.preferredHeight: 30
                       Layout.preferredWidth: ( parent.width - 15 ) / 8
                       selectByMouse: true
                       // Si activation
                       enabled: !bNkeProdMode
                   }
                   Text {
                       id: txtCTCompensationA1
                       text: qsTr("T²+")
                       height: parent.height
                       verticalAlignment: Text.AlignVCenter
                       font.family: "open sans"
                       font.pixelSize: 12
                       font.weight: Font.DemiBold
                   }
                   TextField {
                       id: inputCTCompensationA2
                       //text: qsTr("")
                       text: updatePartID.m_sTCompensationB
                       height: 30
                       activeFocusOnPress: true
                       Layout.preferredHeight: 30
                       Layout.preferredWidth: ( parent.width - 15 ) / 8
                       selectByMouse: true
                       // Si activation
                       enabled: !bNkeProdMode
                   }
                   Text {
                       id: txtCTCompensationA2
                       text: qsTr("T+")
                       height: parent.height
                       verticalAlignment: Text.AlignVCenter
                       font.family: "open sans"
                       font.pixelSize: 12
                       font.weight: Font.DemiBold
                   }
                   TextField {
                       id: inputCTCompensationA3
                       text: updatePartID.m_sTCompensationC
                       height: 30
                       activeFocusOnPress: true
                       Layout.preferredHeight: 30
                       Layout.preferredWidth: ( parent.width - 15 ) / 8
                       selectByMouse: true
                       // Si activation
                       enabled: !bNkeProdMode
                       onEditingFinished: {
                           console.log("onEditingFinished")
                           //updatePartID.m_sTCompensationC=inputCTCompensationA3.text
                           //tDataSensorCT.setTCompensationC(inputCTCompensationA3.text)
                           //inputCTCompensationA3.text=updatePartID.m_sTCompensationC
                       }
                   }
                   Text {
                       id: txtCTCompensationRaw20
                       text: qsTr("Raw20=")
                       height: parent.height
                       verticalAlignment: Text.AlignVCenter
                       font.family: "open sans"
                       font.pixelSize: 12
                       font.weight: Font.DemiBold
                   }
                   TextField {
                       id: inputCTCompensationRaw20
                       //text: qsTr("")
                       text: updatePartID.m_sTCompensationRaw20
                       height: 30
                       activeFocusOnPress: true
                       Layout.preferredHeight: 30
                       Layout.preferredWidth: ( parent.width - 15 ) / 8
                       selectByMouse: true
                       // Si activation
                       enabled: !bNkeProdMode
                   }
               } // RowLayout Average

               //--- Ligne ---
               Rectangle {
                   id: compensationLine
                   width: parent.width
                   height: 1
                   color: "#62888888"
                   Layout.fillWidth: true
                   visible: bCTParam
               }
            }

            ButtonPerso {
                id: btnConfirmSettings
                width: 100
                height: 28
                text: qsTr("Submit")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8

                onClicked: {
                    // On désactive l'état d'erreur si présent
                    errorConfigSettings.visible = false
                    // On fait apparaître le busy
                    busyConfigSettings.visible = true

                    //--------------
                    // Average
                    //--------------
                    //console.log( "inputAverage: "+inputAverage.text )
                    var uiSN_Y, uiSN_N;
                    uiSN_Y = parseInt( inputSN_Y.text );
                    uiSN_N = parseInt( inputSN_N.text );
                    // Test d'intégrité
                    if(  ( uiSN_Y < 0 )
                      || ( uiSN_Y > 255 ) )
                    {
                        // On désactive l'état occupé
                        //calibrationID.bBusyState  = false
                        busyConfigSettings.visible  = false
                        errorConfigSettings.visible = true
                        tooltipSN_Y.show(qsTr("Value must be between 0 and 255"))
                        return;
                    }
                    if(  ( uiSN_N < 0 )
                      || ( uiSN_N > 65535 ) )
                    {
                        // On désactive l'état occupé
                        //calibrationID.bBusyState  = false
                        busyConfigSettings.visible  = false
                        errorConfigSettings.visible = true
                        tooltipSN_N.show(qsTr("Value must be between 0 and 65535"))
                        return;
                    }
                    var fSN_Y        = parseFloat( inputSN_Y.text );
                    var fSN_YRounded = parseFloat(parseInt(fSN_Y));
                    var fSN_N        = parseFloat( inputSN_N.text );
                    var fSN_NRounded = parseFloat(parseInt(fSN_N));
                    // Vérification fonctionnement avec un entier
                    if( fSN_Y !== fSN_YRounded )
                    {
                        // On désactive l'état occupé
                        //calibrationID.bBusyState  = false
                        busyConfigSettings.visible  = false
                        errorConfigSettings.visible = true
                        tooltipSN_Y.show(qsTr("Value must be an integer"))
                        return;
                    }
                    if( fSN_N !== fSN_NRounded )
                    {
                        // On désactive l'état occupé
                        //calibrationID.bBusyState  = false
                        busyConfigSettings.visible  = false
                        errorConfigSettings.visible = true
                        tooltipSN_N.show(qsTr("Value must be an integer"))
                        return;
                    }

                    //calibrationPart1.uiFGetCurrentChannelID()
                    /*var*/ updatePartID.uiChannelID = calibrationPart1.uiFGetCurrentChannelID()
                    // Si c'est un capteur CT
                    if( bCTParam ){
                        // Sauvegarde
                        tFactoryEvent.saveConfigSettingsCT( uiSN_Y,
                                                            uiSN_N,
                                                            parseFloat(inputCTCompensationA1.text),
                                                            parseFloat(inputCTCompensationA2.text),
                                                            parseFloat(inputCTCompensationA3.text),
                                                            parseFloat(inputCTCompensationRaw20.text) );
                    }
                    // Si c'est un Tbd / Chla_nke / PhycoC_nke / PhycoE_nke / CDOM_nke / Trypto
                    else if( bEvosensParam ){
                        var fIntegrationTime, fRange, fAverage;
                        // Vérification intégration time
                        fIntegrationTime = parseFloat( inputIntegrationTime.text)
                        // Vérification range
                        fRange = parseFloat( inputRange.text)
                        // Vérification average
                        fAverage = parseFloat( inputAverageSample.text)

                        var fIntegrationTimeRounded = parseFloat(parseInt(fIntegrationTime));
                        var fRangeTimeRounded       = parseFloat(parseInt(fRange));
                        var fAverageRounded         = parseFloat(parseInt(fAverage));
                        // Vérification fonctionnement avec un entier
                        if( fIntegrationTime !== fIntegrationTimeRounded )
                        {
                            // On désactive l'état occupé
                            busyConfigSettings.visible  = false
                            errorConfigSettings.visible = true
                            inputIntegrationTime.show(qsTr("Value must be an integer"))
                            return;
                        }
                        if( fRange !== fRangeTimeRounded )
                        {
                            // On désactive l'état occupé
                            busyConfigSettings.visible  = false
                            errorConfigSettings.visible = true
                            tooltipRange.show(qsTr("Value must be an integer"))
                            return;
                        }
                        if( fAverage !== fAverageRounded )
                        {
                            // On désactive l'état occupé
                            busyConfigSettings.visible  = false
                            errorConfigSettings.visible = true
                            tooltipAverageSample.show(qsTr("Value must be an integer"))
                            return;
                        }
                        var uiIntegrationTimeRounded = parseInt(fIntegrationTime);
                        var uiRangeTimeRounded       = parseInt(fRange);
                        var uiAverageRounded         = parseInt(fAverage);

                        console.log( uiIntegrationTimeRounded )
                        console.log( uiRangeTimeRounded )
                        console.log( uiAverageRounded )

                        // Test d'intégrité
                        if(  ( uiIntegrationTimeRounded < 0 )
                          || ( uiIntegrationTimeRounded > 65535 ) )
                        {
                            // On désactive l'état occupé
                            busyConfigSettings.visible  = false
                            errorConfigSettings.visible = true
                            inputIntegrationTime.show(qsTr("Value must be between 0 and 65535"))
                            return;
                        }
                        // Test d'intégrité
                        if(  ( uiRangeTimeRounded < 0 )
                          || ( uiRangeTimeRounded > 255 ) )
                        {
                            // On désactive l'état occupé
                            busyConfigSettings.visible  = false
                            errorConfigSettings.visible = true
                            tooltipRange.show(qsTr("Value must be between 0 and 255"))
                            return;
                        }
                        // Test d'intégrité
                        if(  ( uiAverageRounded < 0 )
                          || ( uiAverageRounded > 65535 ) )
                        {
                            // On désactive l'état occupé
                            busyConfigSettings.visible  = false
                            errorConfigSettings.visible = true
                            tooltipAverageSample.show(qsTr("Value must be between 0 and 65535"))
                            return;
                        }

                        // Sauvegarde
                        tFactoryEvent.sFSaveConfigSettingsEvosens( uiSN_Y,
                                                                   uiSN_N,
                                                                   fIntegrationTime,
                                                                   fRange,
                                                                   fAverage );
                    }
                    // Tout sauf evosens
                    else{
                        // Entrée des numéros de série
                        tFactoryEvent.saveConfigSettings( uiSN_Y, uiSN_N );
                    }
                }
            }
            //--------------------
            // Busy : calibration settings
            //--------------------
            BusyIndicatorPerso {
                id: busyConfigSettings
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
                id: errorConfigSettings
                width: 31
                height: 31
                source: "qrc:/Logo/Global/No_Cross.png"
                anchors.left: btnConfirmSettings.right
                anchors.leftMargin: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                visible: false
            }

        } // Fin bloc refresh


        //-----------------------------
        // Update sensor - File open
        //-----------------------------
        FileDialog {
            id: fileDialog
            title: qsTr("Open update file")
            //nameFilters: [ "Binary files (*.bin *.hex)" ]
            //selectedNameFilter: "Binary files (*.bin *.hex)"

            onAccepted: {
                // Mise à jour
                bUpdate = true;
                // Disparition du bouton
                // Apparition du pourcentage et load bar
                updatePartID.m_bUpdateBusyRefresh = true
                updatePartID.m_bUpdateBtnRefreshVisible = false
                // Protection contre click
                vFLockUserClick();

                console.log( "url : "+fileUrl )
                var theurl = fileUrl.toString().replace("file:///","");
                //theurl.toString().replace("file:///","")
                console.log("theurl : "+theurl)
                tFactoryEvent.startUpdate( theurl );
            }
        }

        //-------------------------------
        // Update interface - File open
        //-------------------------------
        FileDialog {
            id: fileDialogInterface
            //property MyClass myObj
            title: qsTr("Open update file")
            //nameFilters: [ "Binary files (*.bin *.hex)" ]
            //selectedNameFilter: "Binary files (*.bin *.hex)"

            onAccepted: {
                // Conversion du nom de fichier avec chemin
                console.log( "url : "+fileUrl )
                var theurl = fileUrl.toString().replace("file:///","");
                console.log("theurl : "+theurl)
                // Vérification du nom
                if( theurl.includes("SENSOR_INTERFACE") ){
                    // Protection contre click
                    vFLockUserClick();
                    //%%AP -  Lancement de la mise à jour manuel
                    tInterfaceUpdateControler.startManualUpdate( theurl );
                }
                else{
                    messageDialogInterfaceUpdateInvalidFileError.open();
                }
            }
        }

        //-----------------------------
        // Write config - File open
        //-----------------------------
        FileDialog {
            id: fileDialogWriteConfig
            title: qsTr("Open config file")
            onAccepted: {
                // Mise à jour
                //bUpdate = true;
                // Disparition du bouton
                //btnWriteConfigOpenShadow.visible = false
                //btnWriteConfigOpenRect.visible   = false
                // Apparition du busy à la place du bouton
                busyWriteConfig.visible   = true
                buttonWriteConfig.visible = false
                // Protection contre click
                vFLockUserClick();
                console.log( "url : "+fileUrl )
                var theurl = fileUrl.toString().replace("file:///","");
                console.log("theurl : "+theurl)
                tFactoryEvent.startWriteConfig( theurl );
            }
        }

        //! [messagedialog]
        MessageDialogPerso {
            id: messageDialog
            title: qsTr("Error")
            text:  qsTr("An error occured")
        }
        // MessageDialog - Save config success
        MessageDialogPerso {
            id: messageDialogSaveConfig
            title: qsTr("Save config")
            text:  qsTr("Save config success")
        }
        // MessageDialog - Update interface success
        MessageDialogPerso {
            id: messageDialogUpdateInterface
            title: qsTr("Update interface")
            text:  qsTr("Update interface success")
        }
        // MessageDialog - Update interface error
        MessageDialogPerso {
            id: messageDialogUpdateInterfaceError
            title: qsTr("Update interface")
            text:  qsTr("Update interface error")
        }
        // MessageDialog - Refresh interface version error
        MessageDialogPerso {
            id: messageDialogRefreshVersionError
            title: qsTr("Refresh interface version")
            text:  qsTr("Refresh interface version error")
        }
        // MessageDialog - Interface update invalid file
        MessageDialogPerso {
            id: messageDialogInterfaceUpdateInvalidFileError
            title: qsTr("Interface update error")
            text:  qsTr("Invalid file selected")
        }
    }
}
