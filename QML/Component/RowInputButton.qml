import QtQuick 2.0
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
import "Component"

Item {
    id: rootParent
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    property string sText: "Submit"
    property bool bBusy: false

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Fonction
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal clicked()

    //==============================================================
    // QML
    //==============================================================
    implicitWidth: 542
    implicitHeight: 40

    height: implicitHeight
    anchors.left: parent.left
    anchors.right: parent.right

    Rectangle {
        color: "#ffffff"
        height: parent.height
        //width: ( parent.width - 15 ) / 3
        width: ( parent.width )

        DropShadow {
            color: "#20000000"
            radius: 10
            verticalOffset: 0
            anchors.fill:btnButton
            samples: 25
            horizontalOffset: 0
            source: btnButton
        }
        Button {
            id: btnButton
            text: qsTr(sText)
            width: 100
            height: 28
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !bBusy
            onClicked: {
                rootParent.clicked();
            }
        }
        BusyIndicatorPerso {
            id: busyRefresh
            width: 36
            height: 37
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:   parent.verticalCenter
            visible: bBusy
        }
    }
}
