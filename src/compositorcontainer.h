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

    Q_PROPERTY(qreal threshold READ getThreshold WRITE setThreshold NOTIFY thresholdChanged)

    Q_PROPERTY(bool rightPressed READ getRightPressed NOTIFY rightPressedChanged)

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

    // Get Threshold
    qreal getThreshold();
    // Set Threshold
    void setThreshold(const qreal& aThreshold);

    // Get Right Pressed
    bool getRightPressed();

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

    // Threshold Changed Signal
    void thresholdChanged(const qreal& aThreshold);

    // Right Pressed Changed Signal
    void rightPressedChanged(const bool& aPressed);

    // Mouse Pressed Signal
    void mousePressed(const QPoint& aPos);
    // Mouse Moved Signal
    void mouseMoved(const QPoint& aPos);
    // Mouse Released Signal
    void mouseReleased(const QPoint& aPos);

    // Mouse Right Button Pressed Signal
    void mouseRightPressed(const QPoint& aPos);
    // Mouse Moved With Right Button Signal
    void mouseRightMoved(const QPoint& aPos);
    // Mouse Right Button Released Signal
    void mouseRightReleased(const QPoint& aPos);

    // Double Clicked Signal
    void doubleClicked();

protected:

    // Set Right Pressed
    void setRightPressed(const bool& aPressed);

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
    bool            leftPressed;
    // Right Presed
    bool            rightPressed;

    // Compare Threshold
    qreal           threshold;

    // Original Pos for Threshold
    qreal           originalPosX;
    // Original Opacity
    qreal           originalThreshold;


};

#endif // COMPOSITORCONTAINER_H
