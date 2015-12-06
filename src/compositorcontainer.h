#ifndef COMPOSITORCONTAINER_H
#define COMPOSITORCONTAINER_H

#include <QQuickWidget>

class MainWindow;

//==============================================================================
// Compositor Container Class
//==============================================================================
class CompositorContainer : public QQuickWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal sourceCompositeWidth READ getSourceCompositeWidth WRITE setSourceCompositeWidth NOTIFY sourceCompositeWidthChanged)
    Q_PROPERTY(qreal sourceCompositeHeight READ getSourceCompositeHeight WRITE setSourceCompositeHeight NOTIFY sourceCompositeHeightChanged)

    Q_PROPERTY(qreal compositeWidth READ getCompositeWidth WRITE setCompositeWidth NOTIFY compositeWidthChanged)
    Q_PROPERTY(qreal compositeHeight READ getCompositeHeight WRITE setCompositeHeight NOTIFY compositeHeightChanged)

public:

    // Constructor
    CompositorContainer(QWidget* aParent = NULL);

    // Get Source Composite Width
    qreal getSourceCompositeWidth();
    // Set Source Composite Width
    void setSourceCompositeWidth(const qreal& aWidth);

    // Get Source Composite Height
    qreal getSourceCompositeHeight();
    // Set Source Composite Height
    void setSourceCompositeHeight(const qreal& aHeight);

    // Get Composite Width
    qreal getCompositeWidth();
    // Set Composite Width
    void setCompositeWidth(const qreal& aWidth);

    // Get Composite Height
    qreal getCompositeHeight();
    // Set Composite Height
    void setCompositeHeight(const qreal& aHeight);

    // Destructor
    ~CompositorContainer();

signals:

    // Source Composite Width Changed Signal
    void sourceCompositeWidthChanged(const qreal& aCompositeWidth);
    // Source Composite Height Changed Signal
    void sourceCompositeHeightChanged(const qreal& aCompositeHeight);

    // Composite Width Changed Signal
    void compositeWidthChanged(const qreal& aCompositeWidth);
    // Composite Height Changed Signal
    void compositeHeightChanged(const qreal& aCompositeHeight);

    // Mouse Pressed Signal
    void mousePressed(const QPoint& aPos);
    // Mouse Moved Signal
    void mouseMoved(const QPoint& aPos);
    // Mouse Released Signal
    void mouseReleased(const QPoint& aPos);

    // Double Clicked Signal
    void doubleClicked();

protected:

    // Mouse Press Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Move Event
    virtual void mouseMoveEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);
    // Mouse Douvle Click Event
    virtual void mouseDoubleClickEvent(QMouseEvent* aEvent);

private:

    // Composite Width
    qreal           compositeWidth;
    // Composite Height
    qreal           compositeHeight;

    // Source Composite Width
    qreal           sourceCompositeWidth;
    // Source Composite Height
    qreal           sourceCompositeHeight;

    // Pressed
    bool            pressed;

};

#endif // COMPOSITORCONTAINER_H
