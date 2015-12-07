import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: viewerRoot

    width: 600
    height: 400

    color: Const.defaultViewBackgroundColor

    // Viewer Image
    Image {
        id: viewerImage
        anchors.centerIn: parent

        width: sourceSize.width * viewerViewController.zoomLevel
        height: sourceSize.height * viewerViewController.zoomLevel

        Behavior on width { NumberAnimation { duration: Const.defaultAnimDuration } }
        Behavior on height { NumberAnimation { duration: Const.defaultAnimDuration } }

        anchors.horizontalCenterOffset: viewerViewController.panPosX
        anchors.verticalCenterOffset: viewerViewController.panPosY

        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter

        fillMode: Image.PreserveAspectFit

        asynchronous: true

        opacity: viewerImage.status === Image.Ready ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: Const.defaultAnimDuration } }
        visible: opacity > 0.0

        source: {
            if (viewerViewController.currentFile === "") {
                return "";
            }

            return "file://" + viewerViewController.currentFile;
        }

        onSourceSizeChanged: {
            // Set Image Source Width
            viewerViewController.imageSourceWidth = viewerImage.sourceSize.width;
            // Set Image Source Height
            viewerViewController.imageSourceHeight = viewerImage.sourceSize.height;
        }

        onWidthChanged: {
            // Set Image Width
            viewerViewController.imageWidth = viewerImage.width;
        }

        onHeightChanged: {
            // Set Image Height
            viewerViewController.imageHeight = viewerImage.height;
        }
    }

    // Loading Text
    Text {
        id: loadingText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Const.defaultFontSize
        color: Const.defaultFontColor
        visible: opacity > 0.0
        opacity: viewerImage.status !== Image.Ready ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: 200 } }
        text: qsTr("Loading...")
    }
}

