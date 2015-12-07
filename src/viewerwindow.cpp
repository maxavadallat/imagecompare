#include <QDebug>
#include <QApplication>
#include <QSettings>
#include <QQmlContext>
#include <QRect>

#include "mainwindow.h"
#include "viewerwindow.h"
#include "constants.h"

#include "ui_viewerwindow.h"

// Main Viewer Window Singleton
static ViewerWindow* viewerWindow = NULL;

//==============================================================================
// Static Constructor
//==============================================================================
ViewerWindow* ViewerWindow::getInstance()
{
    // Check Singleton
    if (!viewerWindow) {
        // Create Main Browser Window
        viewerWindow = new ViewerWindow();
    }

    return viewerWindow;
}

//==============================================================================
// Release Instance
//==============================================================================
void ViewerWindow::release()
{
    // Delete Main Browser Window
    delete viewerWindow;
    // Reset Singleton
    viewerWindow = NULL;
}

//==============================================================================
// Constructor
//==============================================================================
ViewerWindow::ViewerWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::ViewerWindow)
    , mainWindow(MainWindow::getInstance())
    , currentFile("")
    , zoomLevelIndex(DEFAULT_ZOOM_LEVEL_INDEX)
    , zoomLevel(zoomLevels[zoomLevelIndex])
    , zoomFit(false)
    , panPressX(0.0)
    , panPressY(0.0)
    , panPosLastX(0.0)
    , panPosLastY(0.0)
    , panPosX(0.0)
    , panPosY(0.0)
    , leftPressed(false)
    , imageWidth(0.0)
    , imageHeight(0.0)
    , keyModifiers(Qt::NoModifier)
{
    // Setup UI
    ui->setupUi(this);

    // Get QML Context
    QQmlContext* qmlContext = ui->viewerWidget->rootContext();

    // Set Context Property - Main View Controller
    qmlContext->setContextProperty(MAIN_VIEW_CONTROLLER, mainWindow);

    // Set Context Property
    qmlContext->setContextProperty(VIEWER_VIEW_CONTROLLER, this);

    // Set Source
    ui->viewerWidget->setSource(QUrl(QML_SOURCE_VIEWER_VIEW));

    // Restore UI
    //restoreUI();
}

//==============================================================================
// Get Current File
//==============================================================================
QString ViewerWindow::getCurrentFile()
{
    return currentFile;
}

//==============================================================================
// Set Current File
//==============================================================================
void ViewerWindow::setCurrentFile(const QString& aCurrentFile)
{
    // Check Current File
    if (currentFile != aCurrentFile) {
        // Set Current File
        currentFile = aCurrentFile;
        // Emit Current File Changed Signal
        emit currentFileChanged(currentFile);
        // Show Status Text
        showStatusText(currentFile);
    }
}

//==============================================================================
// Get Zoom Level
//==============================================================================
qreal ViewerWindow::getZoomLevel()
{
    return zoomLevel;
}

//==============================================================================
// Get Pan Pos X
//==============================================================================
qreal ViewerWindow::getPanPosX()
{
    return panPosX;
}

//==============================================================================
// Set Pan Pos X
//==============================================================================
void ViewerWindow::setPanPosX(const qreal& aPanPosX)
{
    // Calculate Boundary
    qreal bound = qMax(imageWidth / 2 - (qreal)ui->viewerWidget->width() / 2, 0.0);

    //qDebug() << "ViewerWindow::setPanPosX - bound: " << bound;

    // Calculate Min Bound
    qreal minBound = -bound;
    // Calculate Max Bound
    qreal maxBound = bound;

    // Get New Pan Pos X
    qreal newPanPosX = qBound(minBound, aPanPosX, maxBound);

    // Check Current Pan Pos X
    if (panPosX != newPanPosX) {
        //qDebug() << "ViewerWindow::setPanPosX - newPanPosX: " << newPanPosX;

        // Set Pan Pos X
        panPosX = newPanPosX;

        // Emit Pan Pos X Changed Signal
        emit panPosXChanged(panPosX);

        // ...
    }
}

//==============================================================================
// Get Pan Pos Y
//==============================================================================
qreal ViewerWindow::getPanPosY()
{
    return panPosY;
}

//==============================================================================
// Set Pan Pos Y
//==============================================================================
void ViewerWindow::setPanPosY(const qreal& aPanPosY)
{
    // Calculate Boundary
    qreal bound = qMax(imageHeight / 2 - (qreal)ui->viewerWidget->height() / 2, 0.0);

    //qDebug() << "ViewerWindow::setPanPosY - bound: " << bound;

    // Calculate Min Bound
    qreal minBound = -bound;
    // Calculate Max Bound
    qreal maxBound = bound;

    // Get New Pan Pos Y
    qreal newPanPosY = qBound(minBound, aPanPosY, maxBound);

    // Check Current Pan Pos Y
    if (panPosY != newPanPosY) {
        //qDebug() << "ViewerWindow::setPanPosY - newPanPosY: " << newPanPosY;

        // Set Pan Pos Y
        panPosY = newPanPosY;

        // Emit Pan Pos Y Changed Signal
        emit panPosYChanged(panPosY);

        // ...
    }
}

//==============================================================================
// Get Image Width
//==============================================================================
qreal ViewerWindow::getImageWidth()
{
    return imageWidth;
}

//==============================================================================
// Set Image Width
//==============================================================================
void ViewerWindow::setImageWidth(const qreal& aWidth)
{
    // Check Image Width
    if (imageWidth != aWidth) {
        // Set Image Width
        imageWidth = aWidth;
        // Emit Image Width Changed Signal
        emit imageWidthChanged(imageWidth);

        // Reset Pan Pos X - Boundaries Might Update
        setPanPosX(panPosX);
    }
}

//==============================================================================
// Get Image Height
//==============================================================================
qreal ViewerWindow::getImageHeight()
{
    return imageHeight;
}

//==============================================================================
// Set Image Height
//==============================================================================
void ViewerWindow::setImageHeight(const qreal& aHeight)
{
    // Check Image Height
    if (imageHeight != aHeight) {
        // Set Image Height
        imageHeight = aHeight;
        // Emit Image height Changed Signal
        emit imageHeightChanged(imageHeight);

        // Reset Pan Pos Y - Boundaries Might Update
        setPanPosY(panPosY);
    }
}

//==============================================================================
// Get Source  Width
//==============================================================================
qreal ViewerWindow::getSourceWidth()
{
    return imageSourceWidth;
}

//==============================================================================
// Set Source  Width
//==============================================================================
void ViewerWindow::setSourceWidth(const qreal& aWidth)
{
    // Check  Width
    if (imageSourceWidth != aWidth) {
        // Set  Width
        imageSourceWidth = aWidth;
        // Emit Source  Width Changed Signal
        emit sourceWidthChanged(imageSourceWidth);
    }
}

//==============================================================================
// Get Source  Height
//==============================================================================
qreal ViewerWindow::getSourceHeight()
{
    return imageSourceHeight;
}

//==============================================================================
// Set Source  Height
//==============================================================================
void ViewerWindow::setSourceHeight(const qreal& aHeight)
{
    // Check Source  Height
    if (imageSourceHeight != aHeight) {
        // Set  Height
        imageSourceHeight = aHeight;
        // Emit Source  Height Changed Signal
        emit sourceHeightChanged(imageSourceHeight);
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void ViewerWindow::restoreUI()
{
    qDebug() << "ViewerWindow::restoreUI";

    // Init Settings
    QSettings settings;

    // Set Width & Height
    setGeometry(QRect(settings.value(SETTINGS_KEY_VIEWER_POSX).toInt(),
                      settings.value(SETTINGS_KEY_VIEWER_POSY).toInt(),
                      settings.value(SETTINGS_KEY_VIEWER_WIDTH).toInt(),
                      settings.value(SETTINGS_KEY_VIEWER_HEIGHT).toInt()));

    // Connect Signals
    //connect(mainWindow, SIGNAL(currentFileChanged(QString)), this, SLOT(currentFileChanged(QString)));
}

//==============================================================================
// Update Menu
//==============================================================================
void ViewerWindow::updateMenu()
{
    qDebug() << "ViewerWindow::updateMenu";

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void ViewerWindow::saveSettings()
{
    qDebug() << "ViewerWindow::saveSettings";

    // Init Settings
    QSettings settings;

    // Get Full Screen
    bool fullScreen = windowState() == Qt::WindowFullScreen;

    // Save Full Screen State
    settings.setValue(SETTINGS_KEY_VIEWER_FULLSCREEN, fullScreen);

    // Check Full Screen
    if (!fullScreen) {
        // Get Geometry
        QRect currRect = geometry();

        // Save Window Position
        settings.setValue(SETTINGS_KEY_VIEWER_POSX, currRect.x());
        settings.setValue(SETTINGS_KEY_VIEWER_POSY, currRect.y());
        // Save Window Width & Height
        settings.setValue(SETTINGS_KEY_VIEWER_WIDTH, currRect.width());
        settings.setValue(SETTINGS_KEY_VIEWER_HEIGHT, currRect.height());
    }
}

//==============================================================================
// Zoom In
//==============================================================================
void ViewerWindow::zoomIn()
{
    qDebug() << "ViewerWindow::zoomIn";

    // Check Zoom Fit
    if (zoomFit) {
        // Reset Zoom Fit
        zoomFit = false;
        // Reset Zoom Level Index
        zoomLevelIndex = 0;
        // Iterate Through Zoom Levels
        while (zoomLevels[zoomLevelIndex] < zoomLevel && zoomLevelIndex < DEFAULT_ZOOM_LEVEL_INDEX_MAX) {
            // Inc Zoom Level
            zoomLevelIndex++;
        }

        // Set Zoom Level
        setZoomLevel(zoomLevels[zoomLevelIndex]);

    } else {
        // Check Zoom Level Index
        if (zoomLevelIndex < DEFAULT_ZOOM_LEVEL_INDEX_MAX) {
            //qDebug() << "ViewerWindow::zoomIn";

            // Inz Zoom Level Index
            zoomLevelIndex++;

            // Set Zoom Level
            setZoomLevel(zoomLevels[zoomLevelIndex]);
        }
    }

    // Configure Menu
    updateMenu();
}

//==============================================================================
// Zoom Out
//==============================================================================
void ViewerWindow::zoomOut()
{
    qDebug() << "ViewerWindow::zoomOut";

    // Check Zoom Fit
    if (zoomFit) {
        // reset Zoom Fit
        zoomFit = false;
        // Reset Zoom Level Index
        zoomLevelIndex = DEFAULT_ZOOM_LEVEL_INDEX_MAX;

        while (zoomLevels[zoomLevelIndex] > zoomLevel && zoomLevelIndex > 0) {
            // Dec Zoom Level Index
            zoomLevelIndex--;
        }

        // Set Zoom Level
        setZoomLevel(zoomLevels[zoomLevelIndex]);

    } else {
        // Check Zoom Level Index
        if (zoomLevelIndex > 0) {
            //qDebug() << "MainWindow::zoomOut";

            // Dec Zoom Level Index
            zoomLevelIndex--;

            // Set Zoom Level
            setZoomLevel(zoomLevels[zoomLevelIndex]);
        }
    }

    // Configure Menu
    updateMenu();
}

//==============================================================================
// Zoom Default
//==============================================================================
void ViewerWindow::zoomDefault()
{
    qDebug() << "ViewerWindow::zoomDefault";

    // Reset Zoom Fit
    zoomFit = false;

    // Set Zoom Level Index
    zoomLevelIndex = DEFAULT_ZOOM_LEVEL_INDEX;

    // Set Zoom Level
    setZoomLevel(zoomLevels[zoomLevelIndex]);

    // Configure Menu
    updateMenu();
}

//==============================================================================
// Zoom To Fit
//==============================================================================
void ViewerWindow::zoomToFit()
{
    // Check Zoom Fit
    if (!zoomFit) {
        qDebug() << "ViewerWindow::zoomToFit";

        // Set Zoom Fit
        zoomFit = true;

        // Adjust Zoom Level To Fit
        adjustZoomLevelToFit();
    }

    // Update Menu
    updateMenu();
}

//==============================================================================
// Reset
//==============================================================================
void ViewerWindow::reset()
{
    qDebug() << "ViewerWindow::reset";

    // Reset Pan Position
    setPanPosX(0.0);
    setPanPosY(0.0);

    // Zoom Default
    zoomDefault();
}

//==============================================================================
// Adjust Zoom Level To Fit
//==============================================================================
void ViewerWindow::adjustZoomLevelToFit()
{
    // Calculate Width Ratio
    qreal widthRatio = imageSourceWidth / ui->viewerWidget->width();
    // Calculate Height Ratio
    qreal heightRatio = imageSourceHeight / ui->viewerWidget->height();

    // Init New Zoom Level
    qreal newZoomLevel = 1.0;

    // Check Ratios
    if (widthRatio > heightRatio) {
        // Calculate New Zoom Level
        newZoomLevel = 1.0 / widthRatio;
    } else {
        newZoomLevel = 1.0 / heightRatio;
    }

    // ...

    // Set Zoom Level
    setZoomLevel(newZoomLevel);
    // Reset Pan Pos X
    setPanPosX(0.0);
    // Reset Pan Pos Y
    setPanPosY(0.0);
}

//==============================================================================
// Set Zoom Level
//==============================================================================
void ViewerWindow::setZoomLevel(const qreal& aZoomLevel)
{
    // Check Current Zoom LEvel
    if (zoomLevel != aZoomLevel) {
        //qDebug() << "ViewerWindow::setZoomLevel - aZoomLevel: " << aZoomLevel * 100 << "%";

        // Set Zoom Level
        zoomLevel = aZoomLevel;

        // Emit Zoom Level Changed Signal
        emit zoomLevelChanged(zoomLevel);

        // Get Zoom Level Status Text
        QString zoomtext = QString(tr("Zoom level: ") + "%1%").arg(zoomLevel * 100);

        // Show Status Text
        showStatusText(zoomtext);

        // ...
    }
}

//==============================================================================
// Launc Preferences
//==============================================================================
void ViewerWindow::launcPreferences()
{
    qDebug() << "ViewerWindow::launchBrowserWindow";

    // Launch Preferences
    mainWindow->launchPreferences();
}

//==============================================================================
// Show Window
//==============================================================================
void ViewerWindow::showWindow()
{
    qDebug() << "ViewerWindow::showWindow";

    // Init Settings
    QSettings settings;

    // Check Full Screen
    if (settings.value(SETTINGS_KEY_VIEWER_FULLSCREEN).toBool()) {
        // Show Full Screen
        showFullScreen();
    } else {
        // Hide
        hide();

        // Set Width & Height
        setGeometry(QRect(settings.value(SETTINGS_KEY_VIEWER_POSX).toInt(),
                          settings.value(SETTINGS_KEY_VIEWER_POSY).toInt(),
                          settings.value(SETTINGS_KEY_VIEWER_WIDTH).toInt(),
                          settings.value(SETTINGS_KEY_VIEWER_HEIGHT).toInt()));

        // Show
        showNormal();
        // Raise
        raise();

        // Grab Keyboard
        grabKeyboard();
    }
}

//==============================================================================
// Hide Window
//==============================================================================
void ViewerWindow::hideWindow()
{
    qDebug() << "ViewerWindow::hideWindow";

    // Save Settings
    saveSettings();

    // hide
    hide();

    // Release Keyboard
    releaseKeyboard();

    // Emit Window Closed Signal
    //emit windowClosed();
}

//==============================================================================
// Show Status Text
//==============================================================================
void ViewerWindow::showStatusText(const QString& aStatusText, const int& aTimeout)
{
    // Show Message
    ui->statusbar->showMessage(aStatusText, aTimeout);
}

//==============================================================================
// On Action About Triggered
//==============================================================================
void ViewerWindow::on_actionAbout_triggered()
{
    // Launch About Dialog
}

//==============================================================================
// On Action Preferences Triggered
//==============================================================================
void ViewerWindow::on_actionPreferences_triggered()
{
    // Launch Preferences
    launcPreferences();
}

//==============================================================================
// On Action Rotate Left Triggered
//==============================================================================
void ViewerWindow::on_actionRotate_Left_triggered()
{
    // Rotate Left Current File
    mainWindow->rotateLeft();
}

//==============================================================================
// On Action Rotate Right Triggered
//==============================================================================
void ViewerWindow::on_actionRotate_Right_triggered()
{
    // Rotatet Right Current File
    mainWindow->rotateRigth();
}

//==============================================================================
// On Action Flip Horizontally Trigered
//==============================================================================
void ViewerWindow::on_actionFlip_Horizontally_triggered()
{
    // Flip Current File Horizontally
    mainWindow->flipHorizontally();
}

//==============================================================================
// On Action Flip Vertically Triggered
//==============================================================================
void ViewerWindow::on_actionFlip_Vertically_triggered()
{
    // Flip Current File Vertically
    mainWindow->flipVertically();
}

//==============================================================================
// On Action Quit Triggered
//==============================================================================
void ViewerWindow::on_actionQuit_triggered()
{
    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Key Press Event
//==============================================================================
void ViewerWindow::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Set Key Modifiers
        keyModifiers = aEvent->modifiers();
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void ViewerWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Hide Window
                hideWindow();
            break;

            case Qt::Key_Minus:
                // Zoom Out
                zoomOut();
            break;

            case Qt::Key_Plus:
            case Qt::Key_Equal:
                // Zoom In
                zoomIn();
            break;

            case Qt::Key_Asterisk:
                // Zoom To Fit
                zoomToFit();
            break;

            case Qt::Key_Space:
                // Reset Zoom Level & Pan Position
                reset();
            break;

            default:
            break;
        }

        // Set Key Modifiers
        keyModifiers = aEvent->modifiers();
    }
}

//==============================================================================
// Mouse Wheel Event
//==============================================================================
void ViewerWindow::wheelEvent(QWheelEvent* aEvent)
{
    // Check Event
    if (aEvent) {
#ifdef Q_OS_MACX
        Qt::KeyboardModifier modifier = Qt::MetaModifier;
#else // Q_OS_MACX
        Qt::KeyboardModifier modifier = Qt::ControlModifier;
#endif // Q_OS_MACX

        // Check Keyboard Modifiers
        if (keyModifiers & modifier) {
            // Check Angle Delta Y
            if (aEvent->angleDelta().y() > 0) {
                // Zoom In
                zoomIn();
            } else if (aEvent->angleDelta().y() < 0) {
                // Zoom Out
                zoomOut();
            }
        } else {
            // Check Angle Delta Y
            if (aEvent->angleDelta().y() > 0) {
                // Set Pan Pos Y
                setPanPosY(panPosY + DEFAULT_MOUSE_WHEEL_PAN_STEP);
            } else if (aEvent->angleDelta().y() < 0) {
                // Set Pan Pos Y
                setPanPosY(panPosY - DEFAULT_MOUSE_WHEEL_PAN_STEP);
            }

            // Check Angle Delta X
            if (aEvent->angleDelta().x() > 0) {
                // Set Pan Pos X
                setPanPosX(panPosX + DEFAULT_MOUSE_WHEEL_PAN_STEP);
            } else if (aEvent->angleDelta().x() < 0) {
                // Set Pan Pos X
                setPanPosX(panPosX - DEFAULT_MOUSE_WHEEL_PAN_STEP);
            }
        }
    }
}

//==============================================================================
// Mouse Presse Event
//==============================================================================
void ViewerWindow::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        if (aEvent->button() == Qt::LeftButton) {
            // Set Left Pressed
            leftPressed = true;

            // Save Last Pan Pos X
            panPosLastX = panPosX;
            // Save Last Pan Pos Y
            panPosLastY = panPosY;

            // Save Pan Press X
            panPressX = aEvent->pos().x();
            // Save Pan Press Y
            panPressY = aEvent->pos().y();
        }
    }
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void ViewerWindow::mouseMoveEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Left Pressed
        if (leftPressed) {
            // Set Pan Pos X
            setPanPosX(aEvent->pos().x() - panPressX + panPosLastX);
            // Set Pan Pos Y
            setPanPosY(aEvent->pos().y() - panPressY + panPosLastY);
        }
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void ViewerWindow::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        if (aEvent->button() == Qt::LeftButton) {
            // Set Left Pressed
            leftPressed = false;
        }
    }
}

//==============================================================================
// Resize Event
//==============================================================================
void ViewerWindow::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Zoom Fit
        if (zoomFit) {
            // Adjust Zoom Level To Fit
            adjustZoomLevelToFit();
        }
    }

    QMainWindow::resizeEvent(aEvent);
}

//==============================================================================
// Close Event
//==============================================================================
void ViewerWindow::closeEvent(QCloseEvent* aEvent)
{
    // Check Event
    if (aEvent) {

    }
}

//==============================================================================
// Hide Event
//==============================================================================
void ViewerWindow::hideEvent(QHideEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Emit Window Closed Signal
        emit windowClosed();
    }
}


//==============================================================================
// Destructor
//==============================================================================
ViewerWindow::~ViewerWindow()
{
    // Save Settings
    //saveSettings();

    // Delete Viewer Widget to Avoid Crash
    delete ui->viewerWidget;

    // De;ete UI
    delete ui;

    qDebug() << "ViewerWindow::~ViewerWindow";
}

