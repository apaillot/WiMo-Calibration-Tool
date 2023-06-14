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
    id: m_liveviewASButtonReturn
    width: 98
    height: 46

    //-----------------------------------
    // Bouton de retour
    //-----------------------------------
    DropShadow {
        color: "#10000000"
        radius: 12
        transformOrigin: Item.Center
        verticalOffset: 0
        anchors.fill: liveViewConfigReturnBtn
        samples: 30
        horizontalOffset: 0
        source: liveViewConfigReturnBtn
    }
    Rectangle {
        id: liveViewConfigReturnBtn
        anchors.fill: parent
        color: (bHover)?"#32549C":"#e4e3e9"
        border.color: (bHover)?"#20545454":"#80545454"
        Text {
            id: liveViewConfigReturnTxt
            x: 41
            y: 34
            color: (bHover)?"#ffffff":"#000000"
            text: qsTr("Return")
            font.family: "Open Sans"
            font.pixelSize: 13
            anchors.verticalCenterOffset: 0
            font.weight: Font.Normal
            anchors.verticalCenter: parent.verticalCenter
        }

        Image {
            id: liveViewConfigReturnBtnImg
            x: 12
            y: 19
            width: 13
            height: 33
            anchors.verticalCenterOffset: 2
            anchors.verticalCenter: parent.verticalCenter
            rotation: 180
            fillMode: Image.PreserveAspectFit
            source:  (bHover)?"qrc:/Logo/LiveView/liveview-arrow-rightW.png":"qrc:/Logo/LiveView/liveview-arrow-right.png"
        }
    }

    MouseArea {
        anchors.fill: liveViewConfigReturnBtn
        hoverEnabled: true
        onEntered: {
            bHover=true
        }
        onExited: {
            bHover=false
        }
        onClicked: {
            //console.log("CLICKED!");
            //liveViewStackView.pop()
            m_liveviewASButtonReturn.clicked()
        }
    }
}
