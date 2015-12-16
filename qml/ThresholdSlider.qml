import QtQuick 2.0
import "js/constants.js" as Const

Item {
    id: thresholdSliderRoot

    width: Const.defaultThresholdSliderWidth
    height: Const.defaultThresholdSliderHeight

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
            var value = mainViewController.threshold;
            return qsTr("Threshold: ") + Math.round(value);
        }

        color: Const.defaultFontColor
    }
}
