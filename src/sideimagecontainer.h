#ifndef SIDE_IMAGE_CONTAINER_H
#define SIDE_IMAGE_CONTAINER_H

#include <QQuickWidget>
#include <QMouseEvent>

//==============================================================================
// Side Image Container
//==============================================================================
class SideImageContainer : public QQuickWidget
{
    Q_OBJECT

    Q_PROPERTY(QString side READ getSide WRITE setSide NOTIFY sideChanged)

    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY opacityChanged)

    Q_PROPERTY(bool rightPressed READ getRightPressed WRITE setRightPressed NOTIFY rightPressedChanged)

public:

    // Constructor
    SideImageContainer(QWidget* aParent = NULL);

    // Get Side
    QString getSide();
    // Set Side
    void setSide(const QString& aSide);

    // Get Opacity
    qreal getOpacity();
    // Set Opacity
    void setOpacity(const qreal& aOpacity);

    // Get Right Pressed
    bool getRightPressed();

    // Destructor
    ~SideImageContainer();

signals:

    // Side Changed Signal
    void sideChanged(const QString& aSide);

    // Opacity Changed Signal
    void opacityChanged(const qreal& aOpacity);

    // Right Pressed Changed Signal
    void rightPressedChanged(const bool& aPressed);

    // Mouse Pressed Signal
    void mousePressed(const QPoint& aPos);
    // Mouse Moved Signal
    void mouseMoved(const QPoint& aPos);
    // Mouse Released Signal
    void mouseReleased(const QPoint& aPos);

    // Double Clicked Signal
    void doubleClicked(const QString& aSide);

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

    // Side
    QString         side;

    // Opacity
    qreal           opacity;

    // Pressed
    bool            leftPressed;

    // Right Pressed
    bool            rightPressed;

    // Original Pos for Opacity
    qreal           originalPosY;
    // Original Opacity
    qreal           originalOpacity;
};



#endif // SIDE_IMAGE_CONTAINER_H
