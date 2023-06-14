import QtQuick 2.0
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0

Item {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    // Parameter information
    property string m_sFormula:         ""
    property string m_sCalibrationDate: ""

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // QML
    //==============================================================
    id: m_dashboardInformationBlocID
    implicitWidth: 542
    implicitHeight: 238
    anchors.horizontalCenter: parent.horizontalCenter

    height: (34+(40*2)+(1*1))

    //-------------------------------------------
    // Bloc Dashboard - Information
    //-------------------------------------------
    // Bloc Dashboard - Information
    DropShadow {
        color: "#20000000"
        radius: 12
        verticalOffset: 0
        anchors.fill: m_dashboardInfoID
        samples: 30
        horizontalOffset: 0
        source: m_dashboardInfoID
        //visible: ( bGenericSensor || bWiper || bO2 )
    }
    Rectangle {
        id: m_dashboardInfoID
        color: "#ffffff"
        anchors.fill: parent

        Column{
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            H3BlocTitle{
                sH3Text: "Information"
            }
            //---------------
            // Formula
            //---------------
            RowText{
                sTextDesc: "Formula"
                sLogo: "qrc:/Logo/Calibration/calib-formula.png"
                sTextParam: m_sFormula
                uiLogoHeight: 20
            }
            SectionBlocLine{}
            //---------------
            // Calibration date
            //---------------
            RowText{
                sTextDesc: "Calibration date"
                sLogo: "qrc:/Logo/Calibration/calib-date.png"
                sTextParam: m_sCalibrationDate
                uiLogoHeight: 20
            }
        }
    }
}
