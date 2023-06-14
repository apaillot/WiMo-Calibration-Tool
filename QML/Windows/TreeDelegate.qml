import QtQuick 2.0
import LibTreeDelegate 1.0

Item {
    Text {
        anchors.fill: parent
        color: styleData.textColor
        elide: styleData.elideMode
        //text: styleData.value.indentation + ": " + styleData.value.text
        text: styleData.value.text
    }
}
