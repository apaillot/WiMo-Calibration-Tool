import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.15

import QtQuick.Dialogs 1.3
import QtQuick.Extras 1.4

MessageDialog {
    id: messageDialog
    signal closed()
    modality:Qt.WindowModal
    standardButtons: StandardButton.Ok
    onVisibleChanged: {
        console.log("onVisibleChanged")
        bBackgroundOpaciteVisible= this.visible
        if( !this.visible ){
            this.closed()
        }
    }
    onVisibilityChanged: {
        x = (window.width - width) / 2 + window.x
        y = (window.height - height) / 2 + window.y
    }
}

