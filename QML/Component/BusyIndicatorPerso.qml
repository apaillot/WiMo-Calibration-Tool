//import QtQuick 2.0
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4
//import QtQuick.Controls 2.13
import QtQuick 2.14
import QtQuick.Controls 2.15
/*
AnimatedImage {
    id: animation4
    x: 371
    y: 93
    width: 88
    height: 89
    source: "qrc:/Logo/Loader-gif.gif"
    fillMode: Image.PreserveAspectCrop
    autoTransform: false
    smooth: true
    visible: element.checked
}
*/

BusyIndicator {
    running: false
    onVisibleChanged: {
        if( visible ){
            running = true
        }
        else{
            running = false
        }
    }
}

/*
Image {
    source: "Logo/Loader.gif"
}
*/
/*
Rectangle {

}
*/
/*
import QtQuick 2.12
import QtQuick.Templates 2.12 as T
import QtQuick.Controls.Imagine 2.12
import QtQuick.Controls.Imagine.impl 2.12

T.BusyIndicator {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    topPadding: background ? background.topPadding : 0
    leftPadding: background ? background.leftPadding : 0
    rightPadding: background ? background.rightPadding : 0
    bottomPadding: background ? background.bottomPadding : 0

    topInset: background ? -background.topInset || 0 : 0
    leftInset: background ? -background.leftInset || 0 : 0
    rightInset: background ? -background.rightInset || 0 : 0
    bottomInset: background ? -background.bottomInset || 0 : 0

    contentItem: AnimatedImage {
        opacity: control.running ? 1 : 0
        playing: control.running || opacity > 0
        visible: control.running || opacity > 0
        Behavior on opacity { OpacityAnimator { duration: 250 } }

        source: Imagine.url + "busyindicator-animation"
        AnimatedImageSelector on source {
            states: [
                {"disabled": !control.enabled},
                {"running": control.running},
                {"mirrored": control.mirrored},
                {"hovered": control.hovered}
            ]
        }
    }

    background: NinePatchImage {
        source: Imagine.url + "busyindicator-background"
        NinePatchImageSelector on source {
            states: [
                {"disabled": !control.enabled},
                {"running": control.running},
                {"mirrored": control.mirrored},
                {"hovered": control.hovered}
            ]
        }
    }
}
*/
