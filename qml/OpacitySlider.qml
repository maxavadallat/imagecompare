import QtQuick 2.0
import "js/constants.js" as Const

Item {
    id: opacitySliderRoot

    width: Const.defaultOpacitySliderWidth
    height: Const.defaultOpacitySliderHeight

    property string side: "left"

    opacity: 1.0

    Behavior on opacity { NumberAnimation { duration: Const.defaultAnimDuration } }
    visible: opacity > 0.0

    Rectangle {
        anchors.fill: parent
        color: "#44123123"
    }

    Text {
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        text: {
            var value = sideViewController.side == "left" ? mainViewController.opacityLeft : mainViewController.opacityRight;
            return Math.round(value * 100) + "%";
        }

        color: Const.defaultFontColor
    }
}

