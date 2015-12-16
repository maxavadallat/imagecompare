#include <QQmlContext>

#include "mainwindow.h"
#include "compositorcontainer.h"
#include "compositor.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
CompositorContainer::CompositorContainer(QWidget* aParent)
    : QQuickWidget(aParent)
    , compositeWidth(0.0)
    , compositeHeight(0.0)
    , sourceCompositeWidth(0.0)
    , sourceCompositeHeight(0.0)
    , leftPressed(false)
    , rightPressed(false)
    , threshold(DEFAULT_COMPARE_THRESHOLD)
{
    // ...

    // Set Context Property - Compositor View Controller
    rootContext()->setContextProperty(COMPOSITOR_VIEW_CONTROLLER, this);
    // Register Compositor
    qmlRegisterType<Compositor>(DEFAULT_CUSTOM_COMPONENTS, 0, 1, DEFAULT_CUSTOM_COMPONENT_COMPOSITOR);
}

//==============================================================================
// Get Source Composite Width
//==============================================================================
qreal CompositorContainer::getSourceCompositeWidth()
{
    return sourceCompositeWidth;
}

//==============================================================================
// Set Source Composite Width
//==============================================================================
void CompositorContainer::setSourceCompositeWidth(const qreal& aWidth)
{
    // Check Composite Width
    if (sourceCompositeWidth != aWidth) {
        // Set Composite Width
        sourceCompositeWidth = aWidth;
        // Emit Source Composite Width Changed Signal
        emit sourceCompositeWidthChanged(sourceCompositeWidth);
    }
}

//==============================================================================
// Get Source Composite Height
//==============================================================================
qreal CompositorContainer::getSourceCompositeHeight()
{
    return sourceCompositeHeight;
}

//==============================================================================
// Set Source Composite Height
//==============================================================================
void CompositorContainer::setSourceCompositeHeight(const qreal& aHeight)
{
    // Check Source Composite Height
    if (sourceCompositeHeight != aHeight) {
        // Set Composite Height
        sourceCompositeHeight = aHeight;
        // Emit Source Composite Height Changed Signal
        emit sourceCompositeHeightChanged(sourceCompositeHeight);
    }
}

//==============================================================================
// Get Composite Width
//==============================================================================
qreal CompositorContainer::getCompositeWidth()
{
    return compositeWidth;
}

//==============================================================================
// Set Composite Width
//==============================================================================
void CompositorContainer::setCompositeWidth(const qreal& aWidth)
{
    // Check Composite Width
    if (compositeWidth != aWidth) {
        //qDebug() << "CompositorContainer::setCompositeWidth - aWidth: " << aWidth;

        // Set Composite Width
        compositeWidth = aWidth;
        // Emit Composite Width Changed Signal
        emit compositeWidthChanged(compositeWidth);
    }
}

//==============================================================================
// Get Composite Height
//==============================================================================
qreal CompositorContainer::getCompositeHeight()
{
    return compositeHeight;
}

//==============================================================================
// Set Composite Height
//==============================================================================
void CompositorContainer::setCompositeHeight(const qreal& aHeight)
{
    // Check Composite Height
    if (compositeHeight != aHeight) {
        //qDebug() << "CompositorContainer::setCompositeHeight - aHeight: " << aHeight;

        // Set Composite Height
        compositeHeight = aHeight;
        // Emit Composite Height Changed Signal
        emit compositeHeightChanged(compositeHeight);
    }
}

//==============================================================================
// Get Threshold
//==============================================================================
qreal CompositorContainer::getThreshold()
{
    return threshold;
}

//==============================================================================
// Set Threshold
//==============================================================================
void CompositorContainer::setThreshold(const qreal& aThreshold)
{
    // Get new Threshold
    qreal newThreshold = qBound(0.0, aThreshold, DEFAULT_COMPARE_THRESHOLD_MAX);

    // Check Compare Threshold
    if (threshold != newThreshold) {
        qDebug() << "CompositorContainer::setThreshold - newThreshold: " << newThreshold;
        // Set Compare Threshold
        threshold = newThreshold;
        // Emit Compare Threshold Changed Signal
        emit thresholdChanged(threshold);
    }
}

//==============================================================================
// Get Right Pressed
//==============================================================================
bool CompositorContainer::getRightPressed()
{
    return rightPressed;
}

//==============================================================================
// Set Right Pressed
//==============================================================================
void CompositorContainer::setRightPressed(const bool& aPressed)
{
    // Check Right Pressed
    if (rightPressed != aPressed) {
        // Set Right Pressed
        rightPressed = aPressed;
        // Emit Right Pressed Changed Signal
        emit rightPressedChanged(rightPressed);
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void CompositorContainer::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Mouse Button
        if (aEvent->button() == Qt::LeftButton) {
            // Set Pressed
            leftPressed = true;
            // Emit Mouse pressed Signal
            emit mousePressed(aEvent->pos());
        } else if (aEvent->button() == Qt::RightButton) {
            // Set Right Pressed
            setRightPressed(true);
            // Set Original Pos
            originalPosX = aEvent->pos().x();
            // Set Original Opacity
            originalThreshold = threshold;
        }
    }

    QQuickWidget::mousePressEvent(aEvent);
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void CompositorContainer::mouseMoveEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed
        if (leftPressed) {
            // Emit Mouse Moved Signal
            emit mouseMoved(aEvent->pos());
        } else if (rightPressed) {
            // Calculate Threshold Delta
            qreal delta = originalPosX - aEvent->pos().x();
            // Set Threshold
            setThreshold(originalThreshold - delta);
        }
    }

    QQuickWidget::mouseMoveEvent(aEvent);
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void CompositorContainer::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Mouse Button
        if (aEvent->button() == Qt::LeftButton) {
            // Reset Pressed
            leftPressed = false;
            // Emit Mouse Released Signal
            emit mouseReleased(aEvent->pos());
        } else if (aEvent->button() == Qt::RightButton) {
            // Reset Right Pressed
            setRightPressed(false);
            // Emit Right Mouse Button Released Signal
            emit mouseRightReleased(aEvent->pos());
        }
    }

    QQuickWidget::mouseReleaseEvent(aEvent);
}

//==============================================================================
// Mouse Douvle Click Event
//==============================================================================
void CompositorContainer::mouseDoubleClickEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Emit Double Clicked signal
        emit doubleClicked();
    }

    QQuickWidget::mouseDoubleClickEvent(aEvent);
}

//==============================================================================
// Destructor
//==============================================================================
CompositorContainer::~CompositorContainer()
{
    // ...
}
