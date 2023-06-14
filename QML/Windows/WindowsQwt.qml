import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12


import "../Component"
import "../Windows"
import "../"

Window {
    id: windowQwt
    visible: true
    width: 640
    height: 480
    minimumWidth: 500
    minimumHeight: 300
    color: "#efeef4"
    //title: qsTr("Liveview window")
    title: "WiMo Calibration Tool - Liveview - "+ ( (sPortCOMMain!="")?((tMainData.sInterfaceSN!="")? (" ("+tMainData.sInterfaceSN+" - "+sPortCOMMain+")" ):" ("+sPortCOMMain+")" ):"")

    //---------------------------------------
    // Variable
    //---------------------------------------
    // Nombre de voie de mesure
    property int m_uiChannelNumber: 3
    // Résultat de mesure
    property double m_dCh0Data: 0.0
    property double m_dCh1Data: 0.0
    property double m_dCh2Data: 0.0
    property int uiRectangleWidth: 120
    //---------------------------------------
    // Fonction
    //---------------------------------------
    function vFDisplayNewSamples(){
        console.log("ECH")
        //liveviewID.
        plotview.vFDisplayNewSamples();
    }

    // Signal de fin de refresh
    function vFEnfOfRefreshSignal( uiChannelNumber ){
        // Propagation du signal
        plotview.vFEnfOfRefreshSignal()
        // Sauvegarde du nombre de voie
        m_uiChannelNumber = uiChannelNumber
    }

    // Signal de fin de refresh
    function vFWindowsQwt_UpdateGraphicalView( uiGraphicalDepth ){
        // Propagation du signal
        plotview.vFPlotView_UpdateGraphicalView( uiGraphicalDepth )
    }

    // Clear du graphique
    function vFWindowsQwt_ClearGraphic(){
        // Propagation du signal
        plotview.vFClearGraphic()
    }

    Connections {
        target:tLiveviewConfEvent
        //-------------------------------------------------
        // Réception d'une nouvelle donnée
        //-------------------------------------------------
        function onTransmitLiveViewDataSignal(fCh0Data, fCh1Data, fCh2Data) {
            windowQwt.m_dCh0Data = fCh0Data
            windowQwt.m_dCh1Data = fCh1Data
            windowQwt.m_dCh2Data = fCh2Data
        }
    }


    Rectangle{
        id: rectanglePlotView

        width: parent.width - 30
        height: parent.height - 18 - rowLayout.height - 5

        anchors.top: parent.top
        anchors.topMargin: 18
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#555555"
        border.width: 3

        PlotView {
            id: plotview
            width: parent.width - 40
            height: parent.height - 40
            anchors.verticalCenterOffset: -3
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        Button {
            id: buttonClearGraphic
            //y: 433
            width: 85
            height: 26
            text: qsTr("Clear")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            anchors.left: parent.left
            anchors.leftMargin: 3
            display: AbstractButton.TextBesideIcon
            icon.source: "qrc:/Logo/LiveView/liveview-clear.png"
            onClicked: {
                console.log("TEST");
                plotview.vFClearGraphic()
            }
        }
        Button {
            id: buttonExportGraphic
            width: 85
            height: 26
            text: qsTr("Export...")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            anchors.left: buttonClearGraphic.right
            anchors.leftMargin: 3
            display: AbstractButton.TextBesideIcon
            icon.source: "qrc:/Logo/LiveView/liveview-export.png"
            onClicked: {
                console.log("TEST");
                plotview.vFExport();
            }
        }
    }

    // Layout pour bouton de désactivation
    Rectangle {
        id: rowLayout
        visible: true

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 3
        width: parent.width * 3 / 4
        height: 110
        color: "Transparent"

        //---------------------------
        // Ch0
        //---------------------------
        Rectangle{
            id: windowsQwtCh0Rect
            height: parent.height
            width: ( windowQwt.width * 3 / 4 ) / 3
            color: "Transparent"
            BoutonSwitchExtendView {
                id: windowsQwtCh0Btn
                channelName: liveviewID.ch0Name
                width:  uiRectangleWidth
                height: parent.height - 15
                visible: ( m_uiChannelNumber > 0 )
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                tColor: liveviewID.ch0Active?ptColor[liveviewID.ch0Parameter]:ptColor["disabled"]
                dResult: windowQwt.m_dCh0Data
                onClicked: {
                    console.log(bChecked)
                    if( bChecked ){
                        plotview.vFEnableChannel(0)
                    }
                    else{
                        plotview.vFDisableChannel(0)
                    }
                }
            }
        }
        //---------------------------
        // Ch1
        //---------------------------
        Rectangle{
            id: windowsQwtCh1Rect
            height: parent.height
            width: ( windowQwt.width * 3 / 4 ) / 3
            anchors.left:windowsQwtCh0Rect.right
            color: "Transparent"
            BoutonSwitchExtendView{
                channelName: liveviewID.ch1Name
                width: uiRectangleWidth
                height: parent.height - 15
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: ( m_uiChannelNumber > 1 )
                tColor: liveviewID.ch1Active?ptColor[liveviewID.ch1Parameter]:ptColor["disabled"]
                dResult: windowQwt.m_dCh1Data
                onClicked: {
                    console.log(bChecked)
                    if( bChecked ){
                        plotview.vFEnableChannel(1)
                    }
                    else{
                        plotview.vFDisableChannel(1)
                    }
                }
            }
        }
        //---------------------------
        // Ch2
        //---------------------------
        Rectangle{
            id: windowsQwtCh2Rect
            height: parent.height
            width: ( windowQwt.width * 3 / 4 ) / 3
            anchors.left:windowsQwtCh1Rect.right
            color: "Transparent"
            BoutonSwitchExtendView{
                channelName: liveviewID.ch2Name
                width: uiRectangleWidth
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height - 15
                visible: ( m_uiChannelNumber > 2 )
                tColor: liveviewID.ch2Active?ptColor[liveviewID.ch2Parameter]:ptColor["disabled"]
                dResult: windowQwt.m_dCh2Data

                onClicked: {
                    console.log(bChecked)
                    if( bChecked ){
                        plotview.vFEnableChannel(2)
                    }
                    else{
                        plotview.vFDisableChannel(2)
                    }
                }
            }
        }
    }
}


