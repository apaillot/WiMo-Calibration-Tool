import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0
import QtQuick.Controls.Universal 2.0

Row {
    id: m_dataset

    property var color: btn_dataset.color
    property var currentIndex: cb_dataset.currentIndex
    property var currentText: cb_dataset.currentText

    width: parent.width

    spacing: width * 0.5/12

    ComboBox {
        id: cb_dataset
        width: m_dataset.width * 3/12

        enabled: !m_chart_timer.running

        model: ["Value", "Gross Value", "Scrutation", "Status", "Missing", "Pan", "Alarm"]
    }

    Button {
        id: btn_dataset

        property var color: color_dialog.color

        width: m_dataset.width * 7/12
        height: cb_dataset.height

        Universal.accent: color_dialog.currentColor
        highlighted: true

        onClicked: color_dialog.open()

        ColorDialog {
            id: color_dialog
            color: m_theme.accent

            onRejected: currentColor = color
        }
    }

    Button {
        id: btn_del

        width: m_dataset.width * 1/12
        height: cb_dataset.height

        enabled: !m_chart_timer.running

        icon.source: "qrc:/img/bin.png"
        icon.color: hovered ? m_theme.red : enabled ? m_theme.dark : m_theme.disabled

        onClicked: m_dataset.destroy()
    }
}

