import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.13
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.13
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.1
import LibWiper 1.0

import "Component"
import "Windows"

Item {
   // Wiper grafcet
   property bool bWiperGrafcetStarted: false

    //=============================================================
    // Objet Wiper
    //=============================================================
    Wiper {
        id: wiperID
        objectName: "wiperID"
        uiClockwise: 0
        uiAnticlockwise: 0
        iOffset: 0
        uiMin: 0
        uiMax: 0
        bBusyState: false
        bErrorState: false
        // Changement d'état busy
        onBusyStateChanged:{
            console.log("==onBusyStateChanged==");
            console.log("wiperID.bBusyState = "+wiperID.bBusyState)
            // Busy on
            if( wiperID.bBusyState ){
                // On va bloquer le click
                //mouseAreaWindowBlockClick.visible = true
                vFLockUserClick();
                // On désactive l'état d'erreur si présent
                errorWiperSettings.visible = false
                wiperID.bErrorState        = false
            }
            // Busy off
            else{
                // On va débloquer le click
                //mouseAreaWindowBlockClick.visible = false
                vFAllowUserClick();
                // On fait disparaitre les busy
                busyWiperSettings.visible      = false
                busyRefresh.visible            = false
                busyRefreshCalibration.visible = false
                // Element montré
                //btnFormulaRefresh.visible  = true
            }
        }
        onErrorStateChanged:{
            console.log("==onErrorStateChanged==");
            console.log("wiperID.bErrorState = "+wiperID.bErrorState)
            // Error On
            if( wiperID.bErrorState ){
                // Désactivation de l'état busy
                wiperID.bBusyState  = false
            }
            // Error off
            else{
                // On fait disparaitre les croix d'erreur
                errorWiperSettings.visible = false
            }
        }
    }

    //*************************************************************
    // Le code de l'objet
    //*************************************************************
    implicitWidth: 662
    implicitHeight: 888
    id: wiperPartID

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#efeef4"

        //--------------------------------------------
        // Titre wiper avec logo
        //--------------------------------------------
        Rectangle {
            id: triangleTitleWiper
            x: 92
            y: 91
            width: 28
            height: 28
            color: "#e4e3e9"
            rotation: 45
        }
        Text {
            id: wiperTitle
            x: 0
            y: 38
            width: 404
            height: 27
            text: qsTr("Wiper")
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
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
            font.weight: Font.DemiBold
            font.family: "Open Sans"
        }

        //-----------------------------------------------
        // Premier bloc : wiper settings
        //-----------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: wiperSettings
            samples: 30
            horizontalOffset: 0
            source: wiperSettings
        }
        // Wiper settings
        Rectangle {
            id: wiperSettings
            //x: 25
            y: 100
            width: wiperPartID.width - 120
            height: 200
            color: "#ffffff"
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: rectTitleWiperSettings
                x: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: element4
                    x: 63
                    y: 14
                    text: qsTr("Wiper test angle")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 13
                    font.family: "Open Sans"
                    font.weight: Font.DemiBold
                    font.bold: false
                }
                anchors.topMargin: 0
                clip: false
                anchors.top: parent.top
                transformOrigin: Item.Center
            }

            //------------------------------------------------
            // Wiper settings
            //------------------------------------------------
            ColumnLayout {
                id: columnLayoutWiperSettings
                x: 0
                y: 42
                width: parent.width
                height: 95

                //------------------------------
                // Anticlockwise angle
                //------------------------------
                RowLayout {
                    id: rowAnticlockwiseAngle
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowAnticlockwiseAngle2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgAnticlockwiseAngle
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16

                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtAnticlockwiseAngle
                            text: qsTr("Anticlockwise")
                            height: parent.height
                            anchors.left: imgAnticlockwiseAngle.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    TextField {
                        id: inputAnticlockwise
                        text: wiperID.uiAnticlockwise
                        height: 30
                        activeFocusOnPress: true
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        selectByMouse: true

                        ToolTip {
                            id: tooltipAnticlockwise
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
                        id: rectangleAnticlockwiseDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            id: txtAnticlockwiseAngleUnit
                            text: qsTr("degree")
                            height: parent.height
                            //anchors.left: imgClockwiseAngle.right
                            //anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                } // RowLayout Average

                //--- Ligne ---
                Rectangle {
                    id: anticlockwiseLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                //------------------------------
                // Clockwise angle
                //------------------------------
                RowLayout {
                    id: rowClockwiseAngle
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowClockwiseAngle2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgClockwiseAngle
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Calibration/calib-sensor.png"
                        }

                        Text {
                            id: txtClockwiseAngle
                            text: qsTr("Clockwise")
                            height: parent.height
                            anchors.left: imgClockwiseAngle.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    TextField {
                        id: inputClockwise
                        text: wiperID.uiClockwise
                        height: 30
                        activeFocusOnPress: true
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        selectByMouse: true

                        ToolTip {
                            id: tooltipClockwise
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
                    } // Fin Textfield
                    Rectangle {
                        id: rectangleClockwiseAngleDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            id: txtClockwiseAngleUnit
                            text: qsTr("degree")
                            height: parent.height
                            //anchors.left: imgClockwiseAngle.right
                            //anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                } // End of RowLayout
                //--- Ligne ---
                Rectangle {
                    id: clockwiseAngleLine
                    width: parent.width
                    height: 1
                    color: "#62888888"
                    Layout.fillWidth: true
                }

                /**/

                //------------------------------
                // Offset angle
                //------------------------------
                RowLayout {
                    id: rowOffsetAngle
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    width: parent.width - 20

                    Rectangle {
                        id: rowOffsetAngle2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height
                        Image{
                            id: imgOffsetAngle
                            source: "qrc:/Logo/Calibration/calib-average.png"
                            width: 30
                            height: 16

                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txtOffsetAngle
                            text: qsTr("Offset")
                            height: parent.height
                            anchors.left: imgOffsetAngle.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                    TextField {
                        id: inputOffset
                        text: wiperID.iOffset
                        height: 30
                        activeFocusOnPress: true
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        selectByMouse: true

                        ToolTip {
                            id: tooltipOffset
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
                        id: rectangleOffsetDummy
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            id: txtOffsetAngleUnit
                            text: qsTr("degree")
                            height: parent.height
                            //anchors.left: imgClockwiseAngle.right
                            //anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "open sans"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }
                } // RowLayout Average

                //--- Ligne ---
                Rectangle {
                    id: offsetLine
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

                onClicked: {
                    // On désactive l'état d'erreur si présent
                    errorWiperSettings.visible = false
                    // On active l'état occupé
                    wiperID.bBusyState = true
                    // On fait apparaître le busy
                    busyWiperSettings.visible = true
                    // On va bloquer le click
                    //mouseAreaWindowBlockClick.visible = true

                    //---------------------
                    // Clockwise angle
                    //---------------------
                    console.log( "inputClockwise: "+inputClockwise.text )
                    var uiClockwise;
                    uiClockwise = parseInt( inputClockwise.text );
                    // Test d'intégrité
                    if(  (  ( uiClockwise <= 35 )
                         || ( uiClockwise > 270 ) )
                      && ( uiClockwise != 0 ) )
                     {
                        // On désactive l'état occupé
                        wiperID.bBusyState         = false
                        busyWiperSettings.visible  = false
                        errorWiperSettings.visible = true
                        tooltipClockwise.show("Value must be 0 or between 36 and 270")
                        return;
                     }
                    var fAvg        = parseFloat( inputClockwise.text );
                    var fAvgRounded = parseFloat(parseInt(fAvg));
                    // Vérification fonctionnement avec un entier
                    if( fAvg !== fAvgRounded )
                     {
                        // On désactive l'état occupé
                        wiperID.bBusyState         = false
                        busyWiperSettings.visible  = false
                        errorWiperSettings.visible = true
                        tooltipClockwise.show("Value must be an integer")
                        return;
                     }

                    //---------------------
                    // Anticlockwise angle
                    //---------------------
                    console.log( "inputAnticlockwise: "+inputAnticlockwise.text )
                    var uiAnticlockwise;
                    uiAnticlockwise = parseInt( inputAnticlockwise.text );
                    // Test d'intégrité
                    if(  (  ( uiAnticlockwise <= 35 )
                         || ( uiAnticlockwise > 270 ) )
                      && ( uiAnticlockwise != 0 ) )
                     {
                        // On désactive l'état occupé
                        wiperID.bBusyState         = false
                        busyWiperSettings.visible  = false
                        errorWiperSettings.visible = true
                        tooltipAnticlockwise.show("Value must be 0 or between 36 and 270")
                        return;
                     }
                    var fAnticlockwise        = parseFloat( inputAnticlockwise.text );
                    var fAnticlockwiseRounded = parseFloat(parseInt(fAnticlockwise));
                    // Vérification fonctionnement avec un entier
                    if( fAnticlockwise !== fAnticlockwiseRounded )
                     {
                        // On désactive l'état occupé
                        wiperID.bBusyState         = false
                        busyWiperSettings.visible  = false
                        errorWiperSettings.visible = true
                        tooltipAnticlockwise.show("Value must be an integer")
                        return;
                     }

                    //---------------------
                    // Offset angle
                    //---------------------
                    console.log( "inputOffset: "+inputOffset.text )
                    var iOffset;
                    iOffset = parseInt( inputOffset.text );
                    // Test d'intégrité
                    if(  ( iOffset > 90 )
                      || ( iOffset < -90 ) )
                     {
                        // On désactive l'état occupé
                        wiperID.bBusyState         = false
                        busyWiperSettings.visible  = false
                        errorWiperSettings.visible = true
                        tooltipAnticlockwise.show("Value must be between -90 and 90")
                        return;
                     }
                    var fOffset        = parseFloat( inputOffset.text );
                    var fOffsetRounded = parseFloat(parseInt(fOffset));
                    // Vérification fonctionnement avec un entier
                    if( fOffset !== fOffsetRounded )
                     {
                        // On désactive l'état occupé
                        wiperID.bBusyState         = false
                        busyWiperSettings.visible  = false
                        errorWiperSettings.visible = true
                        tooltipAnticlockwise.show("Value must be an integer")
                        return;
                     }

                    // Mise à jour des variables
                    wiperID.uiClockwise     = uiClockwise;
                    wiperID.uiAnticlockwise = uiAnticlockwise;
                    wiperID.iOffset         = iOffset;
                    // Récupération des paramètres à jour
                    wiperID.submitWiperAngle(parseInt(uiClockwise),
                                             parseInt(uiAnticlockwise),
                                             parseInt(iOffset));
                    // Mise à jour du datetime de calibration
                    //inputDateTime.text = wiperID.sSaveCurrentCalibrationDateTime( calibrationID.currentChannel );
                    // Sauvegarde de la configuration
                    wiperID.saveConfiguration();
                    //tooltipAverageCalib.show("Invalid value",3000)
                }
            }
            //--------------------
            // Busy : wiper settings
            //--------------------
            BusyIndicatorPerso {
                id: busyWiperSettings
                width: 31
                height: 32
                anchors.left: btnConfirmSettings.right
                anchors.leftMargin: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                visible: false
            }
            //--------------------
            // Error : wiper settings
            //--------------------
            Image {
                id: errorWiperSettings
                width: 31
                height: 31
                source: "qrc:/Logo/Global/No_Cross.png"
                anchors.left: btnConfirmSettings.right
                anchors.leftMargin: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                visible: false
            }

        } // Wiper settings

        //----------------------------------------------------------------
        //--------------------------------------------------
        // Bloc : Wiper calibration
        //--------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: wiperCalibration
            samples: 30
            horizontalOffset: 0
            source: wiperCalibration
        }
        Rectangle {
            id: wiperCalibration
            width: wiperPartID.width - 120

            height: 86
            color: "#ffffff"
            anchors.top: wiperSettings.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: wiperCalibrationRect15
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: wiperCalibrationElt5
                    x: 63
                    y: 14
                    text: qsTr("Wiper calibration")
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
                id: columnLayoutWiperCalibration
                x: 0
                y: 38
                width: parent.width
                height: 43

                RowLayout {
                    id: rowWiperCalibration
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30

                    Rectangle {
                        id: rowWiperCalibration2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgWiperCalibration
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                        }

                        Text {
                            id: txtWiperCalibration
                            text: qsTr("Launch a calibration")
                            height: parent.height
                            anchors.left: imgWiperCalibration.right
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
                        id: rowWiperCalibration3
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        ////////////////////////////////////////
                        DropShadow {
                            color: "#20000000"
                            radius: 10
                            verticalOffset: 0
                            anchors.fill:btnWiperCalibrationRect
                            samples: 25
                            horizontalOffset: 0
                            source: btnWiperCalibrationRect
                        }
                        Rectangle {
                            id: btnWiperCalibrationRect
                            width: 100
                            height: 28
                            Layout.leftMargin: 41
                            Layout.preferredHeight: 30
                            color: "#193d8a"
                            radius: 7
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            transformOrigin: Item.Center
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                            Timer {
                                id: btnWiperCalibrationTimer
                                interval: 2200; running: false; repeat: false
                                onTriggered: {
                                    busyRefreshCalibration.visible = false
                                }
                            }

                            Text {
                                id: btnWiperCalibrationTxt
                                x: 28
                                y: 17
                                color: "white"
                                text: qsTr("Start")
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter
                                anchors.verticalCenterOffset: 0
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: "Open Sans"
                                font.pixelSize: 11
                            }

                            MouseArea {
                                id: mouseAreaBtnCalibrationWiper
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    console.log(" clicked" )
                                    // On active l'état occupé
                                    wiperID.bBusyState = true
                                    // On fait apparaître le busy
                                    busyRefreshCalibration.visible = true
                                    // Déclenchement du balai
                                    wiperID.startCalibration();
                                }
                                onEntered: {
                                    parent.color = "#32549C"
                                }
                                onExited:  {
                                    parent.color = "#193d8a"
                                }
                            }
                        }
                        ////////////////////////////////



                    }

                    Rectangle {
                        id: rectangleRefreshCalibrationLoad2
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        BusyIndicatorPerso {
                            id: busyRefreshCalibration
                            width: 36
                            height: 37
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            visible: false
                        }
                        Text {
                            id: wiperCalibrationElt51
                            text: wiperID.uiMin + " " + wiperID.uiMax
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.DemiBold
                            font.family: "Open Sans"
                            font.bold: false
                            font.pixelSize: 13
                        }

                    }
                }
            }
        } // Fin bloc refresh
        //--------------------------------------------------------------------------
        //----------------------------------------------------------------
        //--------------------------------------------------
        // Bloc : Wiper test
        //--------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: wiperTest
            samples: 30
            horizontalOffset: 0
            source: wiperTest
        }
        Rectangle {
            id: wiperTest
            width: wiperPartID.width - 120

            height: 86
            color: "#ffffff"
            anchors.top: wiperCalibration.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: wiperTestRect15
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: wiperTestElt5
                    x: 63
                    y: 14
                    text: qsTr("Wiper test")
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
                id: columnLayoutWiperTest
                x: 0
                y: 38
                width: parent.width
                height: 43

                RowLayout {
                    id: rowWiperTest
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30

                    Rectangle {
                        id: rowWiperTest2
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgWiperTest
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                        }

                        Text {
                            id: txtWiperTest
                            text: qsTr("Launch a test")
                            height: parent.height
                            anchors.left: imgWiperTest.right
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
                        id: rowWiperTest3
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        ////////////////////////////////////////
                        DropShadow {
                            color: "#20000000"
                            radius: 10
                            verticalOffset: 0
                            anchors.fill:btnWiperTestRect
                            samples: 25
                            horizontalOffset: 0
                            source: btnWiperTestRect
                        }
                        Rectangle {
                            id: btnWiperTestRect
                            width: 100
                            height: 28
                            Layout.leftMargin: 41
                            Layout.preferredHeight: 30
                            color: "#193d8a"
                            radius: 7
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            transformOrigin: Item.Center
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                            Timer {
                                id: btnWiperTimer
                                interval: 2200; running: false; repeat: false
                                onTriggered: {
                                    busyRefresh.visible = false
                                    /*
                                    inputFirmware.text  = "CT nke"
                                    inputSN.text        = "20-00001"
                                    inputIdentifier.text   = "CT"
                                    inputManufacturer.text = "nke"
                                    inputVersion.text      = "1.0.2"
                                    */
                                }
                            }

                            Text {
                                id: btnWiperTestTxt
                                x: 28
                                y: 17
                                color: "white"
                                text: qsTr("Start")
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter
                                anchors.verticalCenterOffset: 0
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: "Open Sans"
                                font.pixelSize: 11
                            }

                            MouseArea {
                                id: mouseAreaBtnWiper
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    console.log(" clicked" )
                                    // On active l'état occupé
                                    wiperID.bBusyState = true
                                    // On fait apparaître le busy
                                    busyRefresh.visible = true
                                    // Déclenchement du balai
                                    wiperID.startWiper();
                                }
                                onEntered: {
                                    parent.color = "#32549C"
                                }
                                onExited:  {
                                    parent.color = "#193d8a"
                                }
                            }
                        }
                        ////////////////////////////////



                    }

                    //Layout.preferredWidth: ( parent.width - 15 ) / 3
                    Rectangle {
                        id: rectangleRefreshLoad2
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        BusyIndicatorPerso {
                            id: busyRefresh
                            width: 36
                            height: 37
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            visible: false
                        }
                    }
                }
            }
        } // Fin bloc refresh
        //--------------------------------------------------------------------------

         //----------------------------------------------------------------
        //--------------------------------------------------
        // Bloc : Wiper test
        //--------------------------------------------------
        DropShadow {
            color: "#20000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: wiperTest2
            samples: 30
            horizontalOffset: 0
            source: wiperTest2
        }
        Rectangle {
            id: wiperTest2
            //x: 60
            //y: 99
            width: wiperPartID.width - 120

            //implicitWidth: 536
            height: 40+46*2
            color: "#ffffff"
            anchors.top: wiperTest.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            //visible: ( tMainData.bNkeMode || tMainData.bMetroMode || tMainData.bProdMode )

            Rectangle {
                id: wiperTestRect152
                x: 0
                y: 0
                width: parent.width
                height: 34
                color: "#e4e3e9"
                Text {
                    id: wiperTestElt52
                    x: 63
                    y: 14
                    text: qsTr("Wiper periodic test")
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
                id: columnLayoutWiperTest2
                x: 0
                y: 38
                width: parent.width
                height: 43*2

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
                            text: qsTr("Trigger period")
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
                        id: inputWiperGrafcetPeriod
                        height: 30
                        from: 30
                        to: 3600
                        editable: true
                        //model: [ "1", "2", "5" ]
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        //enabled: !bStarMeasureSwitchState
                        //%%AP - NOP - selectByMouse: true
                    }
                    Rectangle {
                        id: rectangleMPDummy
                        width: 200
                        height: 200
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        Text {
                            text: "seconds"
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



                RowLayout {
                    id: rowWiperTest222
                    width: parent.width - 20
                    height: 30
                    Layout.leftMargin: 10
                    spacing: 5
                    Layout.preferredWidth: parent.width - 20
                    Layout.preferredHeight: 30

                    Rectangle {
                        id: rowWiperTest22
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        Image {
                            id: imgWiperTest2
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/Logo/Dashboard/dashboard-refresh.png"
                        }

                        Text {
                            id: txtWiperTest2
                            text: qsTr("Launch test")
                            height: parent.height
                            anchors.left: imgWiperTest2.right
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
                        id: rowWiperTest32
                        width: ( parent.width - 25 ) / 3
                        Layout.preferredWidth: ( parent.width - 15 ) / 3
                        height: parent.height
                        Layout.preferredHeight: parent.height

                        ////////////////////////////////////////
                        DropShadow {
                            color: "#20000000"
                            radius: 10
                            verticalOffset: 0
                            anchors.fill:btnWiperStartGrafcet
                            samples: 25
                            horizontalOffset: 0
                            source: btnWiperStartGrafcet //btnRefreshRect
                        }
                        BlueButtonPerso{
                            id: btnWiperStartGrafcet
                            text: qsTr("Start")
                            width: 100
                            height: 28
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            //visible: !(bWiperGrafcetStarted)
                            //visible: true
                            onButtonClicked: {
                                console.log("Start clicked");
                                btnWiperStartGrafcet.visible = false
                                btnWiperStopGrafcet.visible = true
                                // Déclenchement du balai
                                timerGrafcetBalai.start();
                                wiperID.startWiper();
                                bWiperGrafcetStarted = true
                            }
                        }
                    }

                    Rectangle {
                        width: 200
                        height: 200
                        color: "#ffffff"
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth: ( parent.width - 15 ) / 3

                        BlueButtonPerso{
                            id: btnWiperStopGrafcet
                            text: qsTr("Stop")
                            width: 100
                            height: 28
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            //visible: bWiperGrafcetStarted
                            visible: false
                            onButtonClicked: {
                                console.log("Stop clicked");
                                btnWiperStartGrafcet.visible = true
                                btnWiperStopGrafcet.visible = false
                                timerGrafcetBalai.stop()
                                bWiperGrafcetStarted = false
                                // Déclenchement du balai
                                //wiperID.startWiper();
                            }
                        }
                    }

                   Timer {
                        id: timerGrafcetBalai
                        interval: inputWiperGrafcetPeriod.value*1000
                        running: false
                        repeat: true
                        onTriggered: {
                            console.log("Test balai")
                            // Déclenchement du balai
                            wiperID.startWiper();
                        }
                    }
                }
            }
        } // Fin bloc refresh
        //--------------------------------------------------------------------------

    }

}
