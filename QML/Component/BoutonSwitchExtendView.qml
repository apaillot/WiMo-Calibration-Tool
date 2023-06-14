import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle{
    id: btnSwitchExtendClick
    property string channelName: "Conductivity"
    property color tColor: "#555555"
    //color:"#FF0000";
    signal clicked()
    property bool bChecked: ch0Enable.checked
    property double dResult: 0.0
    color: "#00FFFFFF"
    radius: 8

    ColumnLayout {

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0.1

        RowLayout {
            //anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.alignment: Qt.AlignCenter
            Rectangle{
                width: 12;
                height: 12;
                //anchors.verticalCenter: ch0Text.verticalCenter
                //anchors.top: ch0Text.top
                //%%AP - 2020.10.06 - Mis en commentaire
                //anchors.right: ch0Text.left
                //anchors.rightMargin: 5
                //anchors.verticalCenter: ch0Text.verticalCenter
                //color: "#555555"
                color: tColor
            }
            Text {
                id: ch0Text
                text: channelName
                font.pointSize: 10
            }
        }
        SwitchButtonPerso {
            id: ch0Enable
            width: 95
            height: 40
            Layout.alignment: Qt.AlignCenter
            //anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.top: ch0Text.bottom
            //anchors.topMargin: 5
            display: AbstractButton.IconOnly

            checked: true
            visible: ( m_uiChannelNumber > 0 )
            onClicked: {
                btnSwitchExtendClick.clicked()
                /*
                console.log(ch0Enable.checked)
                if( ch0Enable.checked ){
                    plotview.vFEnableChannel(0)
                }
                else{
                    plotview.vFDisableChannel(0)
                }*/
            }
        }
        Text {
            text: dResult
            font.pointSize: 10
            Layout.alignment: Qt.AlignCenter

        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
