
#include <QQmlContext>

#include "sideimagecontainer.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
SideImageContainer::SideImageContainer(QWidget* aParent)
    : QQuickWidget(aParent)
    , side("")
    , pressed(false)
{
    // ...

    // Set Controller
    rootContext()->setContextProperty(SIDE_VIEW_CONTROLLER, this);
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
// Mouse Press Event
//==============================================================================
void SideImageContainer::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Set Pressed
        pressed = true;
        // Emit Mouse pressed Signal
        emit mousePressed(aEvent->pos());
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
        if (pressed) {
            // Emit Mouse Moved Signal
            emit mouseMoved(aEvent->pos());
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
        // Reset Pressed
        pressed = false;
        // Emit Mouse Released Signal
        emit mouseReleased(aEvent->pos());
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
