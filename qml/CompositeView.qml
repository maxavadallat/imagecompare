import QtQuick 2.0
import customcomponents 0.1
import "js/constants.js" as Const

Item {
    id: root

    width: 400
    height: 600

    Connections {
        target: compositorViewController

        onThresholdChanged: {
            // Set Threshold
            mainViewController.threshold = compositorViewController.threshold;
        }
    }


    Rectangle {
        anchors.fill: parent
        color: "gray"
    }

    Item {
        id: leftImageContainer
        anchors.fill: parent
        //anchors.margins: 10

        clip: true

        visible: true

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

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 1
            border.color: "#77000777"
        }
    }

    Item {
        id: rightImageContainer
        anchors.fill: parent
        //anchors.margins: 10

        clip: true

        visible: true

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

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 1
            border.color: "#77777000"
        }
    }

    ShaderEffect {
        id: compareShader
        anchors.fill: parent

        width: Math.min(root.width, compositor.compositeWidth);
        height: Math.min(root.height, compositor.compositeHeight);

        Behavior on width { NumberAnimation { duration: Const.defaultAnimDuration } }
        Behavior on height { NumberAnimation { duration: Const.defaultAnimDuration } }

        property variant leftImageSource: ShaderEffectSource {
            sourceItem: leftImageContainer
            hideSource: mainViewController.hideSources
        }

        property variant rightImageSource: ShaderEffectSource {
            sourceItem: rightImageContainer
            hideSource: mainViewController.hideSources
        }

        property real threshold: mainViewController.threshold

        visible: (leftImage.source != "") && (rightImage.source != "")

        fragmentShader: "
            uniform sampler2D leftImageSource;
            uniform sampler2D rightImageSource;
            varying highp vec2 qt_TexCoord0;
            uniform float threshold;
            void main() {
                // Just Draw Source
                //gl_FragColor = texture2D(rightImage, qt_TexCoord0);
                // Gradient
                //gl_FragColor = vec4(qt_TexCoord0.x, qt_TexCoord0.y, qt_TexCoord0.x, 1.0);

                lowp vec4 leftTexture = texture2D(leftImageSource, qt_TexCoord0);
                lowp vec4 rightTexture = texture2D(rightImageSource, qt_TexCoord0);

                lowp vec4 result;

                result.r = (abs(leftTexture.r - rightTexture.r) - (threshold * 0.01)) * 10.0;
                result.g = (abs(leftTexture.g - rightTexture.g) - (threshold * 0.01)) * 10.0;
                result.b = (abs(leftTexture.b - rightTexture.b) - (threshold * 0.01)) * 10.0;
                result.a = 0.5;

                gl_FragColor = result;
            }
        "

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 1
            border.color: "#77000077"
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

        showGrid: mainViewController.showGrid

        zoomLevelIndex: mainViewController.zoomLevelIndex

        onStatusChanged: {
            //console.log("compositor.onStatusChanged - status: " + compositor.status + " - operation: " + compositor.operation);

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

    ThresholdSlider {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 72
        opacity: compositorViewController.rightPressed ? 1.0 : 0.0
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

