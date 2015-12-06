#include <QQmlContext>
#include <QDebug>

#include "sideimagecontainer.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
SideImageContainer::SideImageContainer(QWidget* aParent)
    : QQuickWidget(aParent)
    , side("")
    , opacity(1.0)
    , leftPressed(false)
    , rightPressed(false)
{
    // ...

    // Set Controller
    rootContext()->setContextProperty(SIDE_VIEW_CONTROLLER, this);

    // Set Accepted Buttons

}

//==============================================================================
// Get Side
//==============================================================================
QString SideImageContainer::getSide()
{
    return side;
}

//==============================================================================
// Set Side
//==============================================================================
void SideImageContainer::setSide(const QString& aSide)
{
    // Check Side
    if (side != aSide) {
        // Set Side
        side = aSide;
        // Emit Side Changed Signal
        emit sideChanged(side);

        // Set Conteext Property
        rootContext()->setContextProperty(CONTEXT_PROPERTY_SIDE, side);
    }
}

//==============================================================================
// Get Opacity
//==============================================================================
qreal SideImageContainer::getOpacity()
{
    return opacity;
}

//==============================================================================
// Set Opacity
//==============================================================================
void SideImageContainer::setOpacity(const qreal& aOpacity)
{
    // Get New Opacity
    qreal newOpacity = qBound(0.0, aOpacity, 1.0);

    // Check Opacity
    if (opacity != newOpacity) {
        // Set Opacity
        opacity = newOpacity;
        // Emit Opacity Changed Signal
        emit opacityChanged(opacity);
    }
}

//==============================================================================
// Get Right Pressed
//==============================================================================
bool SideImageContainer::getRightPressed()
{
    return rightPressed;
}

//==============================================================================
// Set Right Pressed
//==============================================================================
void SideImageContainer::setRightPressed(const bool& aPressed)
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
void SideImageContainer::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Button
        if (aEvent->button() == Qt::LeftButton) {
            // Set Pressed
            leftPressed = true;
            // Emit Mouse pressed Signal
            emit mousePressed(aEvent->pos());
        } else if (aEvent->button() == Qt::RightButton) {
            // Set Right Pressed
            setRightPressed(true);
            // Set Original Pos
            originalPosY = aEvent->pos().y();
            // Set Original Opacity
            originalOpacity = opacity;
        }
    }

    QQuickWidget::mousePressEvent(aEvent);
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void SideImageContainer::mouseMoveEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {

        // Check Pressed
        if (leftPressed) {
            // Emit Mouse Moved Signal
            emit mouseMoved(aEvent->pos());
        // Check Right Pressed
        } else if (rightPressed) {
            // Calculate Opacity Delta
            qreal delta = aEvent->pos().y() - originalPosY;
            // Set Opacity
            setOpacity(originalOpacity - delta / 100);
        }
    }

    QQuickWidget::mouseMoveEvent(aEvent);
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void SideImageContainer::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Button
        if (aEvent->button() == Qt::LeftButton) {
            // Reset Pressed
            leftPressed = false;
            // Emit Mouse Released Signal
            emit mouseReleased(aEvent->pos());
        } else if (aEvent->button() == Qt::RightButton) {
            // Set Right Pressed
            setRightPressed(false);
            // Set Original Pos
            originalPosY = 0.0;
        }
    }

    QQuickWidget::mouseReleaseEvent(aEvent);
}

//==============================================================================
// Mouse Douvle Click Event
//==============================================================================
void SideImageContainer::mouseDoubleClickEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Emit Double Clicked Signal
        emit doubleClicked(side);
    }

    QQuickWidget::mouseDoubleClickEvent(aEvent);
}

//==============================================================================
// Destructor
//==============================================================================
SideImageContainer::~SideImageContainer()
{

}
