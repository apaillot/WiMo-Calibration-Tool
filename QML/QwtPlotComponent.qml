import QtQuick 2.0
import QwtQuick2 1.0
//import LibLiveview 1.0

import "Component"
import "Windows"

QwtQuick2Plot {
    id: qwtPlotComponent
    anchors.fill: parent


    function vFDestroyQwtObjects(){
        qwtPlotComponent.destroy();
    }
  //  z:-1
  // Export du graphique en pdf
    function vFExport(){
        qwtPlotComponent.exportQwtPlot();
    }
    // Désactivation voie
    function vFDisableChannel( iChannel ){
        qwtPlotComponent.disableChannel(iChannel);
    }
    // Désactivation voie
    function vFEnableChannel( iChannel ){
        qwtPlotComponent.enableChannel(iChannel);
    }
    // Désactivation voie
    function vFClearGraphic(){
        qwtPlotComponent.clearGraphic();
    }
    function vFDisplayNewSamples( iChannelNumber, dCH0, dCH1, dCH2, uiMeasurePeriod ){
        qwtPlotComponent.displayNewSamples( iChannelNumber, dCH0, dCH1, dCH2, uiMeasurePeriod );
    }

    // Mise à jour grapical depth
    function vFPlotView_UpdateGraphicalView( uiGraphicalDepth ){
        // Mise à jour grapical depth
        qwtPlotComponent.updateGraphicalDepth( uiGraphicalDepth );
    }


    Component.onCompleted: {
        console.log("QwtQuick2Plot on completed : initQwtPlot")
        //qwtPlot.destroy()
        // Init
        initQwtPlot()
    }
}
