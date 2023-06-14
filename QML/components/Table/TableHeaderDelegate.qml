import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.1

import "../"

/** Default delegate used for \c Table header cells.
 *
 * If you create your own delegate, not inhereting from \c TableHeaderDelegate,
 * add the \c column and \c useHeaderFont properties to your delegate's base
 * item.
 *
 * If you want to use the table's default header font, the base item of delegate
 * also needs a \c font property.
 */
ItemDelegate {
    id: m_header_delegate
    /** Setting whether the cell uses the table's default header font.
     *
     * Set to \c false if you want your delegate to use its own font.
     */
    property bool useHeaderFont: true

    /** Index of the table column. Set by the \c Table. */
    property int column: 0

    anchors.fill: parent
    font.bold: true

    Border {
        all: true
    }

    Border {
        _right: true
        //color: m_theme.accent
    }

    ToolTip {
        text: m_header_delegate.text
        visible: m_header_delegate.hovered
        delay: 500
    }
}
