import QtQuick 2.0
import QtQuick.Controls 2.12

import "license"

Item {
    TabBar {
        id: tabbar
        currentIndex: swipeview.currentIndex
        width: parent.width

        TabButton { text: "LGPLv3" }
        TabButton { text: "GPLv3" }
    }

    SwipeView {
        id: swipeview
        anchors.top: tabbar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        clip: true
        currentIndex: tabbar.currentIndex


        License_LGPLv3 {}
        License_GPLv3 {}
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
