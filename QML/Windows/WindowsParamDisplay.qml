import QtQuick 2.14
import QtQuick.Window 2.14

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

//import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.1

import "../Component"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Configuration export")

    property bool bOneTime: false
    // Fonction display
    function vFDisplayWindow(){
        tMainEvent.clearParamWindows();
        timerDisplayCalibrationWindows.start();
    }
    // Timer pour le fonctionnement du submit bouton
    Timer {
        id: timerDisplayCalibrationWindows
        interval: 100;
        running: false;
        repeat: false;
        onTriggered: {
            //if( !bOneTime ){
            //    bOneTime = true
                tMainEvent.vFMainEventDisplayParamWindow()
            //}
        }
    }

    Rectangle{
        width:300
        height: 200
        color: "blue"
    }


    /*
    Component.completed: {
//        theModel.
    }
*/
    TreeView {
        id: treeViewTest
        //anchors.fill: parent
        model: tParamTreeModel
        width: parent.width
        height: parent.height - 100

        itemDelegate: TreeDelegate {}

        TableViewColumn {
            role: "parameters"
            title: "Parameters"
            width: 250
        }

        TableViewColumn {
            role: "raw"
            title: "Raw"
            width: 150
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
            fileDialogSaveConfig.open()
        }
    }

    //-----------------------------
    // FileDialog - Save config
    //-----------------------------
    FileDialog {
        id: fileDialogSaveConfig

        title: "Save config file"
        fileMode: FileDialog.SaveFile
        nameFilters: ["BIN (*.bin)"]

        onAccepted: {
            //console.log( "url : "+fileUrl )
            console.log("fileDialogExportToPdf.files = "+fileDialogSaveConfig.file)
            console.log("fileDialogExportToPdf.files = "+fileDialogSaveConfig.files)
            console.log("fileDialogExportToPdf.folder = "+fileDialogSaveConfig.folder)
            //console.log("sPdfCalibReportFilename = "+sPdfCalibReportFilename)

            var sFolder = fileDialogSaveConfig.folder.toString().replace("file:///","")
            var tFileName = fileDialogSaveConfig.file.toString().split('/');
            var sFileName = tFileName[ tFileName.length-1 ]
            console.log("sFileName = "+sFileName)
            console.log( "sFolder : "+sFolder )
            //var theurl = fileUrl.toString().replace("file:///","");
            //console.log("theurl : "+theurl)
            //tFactoryEvent.startWriteConfig( theurl );
            tFactoryEvent.bFStartSaveConfig( sFileName, sFolder );
        }
    }
}
