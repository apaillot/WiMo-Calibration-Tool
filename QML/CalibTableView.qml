import QtQuick 2.0

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.15


TableView {
    //==============================================================
    // Déclaration des constantes
    //==============================================================

    //==============================================================
    // Déclaration des variables sortantes
    //==============================================================
    //property int currentRow: tableView.currentRow

    //==============================================================
    // Déclaration des variables entrantes
    //==============================================================
    property int m_uiID: 0
    property var m_tCalibPoint:          ListModel{}

    //==============================================================
    // Déclaration des variables internes
    //==============================================================

    //==============================================================
    // Déclaration des signaux
    //==============================================================

    //==============================================================
    // Déclaration des fonctions
    //==============================================================
    function uiFGetCurrentRow(){
        return( tableView.currentRow );
    }

    //==============================================================
    // QML
    //==============================================================

    id: tableView

    x: 16
    y: 49
    //%%AP - 2020.10.05 -
    width: 400
    height: 245

    //highlightOnFocus: true
    //backgroundVisible: true
    TableViewColumn {
        role: "point"
        title: qsTr("Point")
        width: tableView.width / 3 - 1
        //width: parent.width / 3 - 1
        //width: 100
    }
    TableViewColumn {
        role: "physical"
        //title: qsTr("Physical")
        title: qsTr("Standard")
        width: tableView.width / 3 - 1
        //width: parent.width / 3 - 1
        //width: 100
    }
    TableViewColumn {
        role: "raw"
        //title: ( bO2 )?"Corrected":"Raw"
        /*
        title: (  ( tableView.m_uiID == 6 )
               || ( tableView.m_uiID == 7 )
               || ( tableView.m_uiID == 25 ) )?qsTr("Corrected"):qsTr("Raw")
        */
        title: qsTr("Measured")
        width: tableView.width / 3 - 1
        //width: parent.width / 3 - 1
        //width: 100
    }
    //model: calibPoint
    model: m_tCalibPoint
    // Double clique
    onDoubleClicked:{
        // Si erreur de sélection de ligne
        if( tableView.currentRow == -1 ) return;

        //console.log( calibPoint.data(calibPoint.index(tableView.currentRow,0), 258) )
        //popTestPhy.text = calibPoint.data(calibPoint.index(tableView.currentRow,0), 258)
        //popTestRaw.text = calibPoint.data(calibPoint.index(tableView.currentRow,0), 259)
        console.log( m_tCalibPoint.data(m_tCalibPoint.index(tableView.currentRow,0), 258) )
        popTestPhy.text = m_tCalibPoint.data(m_tCalibPoint.index(tableView.currentRow,0), 258)
        popTestRaw.text = m_tCalibPoint.data(m_tCalibPoint.index(tableView.currentRow,0), 259)
        popTestID.bCalibPointModify = true;
        popTestID.open();
    }



} // End tableview

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
