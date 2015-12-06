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
    , panPressX(0.0)
    , panPressY(0.0)
    , panPosLastX(0.0)
    , panPosLastY(0.0)
    , panPosX(0.0)
    , panPosY(0.0)
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
// Save Settings
//==============================================================================
void ViewerWindow::saveSettings()
{
    qDebug() << "ViewerWindow::saveSettings";

    // ...

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

