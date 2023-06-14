import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.1

/** @brief Default delegate used for \c Table cells.
 *
 * If you create your own delegate, not inhereting from \c TableCellDelegate,
 * add the \c column, \c row, \c itemData, \c cellWidth and \c useTableFont
 * properties to your delegate's base item and set \c Layout.minimumWidth and
 * \c Layout.maximumWidth to \c cellWidth.
 *
 * If you want to use the table's default font, the base item of delegate also
 * needs a \c font property.
 */
Label {
    /** Index of the table column. Set by the \c Table. */
    property int column: 0

    /** Index of the table row. Set by the \c Table. */
    property int row: 0

    /** Data of the table call. Set by the \c Table.
     *
     * The itemData field is filled with the model data for the role provided
     * with \c TableColumn::dataRole.
     */
    property string itemData: ""

    /** Width of the table cell. Set by the \c Table. */
    property int cellWidth: 0

    elide: Text.ElideRight
    Layout.minimumWidth: cellWidth
    Layout.maximumWidth: cellWidth
    clip: true
    leftPadding: 5
}
