#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QString>
#include <QSettings>
#include <QThread>
#include <QTimerEvent>
#include <QMultiMap>
#include <QKeyEvent>
#include <QWheelEvent>

#include "constants.h"

namespace Ui {
class MainWindow;
}

class ViewerWindow;
class AboutForm;
class DirSelectorDialog;
class RenameFileDialog;
class InfoDialog;
class CompareDialog;
class Worker;

//==============================================================================
// Main Window
//==============================================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir WRITE setCurrentDir NOTIFY currentDirChanged)

    Q_PROPERTY(QString currentFileLeft READ getCurrentFileLeft WRITE setCurrentFileLeft NOTIFY currentFileLeftChanged)
    Q_PROPERTY(QString currentFileRight READ getCurrentFileRight WRITE setCurrentFileRight NOTIFY currentFileRightChanged)

    Q_PROPERTY(qreal opacityLeft READ getPpacityLeft WRITE setOpacityLeft NOTIFY opacityLeftChanged)
    Q_PROPERTY(qreal opacityRight READ getOpacityRight WRITE setOpacityRight NOTIFY opacityRightChanged)

    Q_PROPERTY(int zoomLevelIndex READ getZoomLevelIndex NOTIFY zoomLevelIndexChanged)
    Q_PROPERTY(qreal zoomLevel READ getZoomLevel NOTIFY zoomLevelChanged)

    Q_PROPERTY(qreal panPosX READ getPanPosX WRITE setPanPosY NOTIFY panPosXChanged)
    Q_PROPERTY(qreal panPosY READ getPanPosY WRITE setPanPosY NOTIFY panPosYChanged)

    Q_PROPERTY(bool manualPanning READ getManualPanning NOTIFY manualPanningChanged)

    Q_PROPERTY(qreal threshold READ getThreshold WRITE setThreshold NOTIFY thresholdChanged)

    Q_PROPERTY(bool hideSources READ getHideSources WRITE setHideSources NOTIFY hideSourcesChanged)

    Q_PROPERTY(bool showGrid READ getShowGrid WRITE setShowGrid NOTIFY showGridChanged)

public:

    // Static Constructor
    static MainWindow* getInstance();
    // Release Instance
    void release();

    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);
    // Get Current Dir
    QString getCurrentDir();

    // Get Current File Left
    QString getCurrentFileLeft();
    // Set Current File Left
    void setCurrentFileLeft(const QString& aCurrentFile);

    // Get Current File Right
    QString getCurrentFileRight();
    // Set Current File Right
    void setCurrentFileRight(const QString& aCurrentFile);

    // Get Left Image Opacity for Compositor View
    qreal getPpacityLeft();
    // Set Left Image Opacity for Compositor View
    void setOpacityLeft(const qreal& aOpacity);

    // Get Right Image Opacity for Compositor View
    qreal getOpacityRight();
    // Set Right Image Opacity for Compositor View
    void setOpacityRight(const qreal& aOpacity);

    // Get Zoom Level Index
    int getZoomLevelIndex();
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

    // Get Manual Panning
    bool getManualPanning();

    // Get Threshold
    qreal getThreshold();
    // Set Threshold
    void setThreshold(const qreal& aThreshold);

    // Get Hide Sources
    bool getHideSources();
    // Set Hide Sources
    void setHideSources(const bool& aHideSources);

    // Get Show Grid
    bool getShowGrid();
    // Set Show Grid
    void setShowGrid(const bool& aShowGrid);

protected:

    // Constructor
    explicit MainWindow(QWidget* aParent = NULL);

    // Destructor
    virtual ~MainWindow();

public slots:

    // Launch Viewer
    void launchViewer(const QString& aFilePath = QString(""));
    // Hide Viewer
    void hideViewer();

    // Toggle Viewer Fullscreen
    void toggleViewerFullScreen();

    // Launch Preferences
    void launchPreferences();

    // Launch About Dialog
    void launchAboutDialog();

    // Launch File Open Left
    void launchFileOpen(const QString& aSide);

    // Launch Help
    void launchHelp();

    // Show Window
    void showWindow();
    // Hide Window
    void hideWindow();

    // Show Status Text Slot
    void showStatusText(const QString& aStatusText, const int& aTimeout = DEFAULT_STATUS_TEXT_SHOW_TIMEOUT);

    // Handle File Open Event Slot
    void handleFileOpenEvent(const QString& aFileName);

    // Rotate Current/Selected Image(s) Left
    void rotateLeft();
    // Rotate Current/Selected Image(s) Right
    void rotateRigth();
    // Flip Current/Selected Image(s) Horizontally
    void flipHorizontally();
    // Flip Current/Selected Image(s) Vertically
    void flipVertically();

    // Stop Worker
    void stopWorkerThread();

    // Zoom In
    void zoomIn();
    // Zoom Out
    void zoomOut();
    // Zoom Default
    void zoomDefault();
    // Zoom To Fit
    void zoomToFit();

    // Reset Zoom & Pan Position
    void reset();

signals:

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);
    // Current Left File Changed Signal
    void currentFileLeftChanged(const QString& aCurrentFile);
    // Current Right File Changed Signal
    void currentFileRightChanged(const QString& aCurrentFile);

    // Left Image Opacity Changed Signal
    void opacityLeftChanged(const qreal& aOpacityLeft);
    // Right Image Opacity Changed Signal
    void opacityRightChanged(const qreal& aOpacityRight);

    // Zoom Level Index Changed Signal
    void zoomLevelIndexChanged(const int& aZoomLevelIndex);
    // Zoom Level Changed Signal
    void zoomLevelChanged(const qreal& aZoomLevel);

    // Pan Pos X Changed Signal
    void panPosXChanged(const qreal& aPanPosX);
    // Pan Pos Y Changed Signal
    void panPosYChanged(const qreal& aPanPosY);

    // Manual Panning Changed Signal
    void manualPanningChanged(const bool& aManualPanning);

    // Threshold Changed Signal
    void thresholdChanged(const qreal& aThreshold);

    // Hide Sources Changed Signal
    void hideSourcesChanged(const bool& aHideSources);

    // Show Grid Changed Signal
    void showGridChanged(const bool& aShowGrid);

    // Current File Updated Signal
    void currentFileUdated();

    // File Updated Signal
    void fileUpdated(const int& aIndex);

    // Deleting Index Signal
    void deletingIndex(const int& aIndex);
    // Index Deleted Signal
    void indexDeleted(const int& aIndex);

    // Operate Worker Signal
    void operateWorker(const int& aOperation);

private slots:

    // Init
    void init();

    // Worker Result Ready Slot
    void workerResultReady(const int& aOperation, const int& aResult);

    // View Window Closed Slot
    void viewerWindowClosed();

    // File Renamed Slot
    void fileRenamed(const QString& aFileName);

    // Pan Start Slot
    void panStart(const QPoint& aPos);
    // Pan Move Slot
    void panMove(const QPoint& aPos);
    // Pan Finished Slot
    void panFinished(const QPoint& aPos);

    // Set Manual Panning
    void setManualPanning(const bool& aManualPanning);

    // Compositor Double Clicked Slot
    void compositorDoubleClicked();
    // Side Viev Double Clicked Slot
    void sideViewDoubleClicked(const QString& aSide);

    // Set Zoom Level Index
    void setZoomLevelIndex(const int& aZoomLevelIndex, const bool& aForce = false);
    // Set Zoom Level
    void setZoomLevel(const qreal& aZoomLevel);

    // Composite Width Changed Slot
    void compositeWidthChanged(const qreal& aCompositeWidth);
    // Composite height Changed Slot
    void compositeHeightChanged(const qreal& aCompositeHeight);

    // Get Bounded Pan Pos X
    qreal boundedPanPosX(const qreal aPanPosX);
    // Get Bounded Pan Pos Y
    qreal boundedPanPosY(const qreal aPanPosY);

    // Update Pan Positions
    void updatePanPositions(const bool& aHorizontal = true, const bool& aVertical = true);

    // Open Left File Button Clicked Slot
    void on_openLeftButton_clicked();
    // Open Right File Button Clicked Slot
    void on_openRightButton_clicked();
    // Action Preferences Triggered Slot
    void on_actionPreferences_triggered();
    // Preferences Button Clicked Slot
    void on_optionsButton_clicked();
    // Action About Triggered Slot
    void on_actionAbout_triggered();
    // Action Rotate Left Triggered Slot
    void on_actionRotate_Left_triggered();
    // Action Rotate Right Triggered Slot
    void on_actionRotate_Right_triggered();
    // Action Flip Horizontally Triggered Slot
    void on_actionFlip_Horizontally_triggered();
    // Action Flip Vertically Triggered Slot
    void on_actionFlip_Vertically_triggered();
    // Action Quit Triggered Slot
    void on_actionQuit_triggered();
    // Reset Zoom & Panning Pos Button Clicked Slot
    void on_resetButton_clicked();
    // Zoom Out Button Clicked Slot
    void on_zoomOutButton_clicked();
    // Zoom In Button Clicked Slot
    void on_zoomInButton_clicked();
    // Zoom To Fit Button Clicked Slot
    void on_zoomToFitButton_clicked();
    // Action Launch Viewer Triggered Slot
    void on_actionViewer_triggered();
    // Action Launch Help Triggered Slot
    void on_actionHelp_triggered();
    // Action Open Left File Triggered Slot
    void on_actionOpen_File_Left_triggered();
    // Action Open Right File Triggered Slot
    void on_actionOpen_File_Right_triggered();
    // Action Close Left File Triggered Slot
    void on_actionClose_Left_File_triggered();
    // Action Close Right File Triggered Slot
    void on_actionClose_Right_File_triggered();
    // Action Zoom In Triggered Slot
    void on_actionZoom_In_triggered();
    // Action Zoom Out Triggered Slot
    void on_actionZoom_Out_triggered();
    // Action Zoom To Default Triggered Slot
    void on_actionZoom_Default_triggered();
    // Action Zoom to Fit Triggered Slot
    void on_actionZoom_To_Fit_triggered();
    // Action Reset Triggered Slot
    void on_actionReset_triggered();
    // Exit Button Clicked Slot
    void on_exitButton_clicked();

private:
    friend class Worker;

    // Restore UI
    void restoreUI();
    // Update Menu
    void updateMenu();

    // Init Worker
    void initWorker();

    // Save Settings
    void saveSettings();

    // Rotate Current/Selected Image(s) Left
    void doRotateLeft();
    // Rotate Current/Selected Image(s) Right
    void doRotateRigth();
    // Flip Current/Selected Image(s) Horizontally
    void doFlipHorizontally();
    // Flip Current/Selected Image(s) Vertically
    void doFlipVertically();

protected:

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

    // Mouse Wheel Event
    virtual void wheelEvent(QWheelEvent* aEvent);

private:

    // UI
    Ui::MainWindow*                 ui;
    // File System Model
    QFileSystemModel*               fsModel;

    // Current Dir
    QString                         currentDir;
    // Current Left File
    QString                         currentFileLeft;
    // Current Right File
    QString                         currentFileRight;

    // Last Open File Path
    QString                         lastOpenPath;

    // Left Image Opacity
    qreal                           opacityLeft;
    // Right Image Opacity
    qreal                           opacityRight;

    // Zoom to Fit
    bool                            zoomFit;
    // Current Zoom LEvel Index
    int                             zoomLevelIndex;
    // Zoom Level
    qreal                           zoomLevel;
    // Previous Zoom Level
    qreal                           prevZoomLevel;

    // Pan Press X
    qreal                           panPressX;
    // Pan Press Y
    qreal                           panPressY;

    // Last Pan Pos X
    qreal                           panPosLastX;
    // Last Pan Pos Y
    qreal                           panPosLastY;

    // Pan Pos X
    qreal                           panPosX;
    // Pan Pos Y
    qreal                           panPosY;

    // Previous Pan Pos X
    qreal                           prevPanPosX;
    // Previous Pan Pos Y
    qreal                           prevPanPosY;

    // Manual Panning
    bool                            manualPanning;

    // Composite Width
    qreal                           compositeWidth;
    // Composite Height
    qreal                           compositeHeight;

    // Last Threshold
    qreal                           lastThreshold;
    // Compare Threshold
    qreal                           threshold;

    // Hide Sources In Compare View
    bool                            hideSources;

    // Show Grid
    bool                            showGrid;

    // Viewer Window
    ViewerWindow*                   viewerWindow;
    // About Form
    AboutForm*                      aboutDialog;

    // Dir Selector Dialog
    DirSelectorDialog*              dirSelector;

    // Rename File Dialog
    RenameFileDialog*               renameFileDialog;
    // Info Dialog
    InfoDialog*                     infoDialog;

    // Selected Transfer Dir
    QString                         transferDir;

    // File Operation Worker
    Worker*                         worker;

    // Worker Thread
    QThread                         workerThread;

    // Transfer Options
    int                             transferOptions;

    // Keyboard Modifiers
    Qt::KeyboardModifiers           keyModifiers;
};

#endif // MAINWINDOW_H
