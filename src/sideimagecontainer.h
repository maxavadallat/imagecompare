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

public:

    // Constructor
    SideImageContainer(QWidget* aParent = NULL);

    // Get Side
    QString getSide();
    // Set Side
    void setSide(const QString& aSide);

    // Destructor
    ~SideImageContainer();

signals:

    // Side Changed Signal
    void sideChanged(const QString& aSide);

    // Mouse Pressed Signal
    void mousePressed(const QPoint& aPos);
    // Mouse Moved Signal
    void mouseMoved(const QPoint& aPos);
    // Mouse Released Signal
    void mouseReleased(const QPoint& aPos);

    // Double Clicked Signal
    void doubleClicked(const QString& aSide);

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

    // Pressed
    bool            pressed;
};



#endif // SIDE_IMAGE_CONTAINER_H
