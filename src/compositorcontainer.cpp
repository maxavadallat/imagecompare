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
    , pressed(false)
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
// Mouse Press Event
//==============================================================================
void CompositorContainer::mousePressEvent(QMouseEvent* aEvent)
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
void CompositorContainer::mouseMoveEvent(QMouseEvent* aEvent)
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
void CompositorContainer::mouseReleaseEvent(QMouseEvent* aEvent)
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
