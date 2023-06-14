import QtQuick 2.13
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
//import QtQml.Models 2.14

import QtQuick.Window 2.13
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4
//import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.1

//import QtQuick.Dialogs 1.3

import "Component"
import "Windows"

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Paramètre général
    property var m_tChannelList:     tDataSensor.tsChannelList
    // Voie courante
    property int m_uiCurrentChannel: calibrationID.currentChannel
    // Activation de la sélection de voie (désactivé si O2 temperature 25)
    property bool m_bSelectSensorCalibration: true
    //------------------------------
    // Parameter information CH0
    //property var    m_tCh0ListCalibrationType: tDataSensorCh0.tsCalibrationList
    //------------------------------
    // Parameter information CH1
    //property var    m_tCh1ListCalibrationType: tDataSensorCh1.tsCalibrationList
    //------------------------------
    // Parameter information CH2
    //property var    m_tCh2ListCalibrationType: tDataSensorCh2.tsCalibrationList
    //------------------------------
    // Parameter information CH0
    property string m_sCh0ID:                  tDataSensorCh0.sID
    property string m_sCh0Unit:                tDataSensorCh0.sUnit
    property var    m_tCh0ListUnit:            tDataSensorCh0.tsUnitList
    property string m_sCh0CalibrationType:     tDataSensorCh0.sCalibrationType
    property var    m_tCh0ListCalibrationType: tDataSensorCh0.tsCalibrationList
    property string m_sCh0Average:             tDataSensorCh0.uiAverage
    property string m_sCh0Formula:             tDataSensorCh0.sFormula
    property string m_sCh0CalibrationDate:     tDataSensorCh0.sCalibrationDate
    property var    m_tCh0CalibPoint:          tCalibPointCh0
    //------------------------------
    // Parameter information CH1
    property string m_sCh1ID:                  tDataSensorCh1.sID
    property string m_sCh1Unit:                tDataSensorCh1.sUnit
    property var    m_tCh1ListUnit:            tDataSensorCh1.tsUnitList
    property string m_sCh1CalibrationType:     tDataSensorCh1.sCalibrationType
    property var    m_tCh1ListCalibrationType: tDataSensorCh1.tsCalibrationList
    property string m_sCh1Average:             tDataSensorCh1.uiAverage
    property string m_sCh1Formula:             tDataSensorCh1.sFormula
    property string m_sCh1CalibrationDate:     tDataSensorCh1.sCalibrationDate
    property var    m_tCh1CalibPoint:          tCalibPointCh1
    //------------------------------
    // Parameter information CH2
    property string m_sCh2ID:                  tDataSensorCh2.sID
    property string m_sCh2Unit:                tDataSensorCh2.sUnit
    property var    m_tCh2ListUnit:            tDataSensorCh2.tsUnitList
    property string m_sCh2CalibrationType:     tDataSensorCh2.sCalibrationType
    property var    m_tCh2ListCalibrationType: tDataSensorCh2.tsCalibrationList
    property string m_sCh2Average:             tDataSensorCh2.uiAverage
    property string m_sCh2Formula:             tDataSensorCh2.sFormula
    property string m_sCh2CalibrationDate:     tDataSensorCh2.sCalibrationDate
    property var    m_tCh2CalibPoint:          tCalibPointCh2

    property string sPdfCalibReportFilename: ""
    property int uiCalibReportTimeoutcpt: 0
    property int uiMaxCalibrationPointNumber: 0

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property int uiBlocWidthOffset: 60

    //=============================================================
    // Connection
    //=============================================================

    //=============================================================
    // Fonction
    //=============================================================

    //-------------------------------------------------
    // Vérification que le champs est un bien un nombre
    //-------------------------------------------------
    function bFIsAValidFloatNumber( sNumber ){
        var uiIndex = 0;

        console.log("== bFIsAValidFloatNumber ==");

        // Si chaîne de caractère nulle
        if( sNumber.length === 0 ){
            console.log("Test 0");
            console.log(uiIndex);
            console.log(sNumber[ uiIndex ]);
            return false;
        }
        // On boucle
        for( uiIndex = 0; uiIndex < sNumber.length; uiIndex++ ){
            // Vérification du premier caractère
            if(  ( uiIndex === 0   )
              && (  ( sNumber[ uiIndex ] === '.' )
                 || ( sNumber[ uiIndex ] === '0' ) )
              ){
              console.log("permier test");
              console.log(uiIndex);
              console.log(sNumber[ uiIndex ]);
              // Invalide
              return false;
            }
            // Si le caractère à l'index est bien un caractère autorisé
            if(  ( sNumber[ uiIndex ] !== '0' )
              && ( sNumber[ uiIndex ] !== '1' )
              && ( sNumber[ uiIndex ] !== '2' )
              && ( sNumber[ uiIndex ] !== '3' )
              && ( sNumber[ uiIndex ] !== '4' )
              && ( sNumber[ uiIndex ] !== '5' )
              && ( sNumber[ uiIndex ] !== '6' )
              && ( sNumber[ uiIndex ] !== '7' )
              && ( sNumber[ uiIndex ] !== '8' )
              && ( sNumber[ uiIndex ] !== '9' )
              && ( sNumber[ uiIndex ] !== '9' )
              && ( sNumber[ uiIndex ] !== '.' ) ){
               console.log("2ième test");
               console.log(uiIndex);
               console.log(sNumber[ uiIndex ]);
                // Invalide
                return false;
            }
            // Vérification du dernier caractère
            if(  ( uiIndex === ( sNumber.length - 1 ) )
              && (  ( sNumber[ uiIndex ] === '.' ) )
              ){
              console.log("3ième test");
              console.log(uiIndex);
              console.log(sNumber[ uiIndex ]);
              // Invalide
              return false;
            }
        }

        console.log("== bFIsAValidFloatNumber END ==");
        // Le nombre est valide
        return true;
    }

    //-------------------------------------------------
    // Vérification qu'une chaine de caractère est conforme
    //-------------------------------------------------
    function bFVerifchar2(evt){
    //	var key=evt.which?evt.which:evt.keyCode;
        var key=evt;
        var char2='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_=-+*.,;:!?<>()[]{}#$%&@"~^µ/\\\'\b';
    //	var char='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
        //if(char2.indexOf(String.fromCharCode(key))>=0)return true;
        if(char2.indexOf(key)>=0)return true;
        return false;
    }

    //-----------------------------------------------------
    // Retourne le char CPP par rapport au calibration type qml
    //-----------------------------------------------------
    function uiFGetCurrentCalibTypeChar( sCurrentCalibType ){
        var uiCalibTypeIndex;
        // Selon le type de calibration de la voie selectionnée
        switch( sCurrentCalibType ){
            // N : None
            case 'None':
                uiCalibTypeIndex = 78
            break;
            // M : Multipoint
            case 'Multipoint':
                uiCalibTypeIndex = 77
            break;
            // C : Corrected
            case 'Corrected':
                uiCalibTypeIndex = 77
            break;
            // S : Steinhart
            case 'Steinhart':
                uiCalibTypeIndex = 83
            break;
            // B : Bennet
            case 'Bennet':
                uiCalibTypeIndex = 66
            break;
            // L : Linear
            case 'Linear':
                uiCalibTypeIndex = 76
            break;
            // 2 : 2nd degree polynomial
            case '2nd degree polynomial':
                uiCalibTypeIndex = 50
            break;
            // 3 : 3rd degree polynomial
            case '3rd degree polynomial':
                uiCalibTypeIndex = 51
            break;
            default:
                uiCalibTypeIndex = 0
        }
        return uiCalibTypeIndex;
    }
    //-----------------------------------------------------
    // Retourne si le calibration type et le nombre de points sont compatibles
    //-----------------------------------------------------
    function bFIsCalibrationTypeValid( sCurrentCalibType, uiCalibPointNb ){
        var uiCalibTypeIndex;
        var uiNeededPointNumber = 0;
        var sCalibrationType;
        // Selon le type de calibration de la voie selectionnée
        switch( sCurrentCalibType ){
            // N : None
            case 'None':
                uiNeededPointNumber = 0;
                sCalibrationType = "None"
            break;
            // M : Multipoint
            case 'Multipoint':
                uiNeededPointNumber = 2;
                sCalibrationType = "Multipoint"
            break;
            // M : Corrected
            case 'Corrected':
                uiNeededPointNumber = 2;
                sCalibrationType = "Corrected"
            break;
            // S : Steinhart
            case 'Steinhart':
                uiNeededPointNumber = 3;
                sCalibrationType = "Steinhart"
            break;
            // B : Bennet
            case 'Bennet':
                uiNeededPointNumber = 4;
                sCalibrationType = "Bennet"
            break;
            // L : Linear
            case 'Linear':
                uiNeededPointNumber = 2;
                sCalibrationType = "Linear"
            break;
            // 2 : 2nd degree polynomial
            case '2nd degree polynomial':
                uiNeededPointNumber = 3;
                sCalibrationType = "2nd degree polynomial"
            break;
            // 3 : 3rd degree polynomial
            case '3rd degree polynomial':
                uiNeededPointNumber = 4;
                sCalibrationType = "3rd degree polynomial"
            break;
            default:
                return false;
        }

        // Si nombre de points de calibration insuffisants
        if( uiCalibPointNb < uiNeededPointNumber ){
            // On indique l'erreur
            tooltipCalibType.visible = true
            tooltipCalibType.text = sCalibrationType+" need "+uiNeededPointNumber+" calibration points";
            // On quitte
            return false;
        }
        // Valide
        return true;
    }

    //-----------------------------------------------------
    // Ajout du point de calibration
    //-----------------------------------------------------
    function vFAddCalibrationPoint( uiCurrentChannel, fPhy, fRaw ){
        calibrationID.addCalibPoint( uiCurrentChannel, fPhy, fRaw );
        calibrationID.currentChannel = uiCurrentChannel
    }
    //-----------------------------------------------------
    // Mise à jour du datetime
    //-----------------------------------------------------
    function vFSaveCurrentCalibrationDateTime( uiCurrentChannel ){
        var sDateTime;
        sDateTime = calibrationID.sSaveCurrentCalibrationDateTime( uiCurrentChannel );
        //if( calibrationID.currentChannel == uiCurrentChannel )
        //    inputDateTime.text = sDateTime;
        // Selon la voie
        switch( uiCurrentChannel ){
            // Voie 0
            case 0:
                calibrationID.ch0DateTime = sDateTime
            break;
            // Voie 1
            case 1:
                calibrationID.ch1DateTime = sDateTime
            break;
            // Voie 2
            case 2:
                calibrationID.ch2DateTime = sDateTime
            break;
            default:
            break;
        }
    }
    //-----------------------------------------------------
    // Sauvegarde de la configuration
    //-----------------------------------------------------
    function vFSaveConfiguration( uiCurrentChannel ){
        tMainEvent.saveChannelConfiguration( uiCurrentChannel );
    }
    //-----------------------------------------------------
    // Nombre de point de calibration pour les points de la voie courante
    //-----------------------------------------------------
    function uiFGetCalibrationPointNumber(){
        // Selon la voie sélectionnée
        switch( calibrationPartID.m_uiCurrentChannel ){
            case 0:
            return calibrationPartID.m_tCh0CalibPoint.rowCount();
            case 1:
            return calibrationPartID.m_tCh1CalibPoint.rowCount();
            case 2:
            return calibrationPartID.m_tCh2CalibPoint.rowCount();
            default:
            return 0;
        }
    }
    //-----------------------------------------------------
    // Vérification que le point raw existe déjà
    //-----------------------------------------------------
    function vFSetCurrentChannel( uiCurrentChannel ){
        calibrationID.currentChannel = uiCurrentChannel
    }
    //-----------------------------------------------------
    // Vérification que le point raw existe déjà
    //-----------------------------------------------------
    function bFIsRawPointAlreadyExist( fRawTest, uiIndex ){
        // Selon la voie
        switch( uiIndex ){
            case(0):
            return( calibrationPointCh0.bFIsRawPointAlreadyExist(fRawTest) )
            case(1):
            return( calibrationPointCh1.bFIsRawPointAlreadyExist(fRawTest) )
            case(2):
            return( calibrationPointCh2.bFIsRawPointAlreadyExist(fRawTest) )
        }
    }
    //-----------------------------------------------------
    // Vérification que le point raw existe déjà
    //-----------------------------------------------------
    function bFIsPhyPointAlreadyExist( fPhyTest, uiIndex  ){
        // Selon la voie
        switch( uiIndex ){
            case(0):
            return( calibrationPointCh0.bFIsPhyPointAlreadyExist(fPhyTest) )
            case(1):
            return( calibrationPointCh1.bFIsPhyPointAlreadyExist(fPhyTest) )
            case(2):
            return( calibrationPointCh2.bFIsPhyPointAlreadyExist(fPhyTest) )
        }
    }
    //-----------------------------------------------------
    // Vérification que le point raw existe déjà
    //-----------------------------------------------------
    function uiFGetCurrentChannelID(){
        return( calibrationID.iCurrentChannelID );
    }

    //=============================================
    // Déclaration des variables
    //=============================================

    // Pour mise à jour select sensor
    property bool bSensorSelectUpdate: false
    property string m_sCurrentCalibType: '78'
    property alias m_H2ItemID: m_H2ItemID

    //%%AP - 2021.11.19 - Modification portée variable
    Connections {
        target: calibrationID

        function onBusyStateChanged() {
            console.log("==onBusyStateChanged==");
            console.log("calibrationID.bBusyState = "+calibrationID.bBusyState)
            // Busy on
            if( calibrationID.bBusyState ){
                // On va bloquer le click
                mouseAreaWindowBlockClick.visible = true
                // On désactive l'état d'erreur si présent
                errorCalibSettings.visible = false
                calibrationID.bErrorState  = false
            }
            // Busy off
            else{
                // Communication avec liveView
                liveViewConfigID_MAIN.vFEndOfBusyState()
                // On va débloquer le click
                mouseAreaWindowBlockClick.visible = false
                // On fait disparaitre les busy
                busyCalibSettings.visible  = false
                //busyRefreshFormula.visible = false
                // Element montré
                //btnFormulaRefresh.visible  = true
                calibrationPointCh0.vFEndOfBusy()
                calibrationPointCh1.vFEndOfBusy()
                calibrationPointCh2.vFEndOfBusy()
            }
        }
        function onErrorStateChanged() {
            console.log("==onErrorStateChanged==");
            console.log("calibrationID.bErrorState = "+calibrationID.bErrorState)
            // Error On
            if( calibrationID.bErrorState ){
                // Désactivation de l'état busy
                calibrationID.bBusyState  = false
            }
            // Error off
            else{
                // On fait disparaitre les croix d'erreur
                errorCalibSettings.visible = false
                calibrationPointCh0.vFEndOfError()
                calibrationPointCh1.vFEndOfError()
                calibrationPointCh2.vFEndOfError()
            }
        }
        function onCurrentChannelChanged() {
            console.log("== currentChannelChanged: "+calibrationPartID.m_uiCurrentChannel +" ==")
            // Mise à jour du select sensor
            //selectSensorCalibration.currentIndex = calibrationPartID.m_uiCurrentChannel;
            var m_sUnit = "";
            // Selon la voie utilisée
            switch( calibrationPartID.m_uiCurrentChannel ){
                // Voie 0
                case 0:
                    console.log("ch0Average : "+calibrationID.ch0Average);
                    // Average
                    inputAverage.text  = calibrationID.ch0Average;
                    // Unit
                    m_sUnit = calibrationID.ch0Unit;
                    // Bloc info
                    sectionInfoBlocCh0.visible = true
                    // Identifiant capteur
                    calibrationID.iCurrentChannelID  = calibrationID.ch0ID;
                    // Calibration type
                    m_sCurrentCalibType  = calibrationID.ch0Type;
                    // CT - Conductivité
                    if( calibrationID.iCurrentChannelID == 4 ){
                        // Calibration théorique
                        inputTheoCalibY1.text = calibrationID.calibTheoPhy1
                        inputTheoCalibX1.text = calibrationID.calibTheoRaw1
                        inputTheoCalibY2.text = calibrationID.calibTheoPhy2
                        inputTheoCalibX2.text = calibrationID.calibTheoRaw2

                        var theoY1 = calibrationID.calibTheoPhy1
                        var theoX1 = calibrationID.calibTheoRaw1
                        var theoY2 = calibrationID.calibTheoPhy2
                        var theoX2 = calibrationID.calibTheoRaw2
                        // Si pas de division par zéro
                        if( (theoX2-theoX1)!=0 ){
                            var theoA = (theoY2-theoY1)/(theoX2-theoX1)
                            var theoB = (theoY2-theoA*theoX2)
                            txtTheoFormula2.text = "Y = "+theoA+"x + "+theoB
                        }
                    }
                break;
                // Voie 1
                case 1:
                    console.log("ch1Average : "+calibrationID.ch1Average);
                    // Average
                    inputAverage.text  = calibrationID.ch1Average
                    // Unit
                    //inputUnit.text = calibrationID.ch1Unit
                    m_sUnit = calibrationID.ch1Unit
                    // Average
                    //inputFormula.text  = calibrationID.ch1Formula
                    // DateTime
                    //inputDateTime.text = calibrationID.ch1DateTime
                    // Identifiant capteur
                    calibrationID.iCurrentChannelID  = calibrationID.ch1ID
                    // Calibration type
                    m_sCurrentCalibType  = calibrationID.ch1Type
                break;
                // Voie 2
                case 2:
                    console.log("ch2Average : "+calibrationID.ch2Average);
                    // Average
                    inputAverage.text  = calibrationID.ch2Average
                    // Unit
                    //inputUnit.text = calibrationID.ch2Unit
                    m_sUnit = calibrationID.ch2Unit
                    // Average
                    //inputFormula.text  = calibrationID.ch2Formula
                    // DateTime
                    //inputDateTime.text = calibrationID.ch2DateTime
                    // Identifiant capteur
                    calibrationID.iCurrentChannelID  = calibrationID.ch2ID
                    // Calibration type
                    m_sCurrentCalibType  = calibrationID.ch2Type
                break;
                default:
                 console.log("Bad channel error");
            }
            console.log("calibrationID.iCurrentChannelID = "+calibrationID.iCurrentChannelID);
            // Si S03 - Température CT
            if( calibrationID.iCurrentChannelID == 3 ){
                selectCalibType.model = listCalibTypeS03;
            }
            else if(  ( calibrationID.iCurrentChannelID == 6 )
                   || ( calibrationID.iCurrentChannelID == 7 )
                   || ( calibrationID.iCurrentChannelID == 25 ) ){
                selectCalibType.model = listCalibTypeO2;
            }
            else{
                selectCalibType.model = listCalibTypeGeneral;
            }

            // Liste unité
            switch( calibrationID.iCurrentChannelID )
             {
              case 3: inputUnit.model = listUnitS03; break;
              case 4: inputUnit.model = listUnitS04; break;
              case 5: inputUnit.model = listUnitS05; break;
              case 6: inputUnit.model = listUnitS06; break;
              case 7: inputUnit.model = listUnitS07; break;
              case 8: inputUnit.model = listUnitS08; break;
              case 9: inputUnit.model = listUnitS09; break;
              case 10: inputUnit.model = listUnitS10; break;
              case 11: inputUnit.model = listUnitS11; break;
              case 12: inputUnit.model = listUnitS12; break;
              case 13: inputUnit.model = listUnitS13; break;
              case 14: inputUnit.model = listUnitS14; break;
              case 15: inputUnit.model = listUnitS15; break;
              case 16: inputUnit.model = listUnitS16; break;
              case 17: inputUnit.model = listUnitS17; break;
              case 18: inputUnit.model = listUnitS18; break;
              case 19: inputUnit.model = listUnitS19; break;
              case 20: inputUnit.model = listUnitS20; break;
              case 21: inputUnit.model = listUnitS21; break;
              case 22: inputUnit.model = listUnitS22; break;
              case 23: inputUnit.model = listUnitS23; break;
              case 24: inputUnit.model = listUnitS24; break;
              case 25: inputUnit.model = listUnitS25; break;
              case 26: inputUnit.model = listUnitS26; break;
              case 27: inputUnit.model = listUnitS27; break;
              default: inputUnit.model = listUnitDefault;
             }

            // Test si unité OK
            var iIndexUnit = inputUnit.find( m_sUnit );
            if( iIndexUnit != -1 ){ inputUnit.currentIndex = iIndexUnit; }

            //console.log( "Index : " + selectCalibType.find("Multipoint") );
            //selectCalibType.currentIndex = 1;

            console.log("selectCalibType.currentIndex = "+selectCalibType.currentIndex);
            console.log("m_sCurrentCalibType = "+calibrationPartID.m_sCurrentCalibType );
            console.log("=> ADD selectCalibType.indexOfValue : " +selectCalibType.indexOfValue(m_sCurrentCalibType));
            // Index courant
            selectCalibType.currentIndex = selectCalibType.indexOfValue(m_sCurrentCalibType);
            console.log("selectCalibType.currentIndex = "+selectCalibType.currentIndex);

            // Par défaut
            rowCalibTemperature.visible  = false;
            calibTemperatureLine.visible = false;
            rowTheoCalib.visible         = false;
            theoCalibLine.visible        = false;
            rowTheoFormula.visible       = false;
            theoFormulaLine.visible      = false;
            // Le paramètre average doit être caché pour certains capteurs
            if(  ( calibrationID.iCurrentChannelID ==  5 ) // Tbd nke
              || ( calibrationID.iCurrentChannelID == 13 ) // Chla nke
              || ( calibrationID.iCurrentChannelID == 15 ) // Fluo-PC
              || ( calibrationID.iCurrentChannelID == 16 ) // Fluo-PE
              || ( calibrationID.iCurrentChannelID == 17 ) // CDOM
              || ( calibrationID.iCurrentChannelID == 18 ) // Fluo-TP
              ){
                calibSettings.height = 160;
                rowAverage.visible = false;
                averageLine.visible = false;
                rowCalibTemperature.visible  = false;
                calibTemperatureLine.visible = false;
                calibScrollView1.contentHeight = 950;
            }
            else{

                calibSettings.height = 210;
                calibScrollView1.contentHeight = 950;
                // Si pH ou ISE
                if(  ( calibrationID.iCurrentChannelID == 8  )
                  || ( calibrationID.iCurrentChannelID == 20 )
                  || ( calibrationID.iCurrentChannelID == 21 )
                  || ( calibrationID.iCurrentChannelID == 22 )
                  || ( calibrationID.iCurrentChannelID == 23 )
                  || ( calibrationID.iCurrentChannelID == 24 ) ){
                    calibSettings.height = 250;
                    rowCalibTemperature.visible  = true;
                    calibTemperatureLine.visible = true;
                    calibScrollView1.contentHeight = 990;
                    inputCalibTemperature.text = tMainEvent.getPHCalibrationTemperature().toFixed(2).toString();
                }
                // Si CT
                if( calibrationID.iCurrentChannelID == 4 && ( tMainData.bNkeMode || tMainData.bMetroMode ) ){
                //if( calibrationID.iCurrentChannelID == 4 && ( tMainData.bNkeMode ) ){
                    calibSettings.height         = 290;
                    rowTheoCalib.visible         = true;
                    theoCalibLine.visible        = true;
                    rowTheoFormula.visible       = true;
                    theoFormulaLine.visible      = true;
                    calibScrollView1.contentHeight = 1050;
                    inputCalibTemperature.text = tMainEvent.getPHCalibrationTemperature().toFixed(2).toString();
                }
                rowAverage.visible = true;
                averageLine.visible = true;
            }
            // Capteur O2 ?
            if(  ( calibrationID.iCurrentChannelID == 6  )    // Oxygène concentration
              || ( calibrationID.iCurrentChannelID == 7  )    // Oxygène saturation
              || ( calibrationID.iCurrentChannelID == 25 ) ){ // Oxygène temperature
                // Limitation sur 2 points de calibration
                uiMaxCalibrationPointNumber = 2;
            }
            else{
                // Limitation sur 14 points de calibration
                uiMaxCalibrationPointNumber = 14;
            }
            console.log("== currentChannelChanged END ==");
        }
    }
    function vFDisplayCalibrationNav(){
        // Si voie oxygène température
        if(  ( calibrationID.ch2ID      == 25 )
          && ( liveviewID.channelNumber == 3  ) ){
            //selectSensorCalibration.currentIndex = 2
            calibrationID.currentChannel = 2
            calibrationPartID.m_bSelectSensorCalibration = false
            calibrationID.currentChannelChanged()
        }
        else{
            calibrationPartID.m_bSelectSensorCalibration = true
        }
    }

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // QML
    //==============================================================
    id: calibrationPartID
    implicitWidth: 662
    implicitHeight: 888
    //anchors.left: parent.left
    //anchors.right: parent.right

    /*
    // Background color
    Rectangle {
        anchors.fill: parent
        color: "#efeef4"
        z: -9
    }
    */


    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#00000000"

        //-------------------------------
        // Titre H2
        //-------------------------------
        H2Item{
            id: m_H2ItemID
            sH2Title: "Calibration"
            sH2Img: "qrc:/Logo/Calibration/calib-title-low.png"
            uiLogoWidth: 34
            anchors.top: parent.top
            anchors.topMargin: 20
        }

        //-------------------------------
        // Bloc Calibration selection
        //-------------------------------
        CalibrationSelectionBloc{
            id:m_calibrationSelectionBlocID
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.leftMargin: uiBlocWidthOffset
            anchors.rightMargin: uiBlocWidthOffset
            anchors.top: m_H2ItemID.bottom

            // Parameter
            m_tChannelList: calibrationPartID.m_tChannelList
            m_uiCurrentChannel: calibrationPartID.m_uiCurrentChannel
            m_bSelectSensorCalibration : calibrationPartID.m_bSelectSensorCalibration

            onCurrentChannelChanged: {
                console.log("uiIndex: "+uiIndex)

                console.log("==ComboBox calibrationPartID.m_uiCurrentChannel onActivated==");
                //console.log("index: "+index);
                console.log("currentIndex: "+uiIndex);

                // Vérification que la voie existe
                if( calibrationID.channelNumber <= uiIndex ){
                    calibrationID.currentChannel = uiIndex;
                }
                // La voie sélectionnée n'existe pas
                else{
                    // On force sur la voie 0
                    currentIndex = 0;
                    calibrationID.currentChannel = 0;
                }
                // Mise à jour des points
                calibrationID.updateCalibPoint();
            }
        }


        //-----------------------------------------------
        // Deuxième bloc : calibration settings
        //-----------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: calibSettings
            samples: 30
            horizontalOffset: 0
            source: calibSettings
        }
        // Calibration settings
        Rectangle {
            id: calibSettings
            //x: 25
            //y: 100
            width: calibrationPartID.width - 120
            height: 200
            color: "#ffffff"
            anchors.top: m_calibrationSelectionBlocID.bottom
            anchors.topMargin: 28
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: rectTitleCalibSettings
                x: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: element4
                    x: 63
                    y: 14
                    text: qsTr("Calibration settings")
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: false
                    font.pixelSize: 13
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: openSansDemiBoldFont.name
                    //font.family: "Open Sans"
                    //font.weight: Font.DemiBold
                }
                anchors.topMargin: 0
                clip: false
                anchors.top: parent.top
                transformOrigin: Item.Center
            }

            //------------------------------------------------
            // Calibration settings
            //------------------------------------------------
            ColumnLayout {
                id: columnLayoutCalibSettings
                //x: 0
                //y: 42
                width: parent.width
                height: parent.height - 34 - 28 - 16 - 5
                anchors.top: rectTitleCalibSettings.bottom
                anchors.topMargin: 5

                //------------------------------
                // Unit
                //------------------------------
                RowLayout {
                    id: rowUnit
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowUnit2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgUnit
                            source: "qrc:/Logo/Calibration/calib-unit-2.png"
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtUnit
                            text: qsTr("Unit")
                            height: parent.height
                            //anchors.left: imgUnit.right
                            //anchors.leftMargin: 10
                            anchors.left: rowUnit2.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    //----------------------------
                    // Default
                    ListModel { id: listUnitDefault; ListElement { text:"--" } }
                    // CT - Température
                    ListModel { id: listUnitS03; ListElement { text:"degC" } }
                    // CT- Conductivité
                    ListModel { id: listUnitS04; ListElement { text:"mS/cm" } }
                    // Turbidité
                    ListModel { id: listUnitS05; ListElement { text:"NTU" } ListElement { text:"FNU" } }
                    // O2 - Concentration
                    ListModel { id: listUnitS06; ListElement { text:"mg/L" } }
                    // O2 - Saturation
                    ListModel { id: listUnitS07; ListElement { text:"%" } }
                    // pH
                    ListModel { id: listUnitS08; ListElement { text:"pH" } }
                    // Turner - Chla
                    ListModel { id: listUnitS09; ListElement { text:"ppb" } }
                    // Turner - PC
                    ListModel { id: listUnitS10; ListElement { text:"ppb" } }
                    // Turner - PE
                    ListModel { id: listUnitS11; ListElement { text:"ppb" } }
                    // Turner - CDOM
                    ListModel { id: listUnitS12; ListElement { text:"ppb" } }
                    // nke - Chla
                    ListModel { id: listUnitS13; ListElement { text:"ppb" } }
                    // PAR
                    ListModel { id: listUnitS14; ListElement { text:"umol/m2/s" } }
                    // nke - PC
                    ListModel { id: listUnitS15; ListElement { text:"ppb" } }
                    // nke - PE
                    ListModel { id: listUnitS16; ListElement { text:"ppb" } }
                    // nke - CDOM
                    ListModel { id: listUnitS17; ListElement { text:"ppb" } }
                    // nke - TRP
                    ListModel { id: listUnitS18; ListElement { text:"ppb" } }
                    // Turner - TRP
                    ListModel { id: listUnitS19; ListElement { text:"ppb" } }
                    // S20 - Redox
                    ListModel { id: listUnitS20; ListElement { text:"mV" } }
                    // S21 - Ammonium
                    ListModel { id: listUnitS21; ListElement { text:"ppm" } }
                    // S22 - Potassium
                    ListModel { id: listUnitS22; ListElement { text:"ppm" } }
                    // S23 - Nitrate
                    ListModel { id: listUnitS23; ListElement { text:"ppm" } }
                    // S24 - ISE
                    ListModel { id: listUnitS24; ListElement { text:"ppm" } }
                    // S25 - Oxygen temperature
                    ListModel { id: listUnitS25; ListElement { text:"degC" } }
                    // S26 - Hydrocarbon
                    ListModel { id: listUnitS26; ListElement { text:"ppb" } }
                    // S27 - Crude Oil
                    ListModel { id: listUnitS27; ListElement { text:"ppb" } }

                    ComboBox {
                        id: inputUnit
                        width: 200

                        // Si activation
                        enabled: !bNkeProdMode
                        model: listUnitDefault
                        height: 200
                        //color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        // Tooltip calib type
                        ToolTip {
                            id: tooltipUnitCalib
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
                        id: rectangleUnitDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        Text {
                            id: elementUnit
                            x: 178
                            y: 11
                            text: qsTr("Invalid value")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 12
                            visible: false;
                        }
                    }
                } // RowLayout Average

                //--- Ligne ---
                Rectangle {
                    id: unitLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //------------------------------
                // Calibration type
                //------------------------------
                RowLayout {
                    id: rowCalibType
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowCalibType2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgCalibType
                            //width: parent.height
                            height: 20
                            width: 30
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Calibration/calib-type.png"
                        }

                        Text {
                            id: txtCalibType
                            text: qsTr("Calibration type")
                            height: parent.height
                            //anchors.left: imgCalibType.right
                            //anchors.leftMargin: 10
                            anchors.left: rowCalibType2.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    ListModel {
                        id: listCalibTypeGeneral
                        ListElement { text:"None";                  value:"N" }
                        ListElement { text:"Multipoint";            value:"M" }
                        ListElement { text:"Linear";                value:"L" }
                        ListElement { text:"2nd degree polynomial"; value:"2" }
                        ListElement { text:"3rd degree polynomial"; value:"3" }
                    }
                    ListModel {
                        id: listCalibTypeO2
                        ListElement { text:"None";      value:"N" }
                        ListElement { text:"Corrected"; value:"M" }
                    }
                    ListModel {
                        id: listCalibTypeS03
                        ListElement { text:"None";                  value:"N" }
                        ListElement { text:"Multipoint";            value:"M" }
                        ListElement { text:"Linear";                value:"L" }
                        ListElement { text:"Steinhart";             value:"S" }
                        ListElement { text:"Bennet";                value:"B" }
                        ListElement { text:"2nd degree polynomial"; value:"2" }
                        ListElement { text:"3rd degree polynomial"; value:"3" }
                    }
                    ComboBox {
                        id: selectCalibType
                        width: 200
                        textRole: "text"
                        valueRole: "value"

                        // Si activation
                        enabled: !bNkeProdMode
                        model: listCalibTypeGeneral
                        height: 200
                        //color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        // Tooltip calib type
                        ToolTip {
                            id: tooltipCalibType
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
                        id: rectangleCalibTypeDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                    }
                }

                //--- Ligne ---
                Rectangle {
                    id: calibTypeLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //------------------------------
                // Average
                //------------------------------
                RowLayout {
                    id: rowAverage
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowAverage2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgAverage
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16

                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtAverage
                            text: qsTr("Average")
                            height: parent.height
                            anchors.left: rowAverage2.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    TextField {
                        id: inputAverage
                        text: qsTr("")
                        height: 30
                        activeFocusOnPress: true
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        selectByMouse: true
                        // Si activation
                        enabled: !bNkeProdMode

                        ToolTip {
                            id: tooltipAverageCalib
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
                        id: rectangleAverageDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        Text {
                            id: element
                            x: 178
                            y: 11
                            text: qsTr("Invalid value")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 12
                            visible: false;
                        }
                    }
                } // RowLayout Average

                //--- Ligne ---
                Rectangle {
                    id: averageLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }


                //------------------------------
                // Calibration Théorique
                //------------------------------
                RowLayout {
                    id: rowTheoCalib
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowTheoCalib2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgTheoCalib
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtTheoCalib
                            text: qsTr("Calibration theo")
                            height: parent.height
                            anchors.left: imgTheoCalib.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleTheoCalib4
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            id: txtTheoCalibY1
                            text: qsTr("Y1=")
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                        TextField {
                            id: inputTheoCalibY1
                            width: parent.width/3
                            anchors.left: txtTheoCalibY1.right
                            anchors.leftMargin: 5
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true
                        }
                        Text {
                            id: txtTheoCalibX1
                            text: qsTr("X1=")
                            anchors.left: inputTheoCalibY1.right
                            anchors.leftMargin: 5
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                        TextField {
                            id: inputTheoCalibX1
                            width: parent.width/3
                            anchors.left: txtTheoCalibX1.right
                            anchors.leftMargin: 5
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true
                        }
                    }
                    Rectangle {
                        id: rectangleTheoCalibDummy
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        Text {
                            id: txtTheoCalibY2
                            text: qsTr("Y2=")
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                        TextField {
                            id: inputTheoCalibY2
                            width: parent.width/3
                            anchors.left: txtTheoCalibY2.right
                            anchors.leftMargin: 5
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true
                        }
                        Text {
                            id: txtTheoCalibX2
                            text: qsTr("X2=")
                            anchors.left: inputTheoCalibY2.right
                            anchors.leftMargin: 5
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                        TextField {
                            id: inputTheoCalibX2
                            width: parent.width/3
                            anchors.left: txtTheoCalibX2.right
                            anchors.leftMargin: 5
                            text: qsTr("")
                            height: 30
                            activeFocusOnPress: true
                            selectByMouse: true
                        }
                    }
                } // RowLayout range
                //--- Ligne ---
                Rectangle {
                    id: theoCalibLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }
                //------------------------------
                // Calibration Théorique
                //------------------------------
                RowLayout {
                    id: rowTheoFormula
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowTheoFormula2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgTheoFormula
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtTheoFormula
                            text: qsTr("Calibration theo")
                            height: parent.height
                            anchors.left: imgTheoFormula.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleTheoFormula4
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            id: txtTheoFormula2
                            text: qsTr("Y=ax+b")
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    Rectangle {
                        id: rectangleTheoFormulaDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                    }
                } // RowLayout range
                //--- Ligne ---
                Rectangle {
                    id: theoFormulaLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //------------------------------
                // Calibration temperature
                //------------------------------
                RowLayout {
                    id: rowCalibTemperature
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowCalibTemperature2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgCalibTemperature
                            //source: "qrc:/Logo/Calibration/calib-average.png"
                            source: "qrc:/Logo/Calibration/calib-temperature.png"
                            width: 30
                            height: 22

                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtCalibTemperature
                            text: qsTr("Calibration temperature")
                            height: parent.height
                            //anchors.left: imgCalibTemperature.right
                            //anchors.leftMargin: 10
                            anchors.left: rowCalibTemperature2.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    TextField {
                        id: inputCalibTemperature
                        //width: 200
                        text: qsTr("")
                        height: 30
                        activeFocusOnPress: true
                        //color: "#ffffff"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        //%%AP - Test
                        //ToolTip.visible: right
                        //ToolTip.text: qsTr("Invalide value")
                        selectByMouse: true
                        // Si activation
                        enabled: !bNkeProdMode

                        ToolTip {
                            id: tooltipCalibTemperature
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
                        id: rectangleCalibTemperatureDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        Text {
                            id: elementCalibTemperature
                            x: 178
                            y: 11
                            text: qsTr("Invalid value")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 12
                            visible: false;
                        }
                    }
                } // RowLayout Average


                //--- Ligne ---
                Rectangle {
                    id: calibTemperatureLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

            } // Fin ColumnLayout

            Button {
                id: btnConfirmSettings
                width: 100
                height: 28
                text: qsTr("Submit")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                // Si activation
                enabled: !bNkeProdMode

                onClicked: {
                    // On désactive l'état d'erreur si présent
                    errorCalibSettings.visible = false
                    // On active l'état occupé
                    calibrationID.bBusyState = true
                    // On fait apparaître le busy
                    busyCalibSettings.visible = true
                    // On va bloquer le click
                    //mouseAreaWindowBlockClick.visible = true

                    // Index de la voie sélectionnée
                    console.log( "Current index: "+calibrationPartID.m_uiCurrentChannel );

                    //--------------
                    // Unit
                    //--------------
                    //console.log( "inputUnit: "+inputUnit.text )
                    console.log( "inputUnit.currentText : "+inputUnit.currentText  )
                    //var sUnit = String(inputUnit.text);
                    //var sUnit = ""+inputUnit.text.toString();
                    var sUnit = ""+inputUnit.currentText.toString();
                    // Test d'intégrité
                    if(  bFVerifchar2( sUnit )
                      && !isNaN(sUnit) ){
                        // On désactive l'état occupé
                        calibrationID.bBusyState   = false;
                        busyCalibSettings.visible  = false;
                        errorCalibSettings.visible = true;
                        tooltipUnitCalib.show(qsTr("Invalid character submit"));
                        return;
                    }
                    // Vérification longueur
                    if( sUnit.length > 15 ){
                        // On désactive l'état occupé
                        calibrationID.bBusyState   = false;
                        busyCalibSettings.visible  = false;
                        errorCalibSettings.visible = true;
                        tooltipUnitCalib.show(qsTr("Unit must be shorter than 15 characters"));
                        return;
                    }
                    //---------------------
                    // Calibration type
                    //---------------------
                    console.log("selectCalibTyp add current value: "+selectCalibType.currentValue )
                    console.log("selectCalibTyp: "+selectCalibType.currentText )
                    console.log("selectCalibTypChar: "+uiFGetCurrentCalibTypeChar(selectCalibType.currentText));
                    // Récupération du nombre de points de calibration
                    console.log( "tableView.rowCount: "+uiFGetCalibrationPointNumber() );


                    // Si nombre de points insuffisants pour le calibraiton type
                    if( !bFIsCalibrationTypeValid( selectCalibType.currentText, uiFGetCalibrationPointNumber() ) ){
                        // On désactive l'état occupé
                        calibrationID.bBusyState  = false
                        busyCalibSettings.visible = false
                        errorCalibSettings.visible = true
                        return;
                    }

                    //-----------------------------
                    // Average
                    //-----------------------------
                    var uiAverage;
                    if(  ( calibrationID.iCurrentChannelID ==  5 ) // Tbd nke
                      || ( calibrationID.iCurrentChannelID == 13 ) // Chla nke
                      || ( calibrationID.iCurrentChannelID == 15 ) // Fluo-PC
                      || ( calibrationID.iCurrentChannelID == 16 ) // Fluo-PE
                      || ( calibrationID.iCurrentChannelID == 17 ) // CDOM
                      || ( calibrationID.iCurrentChannelID == 18 ) // Fluo-TP
                      ){
                        // La valeur est forcée à 1
                        uiAverage = 1;
                    }
                    else{
                        console.log( "inputAverage: "+inputAverage.text )
                        uiAverage = parseInt( inputAverage.text );
                        // Test d'intégrité
                        if(  ( uiAverage == 0 )
                          || ( uiAverage > 1000 ) )
                         {
                            // On désactive l'état occupé
                            calibrationID.bBusyState  = false
                            busyCalibSettings.visible = false
                            errorCalibSettings.visible = true
                            tooltipAverageCalib.show(qsTr("Value must be between 1 and 1000"))
                            return;
                         }
                        var fAvg        = parseFloat( inputAverage.text );
                        var fAvgRounded = parseFloat(parseInt(fAvg));
                        // Vérification fonctionnement avec un entier
                        if( fAvg !== fAvgRounded )
                         {
                            // On désactive l'état occupé
                            calibrationID.bBusyState  = false
                            busyCalibSettings.visible = false
                            errorCalibSettings.visible = true
                            tooltipAverageCalib.show(qsTr("Value must be an integer"))
                            return;
                         }
                    }
                    //-----------------------------

                    //-----------------------------
                    // Calibration temperature
                    //-----------------------------
                    // Si pH ou ISE
                    if(  ( calibrationID.iCurrentChannelID == 8  )
                      || ( calibrationID.iCurrentChannelID == 20 )
                      || ( calibrationID.iCurrentChannelID == 21 )
                      || ( calibrationID.iCurrentChannelID == 22 )
                      || ( calibrationID.iCurrentChannelID == 23 )
                      || ( calibrationID.iCurrentChannelID == 24 ) ){
                        console.log( "Calibration temperature: "+inputCalibTemperature.text )
                        var fCalibTemperature = parseFloat( inputCalibTemperature.text );
                        var sCalibTemperature = inputCalibTemperature.text.toString();
                        // Test d'intégrité
                        if(  ( uiAverage == 0 )
                          || ( uiAverage > 50 ) ){
                            // On désactive l'état occupé
                            calibrationID.bBusyState   = false
                            busyCalibSettings.visible  = false
                            errorCalibSettings.visible = true
                            tooltipCalibTemperature.show(qsTr("Value must be between 0 and 50"))
                            return;
                        }
                        // Test si nombre décimal avec un point uniquement
                        if( !bFIsAValidFloatNumber(sCalibTemperature) ){
                            // On désactive l'état occupé
                            calibrationID.bBusyState   = false
                            busyCalibSettings.visible  = false
                            errorCalibSettings.visible = true
                            tooltipCalibTemperature.show(qsTr("Value is not a valid float number."))
                            return;
                        }
                        // Test si nombre décimal
                        if( sCalibTemperature.split('.').length >= 2 ){
                            // Vérification fonctionnement avec un entier
                            if(  ( sCalibTemperature.split('.').length > 2  )
                              || ( sCalibTemperature.split('.')[1].length <= 0 )
                              || ( sCalibTemperature.split('.')[1].length >  2 ) ){
                                // On désactive l'état occupé
                                calibrationID.bBusyState   = false
                                busyCalibSettings.visible  = false
                                errorCalibSettings.visible = true
                                tooltipCalibTemperature.show(qsTr("Please enter a value with a maximum of 2 decimal places"))
                                return;
                            }
                        }
                        // On assigne la valeur de calibration temperature
                        tMainEvent.setPHCalibrationTemperature( fCalibTemperature );
                    }
                    //-----------------------------
                    // Si on a un CT
                    if( calibrationID.iCurrentChannelID == 4 ){
                        calibrationID.submitCTCompensationFactor( parseFloat(inputTheoCalibY1.text),
                                                                  parseFloat(inputTheoCalibY2.text),
                                                                  parseFloat(inputTheoCalibX1.text),
                                                                  parseFloat(inputTheoCalibX2.text) )
                    }
                    else{
                        timerSubmitCalibrationConfig.uiState    = 2
                    }
                    // Récupération des paramètres à jour
                    calibrationID.submitCalibSettings(parseInt(calibrationPartID.m_uiCurrentChannel),
                                                      sUnit,
                                                      uiFGetCurrentCalibTypeChar(selectCalibType.currentText),
                                                      uiAverage );
                    // Mise à jour du datetime de calibration
                    //inputDateTime.text = calibrationID.sSaveCurrentCalibrationDateTime( calibrationID.currentChannel );
                    calibrationID.sSaveCurrentCalibrationDateTime( calibrationID.currentChannel );
                    // Sauvegarde de la configuration dans le capteur
                    tMainEvent.saveChannelConfiguration( calibrationID.currentChannel );
                    // Lancement du timer
                    timerSubmitCalibrationConfig.bWriteDone = false;
                    timerSubmitCalibrationConfig.bReadDone  = false;
                    timerSubmitCalibrationConfig.uiState    = 0;
                    timerSubmitCalibrationConfig.start();
                }
            }

            // Timer pour le fonctionnement du submit bouton remove point
            Timer {
                id: timerSubmitCalibrationConfig
                interval: 100;
                running: false;
                repeat: true;
                property bool bWriteDone: false
                property bool bReadDone: false
                property int  uiState: 0
                onTriggered: {
                    // Tant que le grafcet est en busy
                    if( tMainEvent.isModbusBusyState() ){
                        return;
                    }
                    // Test de l'état
                    switch( timerSubmitCalibrationConfig.uiState ){
                        //-------------------------
                        // Ecriture configuration général
                        //-------------------------
                        case 0:
                            console.log("tMainEvent.saveSensorConfiguration()")
                            tMainEvent.saveSensorConfiguration();
                            // Changement d'état
                            timerSubmitCalibrationConfig.uiState++;
                        return;
                        //-------------------------
                        // Lecture configuration general
                        //-------------------------
                        case 1:
                            console.log("tMainEvent.getSensorConfiguration()")
                            // Requête de passage en mode calibration
                            tMainEvent.getSensorConfiguration();
                            // Changement d'état
                            timerSubmitCalibrationConfig.uiState++;
                        return;
                        //-------------------------
                        // Ecriture configuration général
                        //-------------------------
                        case 2:
                            console.log("tMainEvent.saveSensorConfiguration()")
                            tMainEvent.saveChannelConfiguration( calibrationID.currentChannel );
                            // Changement d'état
                            timerSubmitCalibrationConfig.uiState++;
                        return;
                        //-------------------------
                        // Lecture configuration general
                        //-------------------------
                        case 3:
                            console.log("tMainEvent.getSensorConfiguration()")
                            // Requête de passage en mode calibration
                            tMainEvent.getChannelConfiguration( calibrationID.currentChannel );
                            // Changement d'état
                            timerSubmitCalibrationConfig.uiState++;
                        return;
                        //-------------------------
                        // Fin graph
                        //-------------------------
                        case 4:
                        break;
                    }
                    var theoY1 = calibrationID.calibTheoPhy1
                    var theoX1 = calibrationID.calibTheoRaw1
                    var theoY2 = calibrationID.calibTheoPhy2
                    var theoX2 = calibrationID.calibTheoRaw2
                    // Si différent de zéro
                    if( (theoX2-theoX1) != 0 ){
                        var theoA = (theoY2-theoY1)/(theoX2-theoX1)
                        var theoB = (theoY2-theoA*theoX2)
                        txtTheoFormula2.text = "Y = "+theoA+"x + "+theoB
                    }
                    // Stop du timer
                    timerSubmitCalibrationConfig.stop();
                    // Passage en busy
                    busyCalibSettings.visible = false;
                    calibrationID.bBusyState  = false;
                }
            }
            //--------------------
            // Busy : calibration settings
            //--------------------
            BusyIndicatorPerso {
                id: busyCalibSettings
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
                id: errorCalibSettings
                width: 31
                height: 31
                source: "qrc:/Logo/Global/No_Cross.png"
                anchors.left: btnConfirmSettings.right
                anchors.leftMargin: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                visible: false
            }

        } // Calibration settings

        //-----------------------------------------------
        // 3ième bloc : calibration info
        //-----------------------------------------------
        CalibrationSectionInfoBloc{
            id: sectionInfoBlocCh0
            visible: (calibrationPartID.m_uiCurrentChannel==0)
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.leftMargin: uiBlocWidthOffset
            anchors.rightMargin: uiBlocWidthOffset
            anchors.top: calibSettings.bottom
            anchors.topMargin: 28
            // Parameters
            m_sFormula:         calibrationPartID.m_sCh0Formula
            m_sCalibrationDate: calibrationPartID.m_sCh0CalibrationDate
        }
        CalibrationSectionInfoBloc{
            id: sectionInfoBlocCh1
            visible: (calibrationPartID.m_uiCurrentChannel==1)
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.leftMargin: uiBlocWidthOffset
            anchors.rightMargin: uiBlocWidthOffset
            anchors.top: calibSettings.bottom
            anchors.topMargin: 28
            // Parameters
            m_sFormula:         calibrationPartID.m_sCh1Formula
            m_sCalibrationDate: calibrationPartID.m_sCh1CalibrationDate
        }
        CalibrationSectionInfoBloc{
            id: sectionInfoBlocCh2
            visible: (calibrationPartID.m_uiCurrentChannel==2)
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.leftMargin: uiBlocWidthOffset
            anchors.rightMargin: uiBlocWidthOffset
            anchors.top: calibSettings.bottom
            anchors.topMargin: 28
            // Parameters
            m_sFormula:         calibrationPartID.m_sCh2Formula
            m_sCalibrationDate: calibrationPartID.m_sCh2CalibrationDate
        }

        //----------
        // Ch0
        //----------
        CalibrationPointPart {
            id: calibrationPointCh0
            height: 318
            width: calibrationPartID.width - 120
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: sectionInfoBlocCh0.bottom
            anchors.topMargin: 28
            visible: ( calibrationPartID.m_uiCurrentChannel == 0 )
            m_tCalibPoint: calibrationPartID.m_tCh0CalibPoint
            m_uiChannelIndex: 0
            m_uiID: calibrationID.ch0ID
        }
        //----------
        // Ch1
        //----------
        CalibrationPointPart {
            id: calibrationPointCh1
            height: 318
            width: calibrationPartID.width - 120
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: sectionInfoBlocCh1.bottom
            anchors.topMargin: 28
            visible: ( calibrationPartID.m_uiCurrentChannel == 1 )
            m_tCalibPoint: calibrationPartID.m_tCh1CalibPoint
            m_uiChannelIndex: 1
            m_uiID: calibrationID.ch1ID
        }
        //----------
        // Ch2
        //----------
        CalibrationPointPart {
            id: calibrationPointCh2
            height: 318
            width: calibrationPartID.width - 120
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: sectionInfoBlocCh2.bottom
            anchors.topMargin: 28
            visible: ( calibrationPartID.m_uiCurrentChannel == 2 )
            m_tCalibPoint: calibrationPartID.m_tCh2CalibPoint
            m_uiChannelIndex: 2
            m_uiID: calibrationID.ch2ID
        }
        //---------------------------------------------
        // Export au format PDF
        //---------------------------------------------
        Rectangle {
            id: rectangleExportToPdf
            width: 110
            height: 35
            color: "#00ffffff"
            anchors.right: parent.right
            anchors.rightMargin: 60
            anchors.top: parent.top
            anchors.topMargin: 40
            Button {
                id: btnExportToPdf
                //x: 1
                width: 115
                height: 35
                text: qsTr("Export to pdf")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                display: AbstractButton.TextBesideIcon
                icon.source: "qrc:/Logo/Calibration/calib-pdf-export3.png"
                icon.height: 30
                icon.width: 22

                onClicked: {
                    //
                    var sCalibFilename;
                    sCalibFilename = calibrationID.sFGenerateCalibReportFilename();

                    fileDialogExportToPdf.currentFile = "file:///"+sCalibFilename
                    fileDialogExportToPdf.folder = StandardPaths.writableLocation(StandardPaths.DesktopLocation)

                    // Création d'une fenêtre pour sélection du repertoire d'enregistrement
                    fileDialogExportToPdf.open()
                }
            }
            // Ajout d'un busy ou fenêtre de validation
            BusyIndicatorPerso {
                id: busyExportToPdf
                width: 31
                height: 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: false
            }
            //-----------------------------
            // Chemin d'enregistrement
            //-----------------------------
            FileDialog {
                id: fileDialogExportToPdf
                title: qsTr("Save calibration report")
                fileMode: FileDialog.SaveFile
                //folder: myObjHasAPath? myObj.path: "file:///" // Here you can set your default folder
                //currentFile: "file:///"+myObj.name // The name of the item that you want to save
                //fileMode: Platform.FileDialog.SaveFile
                nameFilters: ["PDF (*.pdf)"]
                //%%AP - 2021.05.18 -
                //folder: Folder
                // On capture le dossier courant
                onVisibleChanged: {
                    // Get
                    var sPath = calibrationID.getCurrentPathFile()
                    fileDialogExportToPdf.folder = "file:///"+sPath
                }

                onAccepted: {
                    // Apparition du busy à la place du bouton
                    btnExportToPdf.visible  = false
                    busyExportToPdf.visible = true
                    // Protection contre click
                    vFLockUserClick();
                    //console.log( "url : "+fileUrl )
                    //var theurl = fileUrl.toString().replace("file:///","");
                    //console.log("theurl : "+theurl)
                    sPdfCalibReportFilename = fileDialogExportToPdf.file
                    sPdfCalibReportFilename = sPdfCalibReportFilename.toString().replace("file:///","")

                    console.log("fileDialogExportToPdf.files = "+fileDialogExportToPdf.file)
                    console.log("fileDialogExportToPdf.files = "+fileDialogExportToPdf.files)
                    console.log("fileDialogExportToPdf.folder = "+fileDialogExportToPdf.folder)
                    console.log("sPdfCalibReportFilename = "+sPdfCalibReportFilename)

                    calibrationID.exportCalibrationToPdf(sPdfCalibReportFilename)
                    // Réinit du timeout
                    uiCalibReportTimeoutcpt = 20;
                    //tFactoryEvent.startWriteConfig( theurl );
                    fileDialogExportToPdfTimer.start()
                }
                onRejected: {
                    //console.log("File dialog rejected");
                    // Apparition du bouton à la place du busy
                    btnExportToPdf.visible  = true
                    busyExportToPdf.visible = false
                    // Protection contre click
                    vFAllowUserClick();
                }
            }
            Timer {
                id: fileDialogExportToPdfTimer
                interval: 100; running: false; repeat: true
                onTriggered: {
                    // Test si le fichier a bien été créé
                    if( calibrationID.bFIsCalibrationReportExist(sPdfCalibReportFilename) ){
                        //
                        console.log( "sPdfCalibReportFilename = "+sPdfCalibReportFilename );
                        // Apparition du bouton à la place du busy
                        btnExportToPdf.visible  = true
                        busyExportToPdf.visible = false
                        // Protection contre click
                        vFAllowUserClick();
                        // Stop du timer
                        fileDialogExportToPdfTimer.stop();
                        // Message de confirmation
                        messageDialogExportToPdf.open()
                    }
                    // Décrément : timeout ?
                    if( ( --uiCalibReportTimeoutcpt ) == 0 ){
                        // Apparition du bouton à la place du busy
                        btnExportToPdf.visible  = true
                        busyExportToPdf.visible = false
                        // Protection contre click
                        vFAllowUserClick();
                        // Message d'erreur
                        messageDialogExportToPdfError.open()
                    }
                }
            }
            // Validation export pdf
            MessageDialog {
                id: messageDialogExportToPdf
                title: qsTr("Export to pdf")
                text: qsTr("Calibration report saved")
            }
            // Validation export pdf
            MessageDialog {
                id: messageDialogExportToPdfError
                title: qsTr("Export to pdf")
                text: qsTr("Calibration report export error")
            }
        }
    }

}





/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
