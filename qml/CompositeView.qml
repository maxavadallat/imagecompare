import QtQuick 2.0
import customcomponents 0.1
import "js/constants.js" as Const

Item {
    id: root

    width: 400
    height: 600

    Rectangle {
        anchors.fill: parent
        color: "gray"
    }

    Image {
        id: leftImage
        anchors.centerIn: parent

        width: sourceSize.width * mainViewController.zoomLevel
        height: sourceSize.height * mainViewController.zoomLevel

        Behavior on width { NumberAnimation { duration: Const.defaultAnimDuration } }
        Behavior on height { NumberAnimation { duration: Const.defaultAnimDuration } }

        anchors.horizontalCenterOffset: mainViewController.panPosX
        anchors.verticalCenterOffset: mainViewController.panPosY
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        fillMode: Image.Stretch
        smooth: false
        opacity: mainViewController.opacityLeft
        Behavior on opacity { NumberAnimation { duration: Const.defaultAnimDuration } }
        visible: opacity > 0.0

        source: {
            var fileName = mainViewController.currentFileLeft

            if (fileName.length > 0) {
                return "file://" + fileName;
            }

            return "";
        }
    }

    Image {
        id: rightImage
        anchors.centerIn: parent

        width: sourceSize.width * mainViewController.zoomLevel
        height: sourceSize.height * mainViewController.zoomLevel

        Behavior on width { NumberAnimation { duration: Const.defaultAnimDuration } }
        Behavior on height { NumberAnimation { duration: Const.defaultAnimDuration } }

        anchors.horizontalCenterOffset: mainViewController.panPosX
        anchors.verticalCenterOffset: mainViewController.panPosY
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        fillMode: Image.Stretch
        smooth: false
        opacity: mainViewController.opacityRight
        Behavior on opacity { NumberAnimation { duration: Const.defaultAnimDuration } }
        visible: opacity > 0.0

        source: {
            var fileName = mainViewController.currentFileRight;

            if (fileName.length > 0) {
                return "file://" + fileName;
            }

            return "";
        }
    }

    Compositor {
        id: compositor

        anchors.fill: parent

        currentFileLeft: mainViewController.currentFileLeft
        currentFileRight: mainViewController.currentFileRight

        zoomLevel: mainViewController.zoomLevel

        panPosX: mainViewController.panPosX
        panPosY: mainViewController.panPosY

        onStatusChanged: {
            console.log("compositor.onStatusChanged - status: " + compositor.status + " - operation: " + compositor.operation);

            // Check Last Operation
            if (compositor.operation === 1 && compositor.status === 0) {
                // Update Pan Positions
                mainViewController.updatePanPositions();
            }
        }

        onCompositeWidthChanged: {
            // Set Composite Width
            compositorViewController.compositeWidth = aCompositeWidth;
        }

        onCompositeHeightChanged: {
            // Set Composite Height
            compositorViewController.compositeHeight = aCompositeHeight;
        }

        onSourceCompositeWidthChanged: {
            // Set Source Composite Width
            compositorViewController.sourceCompositeWidth = aCompositeWidth;
        }

        onSourceCompositeHeightChanged: {
            // Set Source Composite Height
            compositorViewController.sourceCompositeHeight = aCompositeHeight;
        }
    }

    Column {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 32
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: "No Left Image"
            opacity: mainViewController.currentFileLeft == "" ? 1.0 : 0.0
            Behavior on opacity { NumberAnimation { duration: 200 } }
            visible: opacity > 0.0
            color: "#77FFFFFF"
        }

        Text {
            text: "No Right Image"
            opacity: mainViewController.currentFileRight == "" ? 1.0 : 0.0
            Behavior on opacity { NumberAnimation { duration: 200 } }
            visible: opacity > 0.0
            color: "#77FFFFFF"
        }
    }

    Image {
        id: compareImage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 84

        opacity: 0.5
        Behavior on opacity { NumberAnimation { duration: Const.defaultAnimDuration } }

        source: "qrc:/resources/images/icons/compare-orange-128.png"

        visible: (compositor.status === 1)

        Timer {
            repeat: true
            interval: Const.defaultAnimDuration * 2
            running: compareImage.visible
            onTriggered: {
                if (compareImage.opacity == 0.5) {
                    compareImage.opacity = 0.1;
                } else {
                    compareImage.opacity = 0.5;
                }
            }
        }
    }
}

