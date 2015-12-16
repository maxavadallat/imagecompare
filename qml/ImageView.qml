import QtQuick 2.0
import "js/constants.js" as Const

Item {
    id: imageViewRoot

    width: 400
    height: 600

    Connections {
        target: sideViewController

        onOpacityChanged: {
            // Check Side
            if (sideViewController.side === "left") {
                // Set Left Opacity
                mainViewController.opacityLeft = sideViewController.opacity;
            } else {
                // Set Right Opacity
                mainViewController.opacityRight = sideViewController.opacity;
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "gray"
    }

    Image {
        id: compareImage
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

        source: {
            var fileName = sideViewController.side === "left" ? mainViewController.currentFileLeft : mainViewController.currentFileRight;

            if (fileName.length > 0) {
                return "file://" + fileName;
            }

            return "";
        }
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 32

        text: "No Image"

        opacity: {
            var fileName = sideViewController.side === "left" ? mainViewController.currentFileLeft : mainViewController.currentFileRight;
            if (fileName === "") {
                return 1.0;
            }

            return 0.0;
        }

        Behavior on opacity { NumberAnimation { duration: 200 } }
        visible: opacity > 0.0
        color: "#77FFFFFF"
    }

    OpacitySlider {
        id: opacitySlider
        anchors.left: sideViewController.side === "left" ? parent.left : undefined
        anchors.leftMargin: 32
        anchors.right: sideViewController.side === "right" ? parent.right : undefined
        anchors.rightMargin: 32
        anchors.verticalCenter: parent.verticalCenter
        opacity: sideViewController.rightPressed ? 1.0 : 0.0
    }

    Rectangle {
        id: dropIndicator
        anchors.fill: parent
        color: "transparent"
        border.width: 2
        border.color: "orange"
        visible: dropArea.containsDrag
    }

    DropArea {
        id: dropArea
        anchors.fill: parent

        onDropped: {
            //console.log("dropArea.onDropped - text: " + drop.urls);

            // Get File Name
            var fileName = drop.urls[0];

            // Check File Name
            if (fileName.indexOf("file://") === 0) {

                drop.accept();

                // Adjust File Name
                fileName = fileName.slice(7, fileName.length);

                //console.log("dropArea.onDropped - fileName: " + fileName);

                // Check Side
                if (side === "left") {
                    // Set Left File
                    mainViewController.currentFileLeft = fileName;
                } else {
                    // Set Left File
                    mainViewController.currentFileRight = fileName;
                }
            }
        }
    }
}

