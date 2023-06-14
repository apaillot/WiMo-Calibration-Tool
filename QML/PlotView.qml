import QtQuick 2.0
import QwtQuick2 1.0
//import LibLiveview 1.0
import QtQuick.Dialogs 1.3
//import "componentCreation.js" as MyScriptQWT
//import "componentCreation.js" as qwtPlot

import "Component"
import "Windows"

Item {
    id: plotForm
    property bool bInitComponent: true

    width: parent.width - 40
    height: parent.height - 40

    Rectangle{
        color: "#999999"
        width: parent.width
        height: parent.height
    }

    Text {
        id: text
        text: qsTr("Plot form error")
    }

    QwtQuick2Plot {
        id: qwtPlot
        anchors.fill: parent
        //visible:false

        Component.onCompleted: {
            console.log("QwtQuick2Plot on completed : initQwtPlot")
            //qwtPlot.destroy()
            // Init
            initQwtPlot()
        }
    }


    // Export du graphique en pdf
    function vFExport(){
        qwtPlot.exportQwtPlot();
    }
    // Désactivation voie
    function vFDisableChannel( iChannel ){
        qwtPlot.disableChannel(iChannel)
    }
    // Désactivation voie
    function vFEnableChannel( iChannel ){
        qwtPlot.enableChannel(iChannel)
    }
    // Désactivation voie
    function vFClearGraphic(){
        qwtPlot.clearGraphic();
    }
    function vFDisplayNewSamples(){
        var iChannelNumber = liveviewID.channelNumber
        var dCH0 = liveviewID.fCh0Data
        var dCH1 = liveviewID.fCh1Data
        var dCH2 = liveviewID.fCh2Data
        var uiDelta = liveviewID.recordingSampleDelay

        console.log(dCH0)
        console.log(dCH1)
        console.log(dCH2)
        console.log(uiDelta)

        //qwtPlot.displayNewSamples( iChannelNumber, dCH0, dCH1, dCH2, uiMainMeasurePeriod )
        qwtPlot.displayNewSamples( iChannelNumber, dCH0, dCH1, dCH2, uiDelta )
    }
    // Mise à jour grapical depth
    function vFPlotView_UpdateGraphicalView( uiGraphicalDepth ){
        console.log("= vFPlotView_UpdateGraphicalView =");
        // Mise à jour grapical depth
        qwtPlot.updateGraphicalDepth( uiGraphicalDepth );
    }
    // Signal de fin de refresh
    function vFEnfOfRefreshSignal(){
        bInitComponent = false

        console.log("= vFEnfOfRefreshSignal = updatePlotInformation2_init ");

        if( liveviewID.channelNumber == 1 ){
            // Propagation du signal
            qwtPlot.updatePlotInformation1( liveviewID.channelNumber,
                                            liveviewID.ch0Name,
                                            liveviewID.ch0Unit,
                                            ptColor[liveviewID.ch0Parameter] )
        }
        if( liveviewID.channelNumber == 2 ){
            // Propagation du signal
            qwtPlot.updatePlotInformation2( liveviewID.channelNumber,
                                            liveviewID.ch0Name,
                                            liveviewID.ch1Name,
                                            liveviewID.ch0Unit,
                                            liveviewID.ch1Unit,
                                            ptColor[liveviewID.ch0Parameter],
                                            ptColor[liveviewID.ch1Parameter] )
        }
        if( liveviewID.channelNumber == 3 ){
            // Propagation du signal
            qwtPlot.updatePlotInformation3( liveviewID.channelNumber,
                                            liveviewID.ch0Name,
                                            liveviewID.ch1Name,
                                            liveviewID.ch2Name,
                                            liveviewID.ch0Unit,
                                            liveviewID.ch1Unit,
                                            liveviewID.ch2Unit,
                                            ptColor[liveviewID.ch0Parameter],
                                            ptColor[liveviewID.ch1Parameter],
                                            ptColor[liveviewID.ch2Parameter] )
        }
    }
}
