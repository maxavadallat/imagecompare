#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QThread>

class MainWindow;
class CompositorWorker;

//==============================================================================
// Worker Class Operation Types
//==============================================================================
enum COperationType
{
    COTNoOperation      = 0,
    COTScaleImages,
    COTScaleLeftImage,
    COTScaleRightImage,
    COTCompareImages,
    COTUpdateRects,
    COTUpdateLeftRects,
    COTUpdateRightRects,
};



//==============================================================================
// Compositor Component Class
//==============================================================================
class Compositor : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool match READ getMatch NOTIFY matchChanged)

    Q_PROPERTY(int status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(int operation READ getOperation NOTIFY operationChanged)

    Q_PROPERTY(QString currentFileLeft READ getCurrentFileLeft WRITE setCurrentFileLeft NOTIFY currentFileLeftChanged)
    Q_PROPERTY(QString currentFileRight READ getCurrentFileRight WRITE setCurrentFileRight NOTIFY currentFileRightChanged)

    Q_PROPERTY(int zoomLevelIndex READ getZoomLevelIndex WRITE setZoomLevelIndex NOTIFY zoomLevelIndexChanged)
    Q_PROPERTY(qreal zoomLevel READ getZoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)

    Q_PROPERTY(qreal panPosX READ getPanPosX WRITE setPanPosX NOTIFY panPosXChanged)
    Q_PROPERTY(qreal panPosY READ getPanPosY WRITE setPanPosY NOTIFY panPosYChanged)

    Q_PROPERTY(qreal sourceCompositeWidth READ getSourceCompositeWidth NOTIFY sourceCompositeWidthChanged)
    Q_PROPERTY(qreal sourceCompositeHeight READ getSourceCompositeHeight NOTIFY sourceCompositeHeightChanged)

    Q_PROPERTY(qreal compositeWidth READ getCompositeWidth NOTIFY compositeWidthChanged)
    Q_PROPERTY(qreal compositeHeight READ getCompositeHeight NOTIFY compositeHeightChanged)

    Q_PROPERTY(qreal threshold READ getThreshold WRITE setThreshold NOTIFY thresholdChanged)

    Q_PROPERTY(bool showGrid READ getShowGrid WRITE setShowGrid NOTIFY showGridChanged)

public:

    // Compositor Status Type
    enum CompositorStatusType
    {
        CSIdle = 0,
        CSBusy
    };

    Q_ENUMS(CompositorStatusType)

    // Constructor
    Compositor(QQuickItem* aParent = NULL);

    // Get Matching
    bool getMatch();

    // Get Status
    int getStatus();

    // Get Oparation
    int getOperation();

    // Get Source Composite Width
    qreal getSourceCompositeWidth();
    // Get Source Composite Height
    qreal getSourceCompositeHeight();

    // Get Composite Width
    qreal getCompositeWidth();
    // Get Composite Height
    qreal getCompositeHeight();

    // Get Current File Left
    QString getCurrentFileLeft();
    // Set Current File Left
    void setCurrentFileLeft(const QString& aCurrentFile);

    // Get Current File Right
    QString getCurrentFileRight();
    // Set Current File Right
    void setCurrentFileRight(const QString& aCurrentFile);

    // Get Zoom Level Index
    int getZoomLevelIndex();
    // Set Zoom Level Index
    void setZoomLevelIndex(const int& aZoomLevelIndex);

    // Set Zoom Level
    void setZoomLevel(const qreal& aZoomLevel);
    // Get Zoom Level
    qreal getZoomLevel();

    // Get Pan Pos X
    qreal getPanPosX();
    // Set Pan Pos X
    void setPanPosX(const qreal& aPanPosX);

    // Get Pan Pos Y
    qreal getPanPosY();
    // Set Pan Pos Y
    void setPanPosY(const qreal& aPanPosY);

    // Get Threshold
    qreal getThreshold();
    // Set Threshold
    void setThreshold(const qreal& aThreshold);

    // Get Show Grid
    bool getShowGrid();
    // Set Show Grid
    void setShowGrid(const bool& aShowGrid);

    // Paint
    virtual void paint(QPainter* aPainter);

    // Destructor
    ~Compositor();

signals:

    // Match Changed Signal
    void matchChanged(const bool& aMatch);

    // Source Composite Width Changed Signal
    void sourceCompositeWidthChanged(const qreal& aCompositeWidth);
    // Source Composite Height Changed Signal
    void sourceCompositeHeightChanged(const qreal& aCompositeHeight);

    // Composite Width Changed Signal
    void compositeWidthChanged(const qreal& aCompositeWidth);
    // Composite Height Changed Signal
    void compositeHeightChanged(const qreal& aCompositeHeight);

    // Status Changed Signal
    void statusChanged(const int& aStatus);
    // Oparation Changed Signal
    void operationChanged(const int& aOperation);

    // Current Left File Changed Signal
    void currentFileLeftChanged(const QString& aCurrentFile);
    // Current Right File Changed Signal
    void currentFileRightChanged(const QString& aCurrentFile);

    // Zoom Level Index Changed Signal
    void zoomLevelIndexChanged(const int& aZoomLevelIndex);
    // Zoom Level Changed Signal
    void zoomLevelChanged(const qreal& aZoomLevel);

    // Pan Pos X Changed Signal
    void panPosXChanged(const qreal& aPanPosX);
    // Pan Pos Y Changed Signal
    void panPosYChanged(const qreal& aPanPosY);

    // Threshold Changed Signal
    void thresholdChanged(const qreal& aThreshold);

    // Show Grid Changed Signal
    void showGridChanged(const bool& aShowGrid);

    // Operate Worker Signel
    void operateWorker(const int& aOperation);

protected:

    // Init Worker
    void initWorker();

    // Set Match
    void setMatch(const bool& aMatch);

    // Set Status
    void setStatus(const int& aStatus);

    // Set Operation
    void setOperation(const int& aOperation);

    // Update Scaled Images According to Zoom Level
    void updateScaledImages();

    // Update Left Scaled Image According to Zoom Level
    void updateLeftScaledImage();
    // Update Right Scaled Image According to Zoom Level
    void updateRightScaledImage();

    // Update Left Source Rect
    void updateLeftSourceRect();
    // Update Left Target Rect
    void updateLeftTargetRect();

    // Update Right Source Rect
    void updateRightSourceRect();
    // Update Right Target Rect
    void updateRightTargetRect();

    // Compare Images
    void compareImages();

    // Notify Composite Sizes Changed
    void notifyCompositeSizesChanged();

    // Update Positions: Center, TopLeft, GridStart
    void updatePositions(const bool& aHorizontal = true, const bool& aVertical = true);

    // Stop Worker Thread
    void stopWorkerThread();

protected slots:

    // Worker Result Ready Slot
    void workerResultReady(const int& aOperation, const int& aResult);

protected:

    // Geometry Changed
    virtual void geometryChanged(const QRectF& aNewGeometry, const QRectF& aOldGeometry);

private:
    friend class CompositorWorker;

    // Main Window
    MainWindow*         mainWindow;

    // Status
    int                 status;
    // Operation
    int                 operation;

    // Match
    bool                match;

    // Current Left File
    QString             currentFileLeft;
    // Current Right File
    QString             currentFileRight;

    // Left Image
    QImage              imageLeft;
    // Left Image Scaled
    QImage              imageScaledLeft;
    // Left Source Rect
    QRectF              sourceRectLeft;
    // Left Target Rect
    QRectF              targetRectLeft;

    // Right Image
    QImage              imageRight;
    // Right Image
    QImage              imageScaledRight;
    // Right Source Rect
    QRectF              sourceRectRight;
    // Right Target Rect
    QRectF              targetRectRight;

    // Zoom Level Index
    int                 zoomLevelIndex;
    // Zoom Level
    qreal               zoomLevel;
    // Pan Pos X
    qreal               panPosX;
    // Pan Pos Y
    qreal               panPosY;

    // Most Left Visible Pixel of the Composed Image
    qreal               left;
    // Most Top Visible Pixel of the Composed Image
    qreal               top;
    // Horizontal Center Pixel of the Composed Image
    qreal               centerX;
    // Vertical Center Pixel of the Composed Image
    qreal               centerY;

    // Compare Threshold
    qreal               threshold;
    // Show Grid
    bool                showGrid;
    // Grid Step
    int                 gridStep;

    // Grid Normal Pen
    QPen                gridPen;
    // Grid Section Pen
    QPen                gridSectionPen;

    // Grid Start X
    qreal               gridStartX;
    // Grid Start Y
    qreal               gridStartY;

    // Grid Section Width
    qreal               gridSectionWidth;

    // Worker Thread
    QThread             workerThread;
    // Compositor Worker
    CompositorWorker*   worker;
};






//==============================================================================
// Worker Class For Thread Operations
//==============================================================================
class CompositorWorker : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit CompositorWorker(Compositor* aParent = NULL);

    // Destructor
    virtual ~CompositorWorker();

signals:

    // Result Ready Signal
    void resultReady(const int& aOperation, const int& aResult);

    // Refresh Compositor
    void refreshCompositor();

public slots:

    // Do Work
    void doWork(const int& aOperation);

    // Stop
    void stop();

private:
    // Compositor
    Compositor*      compositor;
};



#endif // COMPOSITOR_H
