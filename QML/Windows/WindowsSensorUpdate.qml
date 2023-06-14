import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11
import "../Component"

Dialog {
//Window {
    id: sensorUpdateWindow
    width: 480
    height: 280
    //background: "#efeef4"
    //color: "#efeef4"
    //title: qsTr("WiMo Sensor updater")
    anchors.centerIn: Overlay.overlay

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    visible: false

    //=============================================================
    // Parameters
    //=============================================================

    //----------------------------------
    // Input parameter
    //----------------------------------
    property string m_sSensorInfo:    sensorUpdateData.sSensorInfo
    property string m_sSensorVersion: sensorUpdateData.sSensorVersion
    property string m_sUpdateVersion: sensorUpdateData.sUpdateVersion
    property int    m_uiDownloadProgression: sensorUpdateData.uiDownloadProgression
    property int    m_uiUpdateProgression:   sensorUpdateData.uiUploadProgression
    property bool   m_bUpdateBusy:    sensorUpdateData.bUpdateBusy
    //property bool   m_bWindowShow:    sensorUpdateData.bWindowShow

    //----------------------------------
    // Debug parameter
    //----------------------------------

    //----------------------------------
    // Propriété interne QML
    //----------------------------------
    // Property
    property int uiRowHeight: 30

    //=============================================================
    // Connection
    //=============================================================
    Connections {
        target:tSensorUpdateControler
        //-------------------------
        // Affichage de la fenêtre
        //-------------------------
        function onOpenSensorUpdateWindowSignal() {
            // Affichage de la fenêtre
            sensorUpdateWindow.visible = true
        }
        //-------------------------
        // Signal pas de mise à jour capteur
        //-------------------------
        function onNoSensorUpdateSignal(){
            messageDialogNoSensorAvailableUpdate.open()
        }
        //-------------------------
        // Signal mise à jour capteur valide
        //-------------------------
        function onSensorUpdateSuccessSignal(){
            messageDialogSensorUpdateSuccess.open()
        }
    }

    Rectangle {
        id: windowsMenu
        x: 0
        y: 0
        width: parent.width
        height: 52
        color: "#132644"

        Text {
            id: text3
            text: qsTr("Sensor update detected")
            color: "#ffffff"
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 15
            font.weight: Font.DemiBold
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle{
        color:"#ffffff"
        //anchors.top: column4.top
        anchors.top: windowsMenu.bottom
        anchors.bottom: column4.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
    Column {
        id: column4
        width: parent.width * 85 / 100

        anchors.topMargin: 9
        anchors.top: windowsMenu.bottom
        anchors.bottom: footerRow.top
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "Transparent"
            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "Transparent"
                Text {
                    text: qsTr("Sensor info:")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "Transparent"
                Text {
                    color: "#575757"
                    text: m_sSensorInfo+" - "+m_sSensorVersion
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.Light
                }
            }
        }
        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "Transparent"
            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "Transparent"
                Text {
                    text: qsTr("Update info:")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "Transparent"
                Text {
                    color: "#575757"
                    text: m_sUpdateVersion
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.Light
                }
            }
        }
        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "Transparent"
            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "Transparent"
                Text {
                    text: qsTr("Status:")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "Transparent"
                Text {
                    color: "#575757"
                    //text: qsTr("Ready")
                    text: (m_uiDownloadProgression==0)?qsTr("Ready"):(m_uiDownloadProgression<100)?qsTr("Downloading..."):(m_uiUpdateProgression<100)?qsTr("Updating..."):qsTr("Finished")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.Light
                }
            }
        }
        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "Transparent"

            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "Transparent"
                Text {
                    id: text1
                    text: qsTr("Download")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "Transparent"
                ProgressBar {
                    id: progressBar
                    width: parent.width
                    height: 22
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    to: 100
                    value: m_uiDownloadProgression
                }
            }
        }

        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "Transparent"
            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                Text {
                    id: text2
                    text: qsTr("Update sensor")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "Transparent"
                ProgressBar {
                    id: progressBar1
                    width: parent.width
                    height: 22
                    anchors.verticalCenter: parent.verticalCenter
                    value: m_uiUpdateProgression
                    anchors.horizontalCenter: parent.horizontalCenter
                    to: 100
                }
            }
        }
    }

    //--------------------------
    // Footer
    //--------------------------
    Row {
        id: footerRow
        height: 50
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 80 / 100
        //color: "Transparent"
        Rectangle {
            width: parent.width * 50 / 100
            height: parent.height
            color: "Transparent"
            Button {
                text: qsTr("Start")
                height: parent.height * 70 / 100
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: !m_bUpdateBusy
                onClicked: {
                    if( enabled ){
                        tSensorUpdateControler.startButtonClicked();
                    }
                }
            }
        }

        Rectangle {
            width: parent.width * 50 / 100
            height: parent.height
            color: "Transparent"
            Button {
                id: button
                text: qsTr("Skip")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 70 / 100
                enabled: !m_bUpdateBusy
                onClicked: {
                    sensorUpdateWindow.close()
                    //messageDialogUpdateInterfaceError.open()
                }
            }
        }
    }
    //-------------------
    //
    //-------------------
    /*
    // MessageDialog - No sensor update
    MessageDialogPerso {
        id: messageDialogNoAvailableUpdate
        title: "Sensor update"
        text: "The sensor already has the latest update."
        modality:Qt.WindowModal
    }
    */
    /*
    // MessageDialog - Update interface error
    MessageDialogPerso {
        id: messageDialogSensorUpdateSuccess
        title: "Sensor update"
        text: "Sensor updated succefully."
        onClosed: {
            sensorUpdateWindow.close();
        }
    }*/
    /*
    MessageDialog {
        id: messageDialogUpdateInterfaceError
        title: "Update interface"
        text: "Update interface error"
    }
    */
}
