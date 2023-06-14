import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQml.Models 2.15
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1
import "Component"
import "Windows"

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Parameter information
    property int m_uiID: 0
    property var m_tCalibPoint:          ListModel{}
    // Index de la voie
    property int m_uiChannelIndex: 0
    // Nombre maximum de points de calibration
    //property int uiMaxCalibrationPointNumber: (((m_uiID==6)||(m_uiID==7)||(m_uiID==25))?2:14)
    property int uiMaxCalibrationPointNumber: 14

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // Fonction externe
    //==============================================================
    //--------------------------------------------------------------
    // Fin du busy
    //--------------------------------------------------------------
    function vFEndOfBusy(){
        busyCalibPoints.visible   = false
    }
    //--------------------------------------------------------------
    // Fin du error
    //--------------------------------------------------------------
    function vFEndOfError(){
        errorCalibPoints.visible   = false
    }

    //==============================================================
    // QML
    //==============================================================
    id: calibrationPointPart
    implicitWidth: 618
    implicitHeight: 318

    //-----------------------------------------------
    // 4ième bloc : calibration points
    //-----------------------------------------------
    DropShadow {
        color: "#20000000"
        radius: 12
        verticalOffset: 0
        anchors.fill: rectCalibrationPoint
        samples: 30
        horizontalOffset: 0
        source: rectCalibrationPoint
    }
    Rectangle {
        id: rectCalibrationPoint
        //x: 25
        //y: 506
        //width: calibrationPartID.width - 120
        //width: 618
        //height: 318
        color: "#ffffff"
        anchors.fill: parent
        // Calibration points - Title
        Rectangle {
            id: rectangle15
            x: 0
            y: -30
            width: parent.width
            height: 34
            color: "#e4e3e9"
            Text {
                id: element8
                x: 63
                y: 14
                text: qsTr("Calibration points")
                font.bold: false
                font.pixelSize: 13
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                //font.weight: Font.DemiBold
                //font.family: "Open Sans"
                font.family: openSansDemiBoldFont.name
            }
            anchors.top: parent.top
            anchors.topMargin: 0
            clip: false
            transformOrigin: Item.Center
        }

        CalibTableView{
            id: tableView
            x: 16
            y: 49
            height: 252
            m_tCalibPoint: calibrationPointPart.m_tCalibPoint
            m_uiID: calibrationPointPart.m_uiID
        }

        ColumnLayout {
            id: columnLayoutButtonCalib
            anchors.right: parent.right
            anchors.rightMargin: 15
            width: parent.width * 1 / 3 - 45
            anchors.top: rectangle15.bottom
            anchors.topMargin: 31
            MessageDialog {
                id: maxPointNumberReachDialog
                text: qsTr("Error: maximum point number reached")
                //informativeText: "Error: maximum point number reached"
                //buttons: MessageDialog.Ok | MessageDialog.Cancel
                buttons: MessageDialog.Ok
                onAccepted: {}
            }
            //---------------
            // Bouton Add line
            //---------------
            Button {
                id: btnAddLine
                x: 400
                y: 55
                width: parent.width
                //height: 45
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 28
                text: qsTr("Add point")
                // Si activation
                enabled: !bNkeProdMode
                onClicked: {
                    // Test si nombre maximum de points atteints
                    console.log( "calibPoint.rowCount(): "+calibrationPointPart.m_tCalibPoint.rowCount() );
                    if( calibrationPointPart.m_tCalibPoint.rowCount() >= uiMaxCalibrationPointNumber ){
                        // Affichage popup d'erreur
                        maxPointNumberReachDialog.open()
                        // On quitte
                        return;
                    }
                    // Réinit du texte présent dans la popup
                    popTestPhy.text = "";
                    popTestRaw.text = "";
                    //
                    popTestID.bCalibPointModify = false;
                    popTestID.open();
                }
            }
            //---------------
            // Bouton Modify selected line
            //---------------
            Button {
                id: btnModifyLine
                x: 406
                y: 127
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 28
                text: qsTr("Modify selected point")
                // Si activation
                enabled: !bNkeProdMode
                onClicked: {
                    console.log(tableView.uiFGetCurrentRow())
                    if( tableView.uiFGetCurrentRow() == -1 ) return;
                    var m_calibPoint = calibrationPointPart.m_tCalibPoint

                    console.log( m_calibPoint.data(m_calibPoint.index(tableView.uiFGetCurrentRow(),0), 258) )
                    popTestPhy.text = m_calibPoint.data(m_calibPoint.index(tableView.uiFGetCurrentRow(),0), 258)
                    popTestRaw.text = m_calibPoint.data(m_calibPoint.index(tableView.uiFGetCurrentRow(),0), 259)
                    popTestID.bCalibPointModify = true;
                    popTestID.open();
                }
            }
            //---------------
            // Bouton Remove line
            //---------------
            Button {
                id: btnRemoveLine
                x: 406
                y: 182
                text: qsTr("Delete selected point")
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 28
                // Si activation
                enabled: !bNkeProdMode
                onClicked: {
                    console.log("Remove line");
                    // Passage en busy
                    busyCalibPoints.visible   = true
                    calibrationID.bBusyState  = true
                    calibrationID.removeCalibPoint(tableView.uiFGetCurrentRow(), calibrationPointPart.m_uiChannelIndex);
                    tMainEvent.saveChannelConfiguration( calibrationPointPart.m_uiChannelIndex );
                    // Lancement du timer
                    timerRemoveCalibrationPoint.bWriteDone = false;
                    timerRemoveCalibrationPoint.bReadDone = false;
                    timerRemoveCalibrationPoint.start();
                }
            }

            // Timer pour le fonctionnement du submit bouton remove point
            Timer {
                id: timerRemoveCalibrationPoint
                interval: 100;
                running: false;
                repeat: true;
                property bool bWriteDone: false
                property bool bReadDone: false
                onTriggered: {
                    // Tant que le grafcet est en busy
                    if( tMainEvent.isModbusBusyState() ){
                        return;
                    }
                    if( !timerRemoveCalibrationPoint.bWriteDone ){
                        timerRemoveCalibrationPoint.bWriteDone = true
                        tMainEvent.getChannelConfiguration( calibrationPointPart.m_uiChannelIndex );
                        return;
                    }
                    // Stop du timer
                    timerRemoveCalibrationPoint.stop();
                    // Passage en busy
                    busyCalibPoints.visible   = false
                    calibrationID.bBusyState  = false
                }
            }



            //--------------------
            // Busy : calibration points
            //--------------------
            BusyIndicatorPerso {
                id: busyCalibPoints
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 31
                Layout.preferredWidth: 31
                visible: false
            }
            //--------------------
            // Error : calibration points
            //--------------------
            Image {
                id: errorCalibPoints
                source: "qrc:/Logo/Global/No_Cross.png"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 31
                Layout.preferredWidth: 31
                visible: false
            }
        } // Fin Column Layout

        //---------------
        // Bouton Remove all lines
        //---------------
        Button {
            id: btnRemoveAllLines
            text: qsTr("Delete all points")
            anchors.bottom: tableView.bottom
            anchors.left: columnLayoutButtonCalib.left
            width: parent.width * 1 / 3 - 45

            // Si activation
            enabled: !bNkeProdMode
            onClicked: {
                console.log("Remove line");
                // Passage en busy
                busyCalibPoints.visible   = true
                calibrationID.bBusyState  = true
                calibrationID.removeAllCalibPoint();
                tMainEvent.saveChannelConfiguration( calibrationPointPart.m_uiChannelIndex );
                // Lancement du timer
                timerRemoveCalibrationPoint.bWriteDone = false;
                timerRemoveCalibrationPoint.bReadDone = false;
                timerRemoveCalibrationPoint.start();
            }
        }

        // Pop-up - Rentrée des points
        Popup {
            id: popTestID
            anchors.centerIn: parent
            property bool bCalibPointModify: false;

            //%%AP - 2020.08.24 -Ajout pour test
            modal: true
            focus: true

            onClosed: {
                // Quitte le popup
                popTestPhyTooltip.hide();
                popTestRawTooltip.hide();
            }

            enter: Transition {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1 }
            }
            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
            }
            ColumnLayout {
                //anchors.fill: parent
                //CheckBox { text: qsTr("E-mail") }
                //CheckBox { text: qsTr("Calendar") }
                //CheckBox { text: qsTr("Contacts") }
                Text{ text: qsTr("Standard") }
                TextField {
                    id: popTestPhy
                    height: 30
                    selectByMouse: true
                    // Touche ENTER (gauche)
                    Keys.onEnterPressed:{
                        console.log( "Press enter" );
                        popupSubmitBtn.clicked();
                    }
                    // Touche ENTER (droite)
                    Keys.onReturnPressed:{
                        console.log( "Press onReturnPressed" );
                        popupSubmitBtn.clicked();
                    }
                    ToolTip {
                        id: popTestPhyTooltip
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
                    /*
                    text: (  ( calibrationPointPart.m_uiID == 6 )
                          || ( calibrationPointPart.m_uiID == 7 )
                          || ( calibrationPointPart.m_uiID == 25 ) )?qsTr("Corrected"):qsTr("Raw")
                    */
                    text: qsTr("Measured")
                }
                TextField {
                    id: popTestRaw
                    height: 30
                    selectByMouse: true
                    Keys.onEnterPressed:{
                        console.log( "Press enter" );
                        popupSubmitBtn.clicked();
                    }
                    Keys.onReturnPressed:{
                        console.log( "Press onReturnPressed" );
                        popupSubmitBtn.clicked();
                    }
                    ToolTip {
                        id: popTestRawTooltip
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
                    id: popupSubmitBtn
                    text: qsTr("Submit")
                    //anchors.horizontalCenter: parent.horizontalCenter
                    //Layout.
                    //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter
                    visible: true
                    property bool bButtonClicked: false
                    onClicked: {
                        console.log( "==popupSubmitBtn==" )
                        // Test si le bouton a déjà été cliqué
                        if( bButtonClicked ) return;
                        // Le bouton a été clické
                        bButtonClicked = true;
                        // Passage en busy
                        busyCalibPoints.visible   = true
                        calibrationID.bBusyState  = true

                        console.log( "============================" );
                        console.log( "calibrationPointPart.m_uiChannelIndex" );
                        console.log( "============================" );
                        console.log( calibrationPointPart.m_uiChannelIndex );

                        var fphy = parseFloat( popTestPhy.text );
                        var fRaw = parseFloat( popTestRaw.text );

                        // Vérification validité valeur
                        if( !bFCheckGenericSensorIntegrity( fphy, fRaw ) ) {
                            // Passage en erreur
                            errorCalibPoints.visible  = true;
                            calibrationID.bErrorState = true;
                            bButtonClicked = false;
                            return;
                        }

                        fphy = parseFloat( popTestPhy.text );
                        fRaw = parseFloat( popTestRaw.text );

                        // Appel d'une fonction CPP pour enregistrement des points
                        calibrationID.addCalibPoint( calibrationPointPart.m_uiChannelIndex, fphy, fRaw );

                        // Quitte le popup
                        popTestPhyTooltip.hide();
                        popTestRawTooltip.hide();

                        // Sauvegarde de la structure de configuration vers le capteur
                        //inputDateTime.text = calibrationID.sSaveCurrentCalibrationDateTime( calibrationPointPart.m_uiChannelIndex );
                        calibrationID.sSaveCurrentCalibrationDateTime( calibrationPointPart.m_uiChannelIndex );
                        // Sauvegarde de la configuration
                        tMainEvent.saveChannelConfiguration( calibrationPointPart.m_uiChannelIndex );
                        console.log("Submit");
                        // Lancement du timer
                        console.log( "TIMER - Start" );
                        timerSubmitCalibrationPoint_WaitChannelWrite.bWriteDone = false;
                        timerSubmitCalibrationPoint_WaitChannelWrite.bReadDone = false;
                        timerSubmitCalibrationPoint_WaitChannelWrite.start();
                    }
                }
                // Timer pour le fonctionnement du submit bouton
                Timer {
                    id: timerSubmitCalibrationPoint_WaitChannelWrite
                    interval: 100;
                    running: false;
                    repeat: true;

                    property bool bWriteDone: false
                    property bool bReadDone: false

                    onTriggered: {
                        // Tant que le grafcet est en busy
                        if( tMainEvent.isModbusBusyState() ){
                            return;
                        }
                        if( !timerSubmitCalibrationPoint_WaitChannelWrite.bWriteDone ){
                            timerSubmitCalibrationPoint_WaitChannelWrite.bWriteDone = true
                            tMainEvent.getChannelConfiguration( calibrationPointPart.m_uiChannelIndex );
                            console.log( "TIMER READ" );
                            return;
                        }
                        //time.text = Date().toString()
                        // Stop du timer
                        timerSubmitCalibrationPoint_WaitChannelWrite.stop();
                        // Fermeture du pop-up
                        popTestID.close();
                        // Relache le verrou
                        popupSubmitBtn.bButtonClicked = false;
                        // Passage en busy
                        busyCalibPoints.visible   = false
                        calibrationID.bBusyState  = false
                        console.log( "TIMER END" );
                    }
                }
            }
        }
    }


    //-----------------------------------------------------------
    // Vérification de l'intégrité du point
    //-----------------------------------------------------------
    function bFCheckGenericSensorIntegrity( fPhy, fRaw ){
        var sPhy = popTestPhy.text.toString();
        var sRaw = popTestRaw.text.toString();
        var bO2Test = (  ( calibrationPointPart.m_uiID == 6  )
                      || ( calibrationPointPart.m_uiID == 7  )
                      || ( calibrationPointPart.m_uiID == 25 ) );
        var bCTDebug = (  ( calibrationPointPart.m_uiID == 3 )
                       || ( calibrationPointPart.m_uiID == 4 ) );
        console.log( "== bFCheckGenericSensorIntegrity ==" );
        console.log( sPhy );
        console.log( sRaw );

        // Si nitrate
        if(  ( calibrationPointPart.m_uiID == 23 )
          && ( parseFloat( sPhy ) == 0 ) ){
            popTestPhyTooltip.show(qsTr("0 is forbidden for nitrate sensor"));
            return false;
        }
        //%%AP - 2021.04.06 -
        // Test si c'est bien un nombre valide
        var tRegularExp = /^-?[0-9]+([.,][0-9]+)?$/;

        if( !tRegularExp.test(sPhy) ){
            popTestPhyTooltip.show(qsTr("Please enter a valid value"));
            return false;
        }
        if( !tRegularExp.test(sRaw) ){
            // Passage en erreur
            popTestRawTooltip.show(qsTr("Please enter a valid value"));
            return false;
        }
        //%%AP - 2021.04.06 -
        // Les virgules deviennent des points
        sPhy = popTestPhy.text.toString().replace(',','.');
        sRaw = popTestRaw.text.toString().replace(',','.');
        popTestPhy.text = sPhy;
        popTestRaw.text = sRaw;
        fPhy = parseFloat( sPhy );
        fRaw = parseFloat( sRaw );

        // Test si nombre décimal
        if( sPhy.split('.').length >= 2 ){
            //%%AP - 2020.11.24 - Changement de la méthode de détection du nombre de chiffre après la virgule
            // car l'arrondi des floats pose problème, exemple : 32.37 qui donne 32.369 arrondi à 1000
            // Phy : Test si max step est bien à 0.001
            if(  ( sPhy.split('.').length > 2  )
              || ( sPhy.split('.')[1].length <= 0 )
              || ( sPhy.split('.')[1].length >  3 ) ){
                // Passage en erreur
                popTestPhyTooltip.show(qsTr("Please enter a value with a maximum of 3 decimal places"));
                return false;
             }
         }
        // Si S18 Trp nke ou S19 Trp Turner
        if(  ( calibrationID.iCurrentChannelID == 18 )
          || ( calibrationID.iCurrentChannelID == 19 ) )
         {
            // Phy : Test si valeurs comprises entre -100000 et 100000
            if(  ( fPhy > 100000 )
              || ( fPhy < -100000 ) )
             {
                // Passage en erreur
                popTestPhyTooltip.show(qsTr("Value must be between -100000 and 100000"))
                return false;
             }
         }
        // Autres capteurs
        else{
            // Phy : Test si valeurs comprises entre -6000 et 6000
            if(  ( fPhy > 6000 )
              || ( fPhy < -6000 ) ){
                // Passage en erreur
                popTestPhyTooltip.show(qsTr("Value must be between -6000 and 6000"))
                return false;
             }
         }
        // Raw : Test si max step est bien à 0.1
        // Test si nombre décimal
        if( sRaw.split('.').length >= 2 ){
            // Raw : Test si max step est bien à 0.1
            if(  ( sRaw.split('.').length > 2  )
              || ( sRaw.split('.')[1].length <= 0 )
              || ( sRaw.split('.')[1].length > 3 ) ){
                // Passage en erreur
                popTestRawTooltip.show(qsTr("Please enter a value with a maximum of "+(3)+" decimal places"))
                return false;
            }
        }
        // Raw : Test si max step est bien à 0.001
        /*
        if(  ( fRaw > (bO2Test?6000:262143) )
          || ( fRaw < (bO2Test?-6000:0) ) ) {
            // Passage en erreur
            popTestRawTooltip.show(qsTr("Value must be between "+(bO2Test?-6000:0)+" and "+(bO2Test?6000:262143)+""))
            return false;
        }
        */
        if(  ( fRaw > 262143 )
          || ( fRaw < -262143 ) ) {
            // Passage en erreur
            popTestRawTooltip.show(qsTr("Value must be between "+(-262143)+" and "+(262143)+""))
            return false;
        }

        var fCurrentPhy = parseFloat( calibPoint.data(calibPoint.index(tableView.uiFGetCurrentRow(),0), 258) )
        var fCurrentRaw = parseFloat( calibPoint.data(calibPoint.index(tableView.uiFGetCurrentRow(),0), 259) )

        console.log("tableView.currentRow: "+tableView.uiFGetCurrentRow() )
        console.log("calibPoint.index(tableView.currentRow,0): "+calibPoint.index(tableView.uiFGetCurrentRow(),0) )
        console.log("popTestID.bCalibPointModify: "+popTestID.bCalibPointModify);
        console.log("bFIsPhyPointAlreadyExist( fphy ): "+bFIsPhyPointAlreadyExist( fPhy ) );
        console.log("bFIsRawPointAlreadyExist( fRaw ): "+bFIsRawPointAlreadyExist( fRaw ) );
        console.log("fCurrentPhy: "+fCurrentPhy+" fphy: "+fPhy);
        console.log("fCurrentRaw: "+fCurrentRaw+" fRaw: "+fRaw);
        //--------------------
        // Vérification que le point n'existe pas déjà
        //--------------------
        // Si c'est une modification
        console.log("popTestID.bCalibPointModify = " + popTestID.bCalibPointModify)
        if( popTestID.bCalibPointModify ){
            console.log("C'est une modification")
            if( bFIsPointAlreadyExist( fPhy, fRaw, tableView.uiFGetCurrentRow() ) ){
                console.log("bFIsPointAlreadyExist > true")
                // Passage en erreur
                errorCalibPoints.visible  = true
                calibrationID.bErrorState = true
                if( bFIsRawPointAlreadyExist( fRaw ) ){
                    popTestRawTooltip.show(qsTr("The point already exists"))
                }
                if( bFIsPhyPointAlreadyExist( fPhy ) ){
                    popTestPhyTooltip.show(qsTr("The point already exists"))
                }
                return false;
            }
            // Suppression de la ligne courrante
            calibrationID.removeCalibPoint(tableView.uiFGetCurrentRow(), m_uiChannelIndex);
        }
        else{
            // Le point Phy n'existe pas
            if( bFIsPhyPointAlreadyExist( fPhy ) ){
                // Passage en erreur
                popTestPhyTooltip.show(qsTr("The point already exists"))
                return false;
            }
            // Le point Raw n'existe pas
            if( bFIsRawPointAlreadyExist( fRaw ) ){
                // Passage en erreur
                popTestRawTooltip.show(qsTr("The point already exists"))
                return false;
            }
        }
        // Valeur valide
        return true;
    }
    //-----------------------------------------------------
    // Vérification que le point raw existe déjà
    //-----------------------------------------------------
    function bFIsRawPointAlreadyExist( fRawTest ){
        var fRawTemp;
        // Parcourt des points
        for( var i=0; i<calibPoint.rowCount(); i++ ){
            // Test si c'est la ligne courante
            if( i == tableView.uiFGetCurrentRow() ) continue;
            fRawTemp = parseFloat( calibPoint.data(calibPoint.index(i,0), 259) )
            // Test si le point physical existe
            if( fRawTemp == fRawTest ){
                // Passage en erreur
                return true;
            }
        }
        return false;
    }
    //-----------------------------------------------------
    // Vérification que le point raw existe déjà
    //-----------------------------------------------------
    function bFIsPhyPointAlreadyExist( fPhyTest ){
        var fPhyTemp;
        // Parcourt des points
        for( var i=0; i<calibPoint.rowCount(); i++ ){
            // Test si c'est la ligne courante
            if( i == tableView.uiFGetCurrentRow() ) continue;
            fPhyTemp = parseFloat( calibPoint.data(calibPoint.index(i,0), 258) )
            // Test si le point physical existe
            if( fPhyTemp == fPhyTest ){
                // Passage en erreur
                return true;
            }
        }
        return false;
    }
    //-----------------------------------------------------
    // Vérification que le point existe déjà
    //-----------------------------------------------------
    function bFIsPointAlreadyExist( fPhyTest, fRawTest, uiCurrentLine ){
        var fPhyTemp, fRawTemp;
        // Parcourt des points
        for( var i=0; i<calibPoint.rowCount(); i++ ){
            // Test si c'est la ligne courante
            if( i == uiCurrentLine ) continue;
            // Point Physique
            fPhyTemp = parseFloat( calibPoint.data(calibPoint.index(i,0), 258) )
            // Test si le point physical existe
            if( fPhyTemp == fPhyTest ){
                // Passage en erreur
                return true;
            }
            // Point Raw
            fRawTemp = parseFloat( calibPoint.data(calibPoint.index(i,0), 259) )
            // Test si le point physical existe
            if( fRawTemp == fRawTest ){
                // Passage en erreur
                return true;
            }
        }
        return false;
    }
}
