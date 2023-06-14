import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================
    // Variable de taille pour les logos capteurs
    property var ptLogoScale: {
        0:0.4, // S00 - Pressure static
        1:0.4, // S01 - Pressure keller
        2:0.4, // S02 - Temperature keller
        3:0.4, // S03 - Temperature
        4:0.4, // S04 - Conductivity

        5:0.4, // S05 - Turbidity
        6:0.4, // S06 - Oxygen concentration
        7:0.4, // S07 - Oxygen saturation
        8:0.4, // S08 - pH

        9:0.5, // S09 - Turner Chla
        10:0.5, // S10 - Turner phycoC
        11:0.5, // S11 - Turner phycoE
        12:0.5, // S12 - CDOM

        13:0.5, // S13 - Fluo Chla
        14:0.5, // S14 - PAR
        15:0.5, // S15 - Fluo phycoC
        16:0.5, // S16 - Fluo phycoE
        17:0.5, // S17 - Fluo CDOM
        18:0.5, // S18 - Fluo Trypto
        19:0.5, // S19 - Turner Trypto
        20:0.5, // S20 - Redox
        21:0.5, // S21 - Ammonium
        22:0.5, // S22 - Potassium
        23:0.5, // S23 - Nitrate
        24:0.5, // S24 - ISE-5
        25:0.5, // S25 - O2 temperature
        60:0.4,
        "unknow":0.4,
        "wiper":0.4,
        "cleaner":0.4
    }
    property var ptColor: {
        0:"#03A9F4", // S00 - Pressure static
        1:"#03A9F4", // S01 - Pressure keller
        2:"#F44336", // S02 - Temperature keller
        3:"#F44336", // S03 - Temperature
        4:"#26A69A", // S04 - Conductivity

        5:"#048B9A", // S05 - Turbidity
        6:"#193D8A", // S06 - Oxygen concentration
        7:"#193D8A", // S07 - Oxygen saturation
        8:"#B0C0E0", // S08 - pH

        9:"#8BC34A", // S09 - Turner Chla
        10:"#285C8C", // S10 - Turner phycoC
        11:"#9B0015", // S11 - Turner phycoE
        12:"#A1CF62", // S12 - CDOM

        13:"#8BC34A", // S13 - Fluo Chla
        14:"#FFB300", // S14 - PAR
        15:"#285C8C", // S15 - Fluo phycoC
        16:"#9B0015", // S16 - Fluo phycoE
        17:"#A1CF62", // S17 - Fluo CDOM
        18:"#CC3399", // S18 - Fluo Trypto
        19:"#CC3399", // S19 - Turner Trypto
        20:"#E23E42", // S20 - Redox
        21:"#1A4973", // S21 - Ammonium
        22:"#77807D", // S22 - Potassium
        23:"#339966", // S23 - Nitrate
        24:"#0099FF", // S24 - ISE-5
        25:"#FE1B00", // S25 - O2 temperature
        26:"#4F463F", // S26 - Crude oil Turner
        27:"#4F463F", // S27 - Crude oil nke
        60:"#B0C0E0",
        "unknow":"#DA4624",
        "wiper":"#A45883",
        "cleaner":"#590098",
        "disabled":"#c0c5ce"
    }

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    property int m_uiSensorChannelIndex: 1
    property bool bChannelActive: true
    property bool bBlocActive: true
    // Param
    property string m_sChannelName: "Conductivity"
    property string m_sChannelUnit: "mS/cm"
    property string m_sChannelSN: "00-00000"
    property string m_sChannelValue: ""

    //==============================================================
    // Déclaration des variables internes
    //==============================================================
    // Taille des blocs
    property int uiBlocWidthOffset: 60
    // Survol du bloc
    property bool bHover: false

    //==============================================================
    // Déclaration des signaux
    //==============================================================
    signal blocClicked()

    //==============================================================
    // QML
    //==============================================================
    width: 140
    height: 135

    Rectangle {
        id: ch0RectContainer
        anchors.fill: parent
        radius: 8
        //visible: (liveviewID.channelNumber>=1)?true:false
        DropShadow {
            color: "#15000000"
            radius: 12
            verticalOffset: 0
            anchors.fill: ch0RectW
            samples: 30
            horizontalOffset: 0
            source: ch0RectW
        }
        Rectangle {
            id: ch0RectW
            width: ch0RectContainer.width
            height: ch0RectContainer.height
            //width: 140
            //height: 135
            color: "#ffffff"
            radius: 8
            border.color: "#c0c5ce"
            border.width: (bHover)?2:0

            MouseArea {
                id: mouseAreaCh0
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    blocClicked()
//                    liveviewID.ch0Active = !liveviewID.ch0Active
                    //
//                    if( liveviewID.ch0Active )
//                        plotview.vFEnableChannel(0)
//                    else
//                        plotview.vFDisableChannel(0)
                }
                onEntered:{bHover=true}
                onExited:{bHover=false}
            }
            Rectangle {
                id: ch0Rect2
                x: 0
                y: 0
                width: 140
                height: 35
                color: bBlocActive?ptColor[m_uiSensorChannelIndex]:ptColor["disabled"]
                radius: 8

                Rectangle {
                    id: ch0Rect3
                    color: bBlocActive?ptColor[m_uiSensorChannelIndex]:ptColor["disabled"]
                    height: ch0Rect2.radius
                    anchors.bottom: ch0Rect2.bottom
                    anchors.left: ch0Rect2.left
                    anchors.right: ch0Rect2.right

                    Image {
                        id: ch0Logo
                        x: 41
                        y: -49
                        width: 58
                        height: 76
                        anchors.verticalCenterOffset: -13
                        anchors.horizontalCenterOffset: 0
                        transformOrigin: Item.Center
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        //source: "Logo/Sensor/logoS2.png"
                        source: "qrc:/Logo/Sensor/logoS"+m_uiSensorChannelIndex+".png"
                        fillMode: Image.PreserveAspectFit
                        scale: ptLogoScale[m_uiSensorChannelIndex]
                    }
                }
            }

            Text {
                id: ch0Name
                x: 0
                y: 78
                width: 140
                height: 20
                text: m_sChannelName
                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: false
                font.weight: Font.Light
                font.family: "open sans"
                color: bBlocActive?"#000000":ptColor["disabled"]
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 13
            }
            // Ch0 : Unité
            Text {
                id: ch0Unit
                x: 0
                y: 94
                width: 140
                height: 16
                //text: qsTr("(degC)")
                text: "("+m_sChannelUnit+")"
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: false
                font.pixelSize: 11
                font.weight: Font.Light
                font.family: "open sans"
                color: bBlocActive?"#000000":ptColor["disabled"]
            }
            // Ch0 : Serial number
            Text {
                id: ch0SN
                x: 9
                y: 113
                width: 140
                height: 20
                //text: qsTr("SN: 20-00001")
                text: qsTr("SN: "+m_sChannelSN)
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenterOffset: 0
                horizontalAlignment: Text.AlignHCenter
                font.bold: false
                font.pixelSize: 11
                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Light
                font.family: "open sans"
                color: bBlocActive?"#000000":ptColor["disabled"]
            }

            Text {
                id: ch0Value
                x: 2
                y: 42
                width: 140
                height: 33
                text: bBlocActive?m_sChannelValue:"--"
                //text: "--"
                color: bBlocActive?"#000000":ptColor["disabled"]
                anchors.horizontalCenterOffset: 0
                horizontalAlignment: Text.AlignHCenter
                font.bold: false
                font.pixelSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Light
                font.family: "open sans"
            }
        }
    }
}
