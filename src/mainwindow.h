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

    Q_PROPERTY(qreal zoomLevel READ getZoomLevel /*WRITE setZoomLevel*/ NOTIFY zoomLevelChanged)

    Q_PROPERTY(qreal panPosX READ getPanPosX WRITE setPanPosY NOTIFY panPosXChanged)
    Q_PROPERTY(qreal panPosY READ getPanPosY WRITE setPanPosY NOTIFY panPosYChanged)

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

    // Update Pan Positions
    void updatePanPositions();

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

    // Zoom Level Changed Signal
    void zoomLevelChanged(const qreal& aZoomLevel);

    // Pan Pos X Changed Signal
    void panPosXChanged(const qreal& aPanPosX);
    // Pan Pos Y Changed Signal
    void panPosYChanged(const qreal& aPanPosY);

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

    // File Renamed Slot
    void fileRenamed(const QString& aFileName);

    // Pan Start Slot
    void panStart(const QPoint& aPos);
    // Pan Move Slot
    void panMove(const QPoint& aPos);
    // Pan Finished Slot
    void panFinished(const QPoint& aPos);

    // Compositor Double Clicked Slot
    void compositorDoubleClicked();
    // Side Viev Double Clicked Slot
    void sideViewDoubleClicked(const QString& aSide);

    // Set Zoom Level
    void setZoomLevel(const qreal& aZoomLevel);

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
    // Exit Button Clicked Slot
    void on_exitButton_clicked();

protected:

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

    // Mouse Wheel Event
    virtual void wheelEvent(QWheelEvent* aEvent);

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

    Qt::KeyboardModifiers           keyModifiers;
};

#endif // MAINWINDOW_H
