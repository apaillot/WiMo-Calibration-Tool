import QtQuick 2.0
import QtQuick.Controls 2.12

import "../components/Table"

Item {
    id: about_librairies

    Table {
        id: table
        anchors.fill: parent
        model: model_librairies
        rowMenu: menu_librairies

        function setColWidth() {
            if (about_librairies.width * 3/12 < 100)
                return 100
            else return about_librairies.width * 3/12
        }

        TableColumn {
            headerText: qsTr("Name")
            textRole: "name"
            columnWidth: table.setColWidth()
        }

        TableColumn {
            headerText: qsTr("License")
            textRole: "license"
            columnWidth: table.setColWidth()
        }

        TableColumn {
            headerText: qsTr("Source")
            textRole: "source"
            columnWidth: table.setColWidth() * 2
        }
    }

    ListModel {
        id: model_librairies

        ListElement { name:"Qt Quick";      license:"LGPLv3";               source:"https://github.com/qt/qtquickcontrols2" }
        // Qt5PrintSupport
        ListElement { name:"Qt SVG";        license:"LGPLv3";               source:"https://github.com/qt/qtsvg" }
        // Qt5OpenGL
        ListElement { name:"Qt Widgets";    license:"LGPLv3";               source:"https://github.com/qt/qtbase" }
        ListElement { name:"Qt Gui";        license:"LGPLv3";               source:"https://github.com/qt/qtbase" }
        ListElement { name:"Qt QML";        license:"LGPLv3";               source:"https://github.com/qt/qtquickcontrols2" }
        ListElement { name:"Qt Network";    license:"LGPLv3";               source:"https://github.com/qt/qtbase" }
        ListElement { name:"Qt SerialPort"; license:"LGPLv3";               source:"https://github.com/qt/qtserialport" }
        ListElement { name:"Qt Core";       license:"LGPLv3";               source:"https://github.com/qt/qtbase" }
        ListElement { name:"Qwt";           license:"LGPLv2.1";             source:"http://qwt.sf.nete" }
        ListElement { name:"QMLTable";      license:"BSD-3-Clause License"; source:"https://github.com/spuschhof/QMLTable" }
    }

    Menu {
        id: menu_librairies

        property var item
        property var source

        onItemChanged: menu_librairies.source = model_librairies.get(menu_librairies.item.row).source

        Action {
            text: "Go to source"
            onTriggered: Qt.openUrlExternally(menu_librairies.source)
        }
    }
}
