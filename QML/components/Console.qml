import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12
import QtQuick.Window 2.12

import CustomComponents 1.0

Item {
    id: w_console
    clip: true

    TConsole {
        id: t_console
    }

    Row {
        id: r_btns

        anchors.right: parent.right

        Button {
            id: btn_filter

            width: 36
            height: 36

            icon.width: parent.width
            icon.height: parent.height
            icon.source: "qrc:/img/filter.png"
            //icon.color: hovered ? m_theme.accent : m_theme.dark

            flat: true

            onClicked: menu_filter_console.open()

            Menu {
                id: menu_filter_console

                y: parent.height

                Menu {
                    title: "Format"

                    CheckBox {
                        id: cb_date
                        text: "Date"
                        checked: settingsController.dateFilter
                        onClicked: settingsController.dateFilter = checked
                    }

                    CheckBox {
                        id: cb_time
                        text: "Time"
                        checked: settingsController.timeFilter
                        onClicked: settingsController.timeFilter = checked
                    }

                }

                Menu {
                    title: "Type"

                    CheckBox {
                        id: cb_req
                        text: t_console.REQ
                        checked: settingsController.reqFilter
                        onClicked: settingsController.reqFilter = checked
                    }

                    CheckBox {
                        id: cb_ack
                        text: t_console.ACK
                        checked: settingsController.ackFilter
                        onClicked: settingsController.ackFilter = checked
                    }

                    CheckBox {
                        id: cb_err
                        text: t_console.ERR
                        checked: settingsController.errFilter
                        onClicked: settingsController.errFilter = checked
                    }

                    CheckBox {
                        id: cb_info
                        text: t_console.INFO
                        checked: settingsController.infoFilter
                        onClicked: settingsController.infoFilter = checked
                    }

                    CheckBox {
                        id: cb_f1
                        text: t_console.F1
                        checked: settingsController.f1Filter
                        onClicked: settingsController.f1Filter = checked
                    }

                    CheckBox {
                        id: cb_f4
                        text: t_console.F4
                        checked: settingsController.f4Filter
                        onClicked: settingsController.f4Filter = checked
                    }

                    CheckBox {
                        id: cb_fc
                        text: t_console.FC
                        checked: settingsController.fcFilter
                        onClicked: settingsController.fcFilter = checked
                    }
                }

                Menu {
                    title: "Command"

                    CheckBox {
                        id: cb_set_mode
                        text: "SET MODE (0x0b)"
                        checked: settingsController.x0BFilter
                        onClicked: settingsController.x0BFilter = checked
                    }

                    CheckBox {
                        id: cb_flags
                        text: "FLAGS (0x10)"
                        checked: settingsController.x10Filter
                        onClicked: settingsController.x10Filter = checked
                    }

                    CheckBox {
                        id: cb_get_address
                        text: "ADDRESS (0x11)"
                        checked: settingsController.x11Filter
                        onClicked: settingsController.x11Filter = checked
                    }

                    CheckBox {
                        id: cb_req_id
                        text: "REQ ID (0x12)"
                        checked: settingsController.x12Filter
                        onClicked: settingsController.x12Filter = checked
                    }
                }
            }
        }

        Button {
            id: btn_open

            width: 36
            height: 36

            icon.width: parent.width
            icon.height: parent.height
            icon.source: "qrc:/img/file.png"
            //icon.color: hovered ? m_theme.accent : m_theme.dark

            flat: true

            onClicked: t_console.vFOpenFile()
        }

        Button {
            id: btn_save

            width: 36
            height: 36

            enabled: false

            icon.width: parent.width
            icon.height: parent.height
            icon.source: "qrc:/img/save.png"
            //icon.color: hovered ? m_theme.accent : "grey"
            icon.color: hovered ? "transparent" : "grey"

            flat: true
        }

        Button {
            id: btn_sync

            width: 36
            height: 36

            icon.width: parent.width
            icon.height: parent.height
            icon.source: "qrc:/img/sync.png"
            //icon.color: hovered ? m_theme.accent : m_theme.dark

            flat: true

            onClicked: t_console.vFRefresh()
        }

        Button {
            id: btn_del

            width: 36
            height: 36

            icon.width: parent.width
            icon.height: parent.height
            icon.source: "qrc:/img/bin.png"
            //icon.color: hovered ? m_theme.accent : m_theme.dark

            flat: true

            onClicked: t_console.vFDeleteFile()
        }
    }

    ListView {
        id: list_view
        clip: true

        anchors.top: r_btns.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        model: t_console.lines

        ScrollBar.vertical: ScrollBar {}

        delegate: Text {
            clip: true

            width: list_view.width
            height: list_view.filter(modelData) ? contentHeight : 0

            text: list_view.format(modelData)
            //color: modelData.split(',')[1] === t_console.ERR ? m_theme.red : m_theme.dark
            wrapMode: Text.WordWrap
        }

        function format(debugline) {

            var datetime = debugline.split(',')[0];
            var type = debugline.split(',')[1];
            var value = debugline.split(',')[2];

            var date = datetime.split(' ')[0];
            var time = datetime.split(' ')[1];

            var line = "";

            if (cb_date.checked)
                line += date + " "
            if (cb_time.checked)
                line += time + " "
            if (cb_date.checked | cb_time.checked)
                line += ": "

            line += type + "," + value

            return line;
        }

        function filter(debugline) {
            if (debugline.split(',').length < 3)
                return false;

            var type = debugline.split(',')[1];
            var value = debugline.split(',')[2];

            var cmd = value.substring(1,3);

            /******************************** TYPE ********************************/
            if (!cb_req.checked & type === t_console.REQ)
                return false;

            if (!cb_ack.checked & type === t_console.ACK)
                return false;

            if (!cb_err.checked & type === t_console.ERR)
                return false;

            if (!cb_info.checked & type === t_console.INFO)
                return false;

            if (!cb_f1.checked & type === t_console.F1)
                return false;

            if (!cb_f4.checked & type === t_console.F4)
                return false;

            if (!cb_fc.checked & type === t_console.FC)
                return false;
            /**********************************************************************/

            /********************************* CMD ********************************/
            if (!cb_set_mode.checked) {
                if (type === t_console.REQ && cmd === "4b")
                    return false;
                else if (type === t_console.ACK && cmd === "8b")
                    return false;
            }

            if (!cb_flags.checked) {
                if (type === t_console.REQ && cmd === "50")
                    return false;
                else if (type === t_console.ACK && cmd === "90")
                    return false;
            }

            if (!cb_get_address.checked) {
                if (type === t_console.REQ && cmd === "51")
                    return false;
                else if (type === t_console.ACK && cmd === "91")
                    return false;
            }

            if (!cb_req_id.checked) {
                if (type === t_console.REQ && cmd === "52")
                    return false;
                else if (type === t_console.ACK && cmd === "92")
                    return false;
            }
            /**********************************************************************/

            return true;
        }
    }
}
