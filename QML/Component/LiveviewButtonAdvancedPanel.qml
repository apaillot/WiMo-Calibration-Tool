import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property bool bHover: false

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal clicked()

    //==============================================================
    // QML
    //==============================================================
    id: m_liveviewButtonAdvancedPanel
    width: 165
    height: 46

    DropShadow {
        color: "#10000000"
        radius: 12
        transformOrigin: Item.Center
        verticalOffset: 0
        anchors.fill: liveViewConfigBtn
        samples: 30
        horizontalOffset: 0
        source: liveViewConfigBtn
    }
    Rectangle {
        id: liveViewConfigBtn
        anchors.fill: parent
        //x: 423
        //width: 165
        //height: 46
        color: (bHover)?"#32549C":"#e4e3e9"
        border.color: (bHover)?"#20545454":"#80545454"

        Text {
            id: liveViewConfigBtnTxt
            x: 22
            y: 34
            //color: "#ffffff"
            color: (bHover)?"#ffffff":"#000000"
            text: qsTr("Advanced settings")
            font.weight: Font.Normal
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 13
        }

        Image {
            id: liveViewConfigBtnImg
            x: 141
            y: 19
            width: 13
            height: 33
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
            source: (bHover)?"qrc:/Logo/LiveView/liveview-arrow-rightW.png":"qrc:/Logo/LiveView/liveview-arrow-right.png"
        }
    }
    MouseArea {
        anchors.fill: liveViewConfigBtn
        hoverEnabled: true
        onEntered: {
            bHover=true
        }
        onExited: {
            bHover=false
        }
        onClicked: {
            m_liveviewButtonAdvancedPanel.clicked();
        }
    }

}
