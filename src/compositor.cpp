
#include <QDebug>

#include "mainwindow.h"
#include "compositor.h"
#include "constants.h"
#include "defaultsettings.h"

//==============================================================================
// Constructor
//==============================================================================
Compositor::Compositor(QQuickItem* aParent)
    : QQuickPaintedItem(aParent)
    , mainWindow(NULL)
    , status(CSIdle)
    , operation(COTNoOperation)
    , match(false)
    , currentFileLeft("")
    , currentFileRight("")
    , imageLeft(QImage())
    , imageScaledLeft(QImage())
    , sourceRectLeft(QRect(0, 0, 0, 0))
    , targetRectLeft(QRect(0, 0, 0, 0))
    , imageRight(QImage())
    , imageScaledRight(QImage())
    , sourceRectRight(QRect(0, 0, 0, 0))
    , targetRectRight(QRect(0, 0, 0, 0))
    , zoomLevel(1.0)
    , panPosX(0)
    , panPosY(0)
    , threshold(DEFAULT_COMPARE_THRESHOLD)
    , worker(NULL)
{
    // ...
}

//==============================================================================
// Init Worker
//==============================================================================
void Compositor::initWorker()
{
    // Check Worker Thread
    if (workerThread.isRunning()) {
        // Terminate Worker Thread
        workerThread.terminate();
    }

    // Check Worker
    if (!worker) {
        qDebug() << "Compositor::initWorker";
        // Create Worker
        worker = new CompositorWorker(this);

        // Connect Signals
        connect(worker, SIGNAL(resultReady(int,int)), this, SLOT(workerResultReady(int,int)), Qt::QueuedConnection);
        connect(this, SIGNAL(operateWorker(int)), worker, SLOT(doWork(int)), Qt::DirectConnection);
        connect(worker, SIGNAL(refreshCompositor()), this, SLOT(update()));

        // ...

        // Move To Thread
        worker->moveToThread(&workerThread);
    }

    // Start Worker Thread
    workerThread.start();
    // Set Priprity
    workerThread.setPriority(QThread::IdlePriority);
}

//==============================================================================
// Set Match
//==============================================================================
void Compositor::setMatch(const bool& aMatch)
{
    // Check Match
    if (match != aMatch) {
        // Set Match
        match = aMatch;
        // Emit Match Changed Signal
        emit matchChanged(match);
    }
}

//==============================================================================
// Set Status
//==============================================================================
void Compositor::setStatus(const int& aStatus)
{
    // Check Status
    if (status != aStatus) {
        // Set Status
        status = aStatus;
        // Emit Status Changed Signal
        emit statusChanged(status);
    }
}

//==============================================================================
// Set Operation
//==============================================================================
void Compositor::setOperation(const int& aOperation)
{
    // Check Operation
    if (operation != aOperation) {
        // Set Operation
        operation = aOperation;
        // Emit Operation Changed Signal
        emit operationChanged(operation);
    }
}

//==============================================================================
// Get Matching
//==============================================================================
bool Compositor::getMatch()
{
    return match;
}

//==============================================================================
// Get Status
//==============================================================================
int Compositor::getStatus()
{
    return status;
}

//==============================================================================
// Get Oparation
//==============================================================================
int Compositor::getOperation()
{
    return operation;
}

//==============================================================================
// Get Source Composite Width
//==============================================================================
qreal Compositor::getSourceCompositeWidth()
{
    return qMax(imageLeft.width(), imageRight.width());
}

//==============================================================================
// Get Source  Composite Height
//==============================================================================
qreal Compositor::getSourceCompositeHeight()
{
    return qMax(imageLeft.height(), imageRight.height());
}

//==============================================================================
// Get Composite Width
//==============================================================================
qreal Compositor::getCompositeWidth()
{
    return qMax(imageScaledLeft.width(), imageScaledRight.width());
}

//==============================================================================
// Get Composite Height
//==============================================================================
qreal Compositor::getCompositeHeight()
{
    return qMax(imageScaledLeft.height(), imageScaledRight.height());
}

//==============================================================================
// Get Current File Left
//==============================================================================
QString Compositor::getCurrentFileLeft()
{
    return currentFileLeft;
}

//==============================================================================
// Set Current File Left
//==============================================================================
void Compositor::setCurrentFileLeft(const QString& aCurrentFile)
{
    // Check Current File
    if (currentFileLeft != aCurrentFile && aCurrentFile != "") {
        // Set Match
        setMatch(false);
        // Set Status
        setStatus(CSBusy);
        // Set Operation
        setOperation(COTScaleLeftImage);

        qDebug() << "Compositor::setCurrentFileLeft - aCurrentFile: " << aCurrentFile;

        // Set Current File
        currentFileLeft = aCurrentFile;

        // Emit Current File Changed Signal
        emit currentFileLeftChanged(currentFileLeft);

        // Load Image
        imageLeft.load(currentFileLeft);

        // Notify Composite Sizes Changed
        notifyCompositeSizesChanged();

        // Init Worker
        initWorker();
        // Emit Signal to Start Operation
        emit operateWorker(COTScaleLeftImage);

        // ...
    }
}

//==============================================================================
// Get Current File Right
//==============================================================================
QString Compositor::getCurrentFileRight()
{
    return currentFileRight;
}

//==============================================================================
// Set Current File Right
//==============================================================================
void Compositor::setCurrentFileRight(const QString& aCurrentFile)
{
    // Check Current File
    if (currentFileRight != aCurrentFile && aCurrentFile != "") {
        // Set Match
        setMatch(false);
        // Set Status
        setStatus(CSBusy);
        // Set Operation
        setOperation(COTScaleRightImage);

        qDebug() << "Compositor::setCurrentFileRight - aCurrentFile: " << aCurrentFile;

        // Set Current File
        currentFileRight = aCurrentFile;

        // Emit Current File Changed Signal
        emit currentFileRightChanged(currentFileRight);

        // Load Image
        imageRight.load(currentFileRight);

        // Notify Composite Sizes Changed
        notifyCompositeSizesChanged();

        // Init Worker
        initWorker();
        // Emit Signal to Start Operation
        emit operateWorker(COTScaleRightImage);

        // ...
    }
}

//==============================================================================
// Get Zoom Level
//==============================================================================
qreal Compositor::getZoomLevel()
{
    return zoomLevel;
}

//==============================================================================
// Set Zoom Level
//==============================================================================
void Compositor::setZoomLevel(const qreal& aZoomLevel)
{
    // Check Current Zoom LEvel
    if (zoomLevel != aZoomLevel && aZoomLevel > 0.0 && (currentFileLeft != "" || currentFileRight != "")) {
        // Set Match
        setMatch(false);
        // Set Status
        setStatus(CSBusy);
        // Set Operation
        setOperation(COTScaleImages);

        qDebug() << "Compositor::setZoomLevel - aZoomLevel: " << aZoomLevel * 100 << "%";

        // Set Zoom Level
        zoomLevel = aZoomLevel;

        // Emit Zoom Level Changed Signal
        emit zoomLevelChanged(zoomLevel);

        // Init Worker
        initWorker();
        // Emit Signal to Start Operation
        emit operateWorker(COTScaleImages);

        // ...
    }
}

//==============================================================================
// Get Pan Pos X
//==============================================================================
qreal Compositor::getPanPosX()
{
    return panPosX;
}

//==============================================================================
// Set Pan Pos X
//==============================================================================
void Compositor::setPanPosX(const qreal& aPanPosX)
{
    // Check Current Pan Pos X
    if (panPosX != aPanPosX && (currentFileLeft != "" || currentFileRight != "")) {
        // Set Match
        setMatch(false);
        // Set Status
        setStatus(CSBusy);
        // Set Operation
        setOperation(COTUpdateRects);

        //qDebug() << "Compositor::setPanPosX - aPanPosX: " << aPanPosX;

        // Set Current Pan Pos X
        panPosX = aPanPosX;

        // Emit Pan Pos X Changed Signal
        emit panPosXChanged(panPosX);

        // Init Worker
        initWorker();
        // Emit Signal to Start Operation
        emit operateWorker(COTUpdateRects);

        // ...
    }
}

//==============================================================================
// Get Pan Pos Y
//==============================================================================
qreal Compositor::getPanPosY()
{
    return panPosY;
}

//==============================================================================
// Set Pan Pos Y
//==============================================================================
void Compositor::setPanPosY(const qreal& aPanPosY)
{
    // Check Current Pan Pos X
    if (panPosY != aPanPosY && (currentFileLeft != "" || currentFileRight != "")) {
        // Set Match
        setMatch(false);
        // Set Status
        setStatus(CSBusy);
        // Set Operation
        setOperation(COTUpdateRects);

        //qDebug() << "Compositor::setPanPosY - aPanPosY: " << aPanPosY;

        // Set Current Pan Pos X
        panPosY = aPanPosY;

        // Emit Pan Pos X Changed Signal
        emit panPosYChanged(panPosY);

        // Init Worker
        initWorker();
        // Emit Signal to Start Operation
        emit operateWorker(COTUpdateRects);

        // ...
    }
}

//==============================================================================
// Get Threshold
//==============================================================================
qreal Compositor::getThreshold()
{
    return threshold;
}

//==============================================================================
// Set Threshold
//==============================================================================
void Compositor::setThreshold(const qreal& aThreshold)
{
    // Check Compare Threshold
    if (threshold != aThreshold) {
        // Set Compare Threshold
        threshold = aThreshold;
        // Emit Compare Threshold Changed Signal
        emit thresholdChanged(threshold);
    }
}

//==============================================================================
// Paint
//==============================================================================
void Compositor::paint(QPainter* aPainter)
{
    // Check Painter
    if (aPainter) {
        //qDebug() << ".";

        // Save Painter
        aPainter->save();
/*
        // Set Opacity
        aPainter->setOpacity(1.0);

        // Check Left Scaled Image
        if (imageScaledLeft.width() > 0 && imageScaledLeft.height() > 0) {
            aPainter->drawImage(targetRectLeft, imageScaledLeft, sourceRectLeft);
        }

        // Set Opacity
        aPainter->setOpacity(0.5);

        // Check Right Scaled Image
        if (imageScaledRight.width() > 0 && imageScaledRight.height() > 0) {
            aPainter->drawImage(targetRectRight, imageScaledRight, sourceRectRight);
        }
*/
/*
        // Set Pen
        aPainter->setPen(Qt::NoPen);
        // Set Brush
        aPainter->setBrush(QColor::fromRgba(match ? DEFAULT_COLOR_IMAGE_COMPARE_MATCH : DEFAULT_COLOR_IMAGE_COMPARE_NOMATCH));

        // Draw Overlay Rect
        aPainter->drawRect(boundingRect());
*/
        // Restore Painter
        aPainter->restore();
    }
}

//==============================================================================
// Update Scaled Images According Zoom Level
//==============================================================================
void Compositor::updateScaledImages()
{
    qDebug() << "Compositor::updateScaledImages";
    // Update Left Scaled Image
    updateLeftScaledImage();
    // Update Right Scaled Image
    updateRightScaledImage();
}

//==============================================================================
// Update Left Scaled Image According to Zoom Level
//==============================================================================
void Compositor::updateLeftScaledImage()
{
    // Check Left Image Width & Height
    if (imageLeft.width() > 0 && imageLeft.height() > 0) {
        qDebug() << "Compositor::updateLeftScaledImage";
        // Generate Scaled Image
        imageScaledLeft = imageLeft.scaled(imageLeft.width() * zoomLevel, imageLeft.height() * zoomLevel);

        // Update Left Source Rect
        updateLeftSourceRect();
        // Update Left Target Rect
        updateLeftTargetRect();

    } else {
        // Reset Scaled Left Image
        imageScaledLeft = QImage();
        // Reset Source Rect
        sourceRectLeft = QRect(0, 0, 0, 0);
        // Reset Target Rect
        targetRectLeft = boundingRect();
    }
}

//==============================================================================
// Update Right Scaled Image According to Zoom Level
//==============================================================================
void Compositor::updateRightScaledImage()
{
    // Check Right Image Width & Height
    if (imageRight.width() > 0 && imageRight.height() > 0) {
        qDebug() << "Compositor::updateRightScaledImage";
        // Generate Scaled Image
        imageScaledRight = imageRight.scaled(imageRight.width() * zoomLevel, imageRight.height() * zoomLevel);

        // Update Right Source Rect
        updateRightSourceRect();
        // Update Right Target Rect
        updateRightTargetRect();

    } else {
        // Reset Scaled Right Image
        imageScaledRight = QImage();
        // Reset Source Rect
        sourceRectRight = QRect(0, 0, 0, 0);
        // Reset Target Rect
        targetRectRight = boundingRect();
    }
}

//==============================================================================
// Update Left Source Rect
//==============================================================================
void Compositor::updateLeftSourceRect()
{
    // Set Source Rect
    sourceRectLeft = QRect(qMax((qreal)imageScaledLeft.width() / 2 - boundingRect().width() / 2 - panPosX, 0.0),
                           qMax((qreal)imageScaledLeft.height() / 2 - boundingRect().height() / 2 - panPosY, 0.0),
                           qMin(boundingRect().width(), (qreal)imageScaledLeft.width()),
                           qMin(boundingRect().height(), (qreal)imageScaledLeft.height()));
}

//==============================================================================
// Update Left Target Rect
//==============================================================================
void Compositor::updateLeftTargetRect()
{
    // Set Target Rect
    targetRectLeft = QRect(qMax(boundingRect().width() / 2 - (qreal)imageScaledLeft.width() / 2, 0.0),
                           qMax(boundingRect().height() / 2 - (qreal)imageScaledLeft.height() / 2, 0.0),
                           qMin(boundingRect().width(), (qreal)imageScaledLeft.width()),
                           qMin(boundingRect().height(), (qreal)imageScaledLeft.height()));
}

//==============================================================================
// Update Right Source Rect
//==============================================================================
void Compositor::updateRightSourceRect()
{
    // Set Source Rect
    sourceRectRight = QRect(qMax((qreal)imageScaledRight.width() / 2 - boundingRect().width() / 2 - panPosX, 0.0),
                            qMax((qreal)imageScaledRight.height() / 2 - boundingRect().height() / 2 - panPosY, 0.0),
                            qMin(boundingRect().width(), (qreal)imageScaledRight.width()),
                            qMin(boundingRect().height(), (qreal)imageScaledRight.height()));
}

//==============================================================================
// Update Right Target Rect
//==============================================================================
void Compositor::updateRightTargetRect()
{
    // Set Target Rect
    targetRectRight = QRect(qMax(boundingRect().width() / 2 - (qreal)imageScaledRight.width() / 2, 0.0),
                            qMax(boundingRect().height() / 2 - (qreal)imageScaledRight.height() / 2, 0.0),
                            qMin(boundingRect().width(), (qreal)imageScaledRight.width()),
                            qMin(boundingRect().height(), (qreal)imageScaledRight.height()));
}

//==============================================================================
// Compare Images
//==============================================================================
void Compositor::compareImages()
{
    // Get Left Image Width
    int lWidth = sourceRectLeft.width();
    // Get Left Image Height
    int lHeight = sourceRectLeft.height();
    // Get Right Image Width
    int rWidth = sourceRectRight.width();
    // Get Right Image Height
    int rHeight = sourceRectRight.height();

    if (lWidth == 0 || lHeight == 0 || rWidth == 0 || rHeight == 0) {
        return;
    }

    // Check Source Sizes
    if (sourceRectLeft.width() == sourceRectRight.width() && sourceRectLeft.height() == sourceRectRight.height()) {

        qDebug() << "Compositor::compareImages - [" << lWidth << "x" << lHeight << "]@[" << sourceRectLeft.x() << ":" << sourceRectLeft.y() << "]";

        // Iterate Thru Image's Source Pixels
        for (int y = sourceRectRight.y(); y < sourceRectRight.y() + lHeight && y < imageScaledLeft.height(); ++y) {
            for (int x = sourceRectRight.x(); x < sourceRectRight.x() + lWidth && x < imageScaledLeft.width(); ++x) {

//                if (x & 0x2) {
//                    fprintf(stderr, ".");
//                }

                if (imageScaledLeft.pixel(x, y) != imageScaledRight.pixel(x, y)) {
                    qDebug() << "Compositor::compareImages - no match";

//                    fprintf(stderr, "\n");

                    // Set Match
                    setMatch(false);
                    return;
                }
            }

//            fprintf(stderr, "\n");
        }

        // Set Match
        setMatch(true);

        qDebug() << "Compositor::compareImages - done";
    }
}

//==============================================================================
// Notify Composite Sizes Changed
//==============================================================================
void Compositor::notifyCompositeSizesChanged()
{
    // Emit Source Composite Width Changed
    emit sourceCompositeWidthChanged(qMax(imageLeft.width(), imageRight.width()));
    // Emit Source Composite Height Changed
    emit sourceCompositeHeightChanged(qMax(imageLeft.height(), imageRight.height()));
    // Emit Composite Width Changed Signal
    emit compositeWidthChanged(qMax(imageScaledLeft.width(), imageScaledRight.width()));
    // Emit Composite Height Changed Signal
    emit compositeHeightChanged(qMax(imageScaledLeft.height(), imageScaledRight.height()));
}

//==============================================================================
// Stop Worker Thread
//==============================================================================
void Compositor::stopWorkerThread()
{
    if (workerThread.isRunning()) {
        qDebug() << "Compositor::stopWorkerThread";
        // Terminate
        workerThread.terminate();
        // Wait
        //workerThread.wait();
    }
}

//==============================================================================
// Worker Result Ready Slot
//==============================================================================
void Compositor::workerResultReady(const int& aOperation, const int& aResult)
{
    switch (aOperation) {
        case COTNoOperation:
            // NOOP
        break;

        case COTScaleImages:
        case COTScaleLeftImage:
        case COTScaleRightImage:
            // Update
            update();
            // Notify Composite Sizes Changed
            notifyCompositeSizesChanged();
            // Set Status
            setStatus(CSIdle);

            // ...

            // Set Operation
            setOperation(COTCompareImages);
            // Set Status
            setStatus(CSBusy);

            // Emit Signal to Start Operation
            emit operateWorker(COTCompareImages);
        return;

        case COTCompareImages:
            // Update
            update();
        break;

        case COTUpdateRects:
            // Update
            update();
            // Set Status
            setStatus(CSIdle);

            // Set Operation
            setOperation(COTCompareImages);
            // Set Status
            setStatus(CSBusy);

            // Emit Signal to Start Operation
            emit operateWorker(COTCompareImages);
        return;

        default:
            qDebug() << "Compositor::workerResultReady - aOperation: " << aOperation << " - aResult: " << aResult;
        break;
    }

    // Set Status
    setStatus(CSIdle);
}

//==============================================================================
// Geometry Changed
//==============================================================================
void Compositor::geometryChanged(const QRectF& aNewGeometry, const QRectF& aOldGeometry)
{
    // Calling Super Geometry Changed
    QQuickPaintedItem::geometryChanged(aNewGeometry, aOldGeometry);

    //qDebug() << "Compositor::geometryChanged - aNewGeometry: " << aNewGeometry;

    if (currentFileLeft != "" || currentFileRight != "") {
        // Init Worker
        initWorker();
        // Emit Signal to Start Operation
        emit operateWorker(COTUpdateRects);
    }
}

//==============================================================================
// Destructor
//==============================================================================
Compositor::~Compositor()
{
    // Stop Worker Thread
    stopWorkerThread();

    // Wait for Worker Thread To Finish
    workerThread.wait();

    // Check Worker
    if (worker) {
        delete worker;
        worker = NULL;
    }

    // ...
}

























//==============================================================================
// Constructor
//==============================================================================
CompositorWorker::CompositorWorker(Compositor* aParent)
    : QObject(NULL)
    , compositor(aParent)
{
    qDebug() << "CompositorWorker::CompositorWorker";

    // ...
}

//==============================================================================
// Do Work
//==============================================================================
void CompositorWorker::doWork(const int& aOperation)
{
    // Check Compositor
    if (!compositor) {
        return;
    }

    //qDebug() << "CompositorWorker::doWork - aOperation: " << aOperation;

    // Switch Operation
    switch (aOperation) {
        case COTScaleImages:
            // Update Scaled Images
            compositor->updateScaledImages();
        break;

        case COTScaleLeftImage:
            // Update Left Scaled Image
            compositor->updateLeftScaledImage();
        break;

        case COTScaleRightImage:
            // Update Right Scaled Image
            compositor->updateRightScaledImage();
        break;

        case COTCompareImages:
            // Compare Images
            compositor->compareImages();
        break;

        case COTUpdateRects:
            // Update Left Source Rect
            compositor->updateLeftSourceRect();
            // Update Left Target Rect
            compositor->updateLeftTargetRect();
            // Update Right Source Rect
            compositor->updateRightSourceRect();
            // Update Right Target Rect
            compositor->updateRightTargetRect();
        break;

        default:
            qDebug() << "CompositorWorker::doWork - aOperation: " << aOperation << " - UNHANDLED OPERATION!";
        return;
    }

    //qDebug() << "CompositorWorker::doWork - aOperation: " << aOperation << " Ready.";

    // Emit Result Ready
    emit resultReady(aOperation, 0);
}

//==============================================================================
// Stop
//==============================================================================
void CompositorWorker::stop()
{
    // Check CompositorWorker Thread
    if (compositor->workerThread.isRunning()) {
        qDebug() << "CompositorWorker::stop";

        // Terminate CompositorWorker Thread
        compositor->workerThread.terminate();

        // ...
    }
}

//==============================================================================
// Destructor
//==============================================================================
CompositorWorker::~CompositorWorker()
{
    // Stop
    stop();

    qDebug() << "CompositorWorker::~CompositorWorker";
}

