import QtQuick 2.0
import "js/constants.js" as Const

Item {
    id: imageViewRoot

    width: 400
    height: 600

    Rectangle {
        anchors.fill: parent
        color: side === "left" ? "#44777700" : "#44007777"
    }

    Image {
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
            var fileName = side === "left" ? mainViewController.currentFileLeft : mainViewController.currentFileRight;

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
            var fileName = side === "left" ? mainViewController.currentFileLeft : mainViewController.currentFileRight;
            if (fileName == "") {
                return 1.0;
            }

            return 0.0;
        }
        Behavior on opacity { NumberAnimation { duration: 200 } }
        visible: opacity > 0.0
        color: "#77FFFFFF"
    }
}

