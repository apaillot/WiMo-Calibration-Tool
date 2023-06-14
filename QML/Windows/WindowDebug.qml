import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12
import QtQuick.Extras 1.4


import "../Component"
import "../Windows"
import "../"

Window {
    id: windowDebug
    width: 480
    height: 360
    visible: true
    color: "#ffffff"


    function vFWindowDebug_AddALine( sAddString ){
        //testAreaDebug.text += sAddString
        testAreaDebug.text = sAddString + testAreaDebug.text
        //%%AP - 2021.12.17 - Ajout d'une limitation de taille de chaine
        if(testAreaDebug.text.length > 1512){
         testAreaDebug.text = testAreaDebug.text.slice(0,4096);
        }
    }
    Rectangle{
        //anchors.fill: view
        color: "#dddddd"
        border.color: "black"
        border.width: 1
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 165
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        ScrollView {
            id: view
            anchors.fill: parent

            TextArea {
                id: testAreaDebug
                text: "TextArea\n...\n...\n...\n...\n...\n...\n"
            }
        }
    }
    SwitchButtonPerso {
        id: switchNkeModeCpp
        width: 117
        text: "bNkeMode"
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15

        checked: tMainData.bNkeMode
        onClicked: {
            tMainData.bNkeMode = !tMainData.bNkeMode
        }
    }
    SwitchButtonPerso {
        id: switchMetroModeCpp
        width: 117
        text: "bMetroMode"
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: switchNkeModeCpp.bottom
        anchors.topMargin: 15

        checked: tMainData.bMetroMode
        onClicked: {
            tMainData.bMetroMode = !tMainData.bMetroMode
        }
    }
    SwitchButtonPerso {
        id: switchNkeProdModeCpp
        width: 117
        text: "bProdMode"
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: switchMetroModeCpp.bottom
        anchors.topMargin: 15

        checked: tMainData.bProdMode
        onClicked: {
            tMainData.bProdMode = !tMainData.bProdMode
        }
    }

}


