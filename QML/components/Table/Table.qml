import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.14
import QtQml.Models 2.2

/** @brief Simple and fast table implementation for QtQuick Components 2.
 *
 * The \c Table object provides an easy to use replacement for the QtQUick
 * Components 1 TableView.
 */
Flickable {
    id: table
    /** type:TableColumn Table column definitions for the table. */
    default property alias columns: columnCont.children

    /** type:int Width of the table header in pixel. */
    readonly property alias headerWidth: headerRow.width

    /** type:int Column spacin in pixel. */
    property alias columnSpacing: headerRow.spacing

    /** type:model Model used fot the table. */
    property alias model: tableView.model

    property Menu rowMenu: null
    property color tLightGray: "#D3D3D3"
    property color tLighterGray: "#E3E3E3"

    implicitHeight: tableView.contentHeight

    contentWidth: headerWidth
    contentHeight: headerRow.height + tableView.contentHeight
    boundsBehavior: Flickable.StopAtBounds

    ScrollBar.vertical: ScrollBar {}
    ScrollBar.horizontal: ScrollBar {}

    Column {
        anchors.fill: parent

        RowLayout {
            id: headerRow
            spacing: 0

            Repeater {
                id: headerRep
                model: columns

                delegate: Item {
                    id: headerCont
                    Layout.minimumWidth: columns[index].columnWidth
                    Layout.maximumWidth: columns[index].columnWidth
                    Layout.minimumHeight: 32
                    Layout.maximumHeight: 32

                    Component.onCompleted: {
                        var obj = columns[index].headerDelegate.
                            createObject(headerCont,
                                         {text: columns[index].headerText,
                                          column: index});

                        var tri = 0;

                        function sort() {
                            tri++;
                            tableView.model.vFSort(index, tri % 3);
                        }

                        obj.onClicked.connect(sort);
                    }
                }
            }
        }

        ListView {
            id: tableView

            width: headerRow.width
            height: contentHeight

            boundsBehavior: Flickable.StopAtBounds

            delegate: ItemDelegate {
                id: rowDelegate

                width: tableView.width

                background: Rectangle {
                    color: index % 2 === 0 ? hovered ? tLightGray : tLighterGray : hovered ? tLightGray : "transparent";
                }

                RowLayout {
                    id: tableRow
                    spacing: columnSpacing
                    anchors.fill: parent
                }

                Component.onCompleted: {
                    for(var i = 0; i < columns.length; i++) {
                        var obj = columns[i].cellDelegate.createObject(
                            tableRow,
                            {text: model[columns[i].textRole],
                             itemData: model[columns[i].dataRole],
                             column: i,
                             row: index,
                             cellWidth: Qt.binding(function() {return columns[this.column].columnWidth;})});
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        if (mouse.button == Qt.RightButton) {
                            rowMenu.item = tableView.model.index(index, 0);
                            rowMenu.popup();
                        }
                    }
                }
            }
        }
    }

    Item {
        id: columnCont
    }
}
