import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11
import "../Component"

//import QtQuick.Dialogs 1.3

Dialog {
//Window {
    id: interfaceUpdateWindow
    width: 480
    height: 280
    //background: "#efeef4"
    //color: "#efeef4"
    //title: qsTr("WiMo Interface updater")
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
    // Numéro de version interface
    property int m_uiInterfaceMajor: tMainData.uiInterfaceVersionMajor
    property int m_uiInterfaceMinor: tMainData.uiInterfaceVersionMinor
    property int m_uiInterfaceBuild: tMainData.uiInterfaceVersionBuild
    property string m_sInterfaceInfo:    interfaceUpdateData.sInterfaceInfo
    //property string m_sInterfaceVersion: interfaceUpdateData.sInterfaceVersion
    property string m_sUpdateVersion:  interfaceUpdateData.sUpdateVersion
    property int    m_uiDownloadProgression: interfaceUpdateData.uiDownloadProgression
    property int    m_uiUpdateProgression:   interfaceUpdateData.uiUploadProgression
    property bool   m_bUpdateBusy:    interfaceUpdateData.bUpdateBusy
    //property bool   m_bWindowShow:    interfaceUpdateData.bWindowShow


    //----------------------------------
    // Variable calculé
    //----------------------------------
    property string m_sInterfaceVersion: (m_uiInterfaceMajor!=0)?m_uiInterfaceMajor+"."+m_uiInterfaceMinor+"."+m_uiInterfaceBuild:""

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
        target:tInterfaceUpdateControler
        //-------------------------
        // Affichage de la fenêtre
        //-------------------------
        function onOpenInterfaceUpdateWindowSignal() {
            // Affichage de la fenêtre
            interfaceUpdateWindow.visible = true
        }
        //-------------------------
        // Signal pas de mise à jour capteur
        //-------------------------
        function onNoInterfaceUpdateSignal(){
            messageDialogNoInterfaceAvailableUpdate.open()
        }
        //-------------------------
        // Signal mise à jour capteur valide
        //-------------------------
        //function onInterfaceUpdateSuccessSignal(){
        function onUpdateSuccessSignal(){
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
            text: qsTr("Interface update detected")
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
            color: "transparent"

            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "transparent"
                Text {
                    text: qsTr("Interface info:")
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "transparent"
                Text {
                    color: "#575757"
                    text: m_sInterfaceVersion
                    font.pixelSize: 12
                    font.weight: Font.Light
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent"

            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "transparent"
                Text {
                    text: qsTr("Update info:")
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "transparent"
                Text {
                    color: "#575757"
                    text: qsTr(m_sUpdateVersion)
                    font.pixelSize: 12
                    font.weight: Font.Light
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Rectangle {
            width: parent.width
            height: uiRowHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent"

            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "transparent"
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
                color: "transparent"
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
            color: "transparent"

            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "transparent"
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
                color: "transparent"
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
            color: "transparent"
            Rectangle {
                width: parent.width * 30 / 100
                height: parent.height
                anchors.left: parent.left
                color: "transparent"
                Text {
                    id: text2
                    text: qsTr("Update interface")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                }
            }

            Rectangle {
                width: parent.width * 70 / 100
                height: parent.height
                anchors.right: parent.right
                color: "transparent"
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

        Rectangle {
            width: parent.width * 50 / 100
            height: parent.height
            color: "transparent"
            Button {
                text: qsTr("Start")
                height: parent.height * 70 / 100
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: !m_bUpdateBusy
                onClicked: {
                    if( enabled ){
                        tInterfaceUpdateControler.startButtonClicked();
                    }
                }
            }
        }

        Rectangle {
            width: parent.width * 50 / 100
            height: parent.height
            color: "transparent"
            Button {
                id: button
                text: qsTr("Skip")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 70 / 100
                enabled: !m_bUpdateBusy
                onClicked: {
                    interfaceUpdateWindow.close()
                    //messageDialogUpdateInterfaceError.open()
                }
            }
        }
    }
    //-------------------
    //
    //-------------------
    /*
    // MessageDialog - No interface update
    MessageDialogPerso {
        id: messageDialogNoAvailableUpdate
        title: "Interface update"
        text: "The interface already has the latest update."
        modality:Qt.WindowModal
    }
    */
    /*
    // MessageDialog - Update interface error
    MessageDialogPerso {
        id: messageDialogInterfaceUpdateSuccess
        title: "Interface update"
        text: "Interface updated succefully."
        onClosed: {
            interfaceUpdateWindow.close();
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
