#ifndef MAINVIEWERWINDOW_H
#define MAINVIEWERWINDOW_H

#include <QMainWindow>
#include <QQmlContext>

#include "constants.h"

namespace Ui {
class ViewerWindow;
}

class MainWindow;


//==============================================================================
// Viewer Window Class
//==============================================================================
class ViewerWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(QString currentFile READ getCurrentFile WRITE setCurrentFile NOTIFY currentFileChanged)

public:

    // Static Constructor
    static ViewerWindow* getInstance();
    // Release Instance
    void release();

    // Get Current File
    QString getCurrentFile();
    // Set Current File
    void setCurrentFile(const QString& aCurrentFile);

signals:

    // Current File Changed Slot
    void currentFileChanged(const QString& aFilePath);

public slots:

    // Launc Preferences
    void launcPreferences();
    // Show Window
    void showWindow();
    // Hide Window
    void hideWindow();
    // Show Status Text
    void showStatusText(const QString& aStatusText, const int& aTimeout = DEFAULT_STATUS_TEXT_SHOW_TIMEOUT);

protected:

    // Constructor
    explicit ViewerWindow(QWidget* aParent = NULL);

    // Destructor
    virtual ~ViewerWindow();

private slots:

    // On Action About Triggered
    void on_actionAbout_triggered();
    // On Action Preferences Triggered
    void on_actionPreferences_triggered();
    // On Action Rename Triggered
    void on_actionRename_triggered();
    // On Action Delete Triggered
    void on_actionDelete_triggered();
    // On Action Rotate Left Triggered
    void on_actionRotate_Left_triggered();
    // On Action Rotate Rigth Triggered
    void on_actionRotate_Right_triggered();
    // On Action Flip Horizontally Triggered
    void on_actionFlip_Horizontally_triggered();
    // On Action Flip Vertically Triggered
    void on_actionFlip_Vertically_triggered();
    // On Action Quit Triggered
    void on_actionQuit_triggered();

private:
    friend class MainWindow;

    // Restore UI
    void restoreUI();

    // Save Settings
    void saveSettings();

private:

    // UI
    Ui::ViewerWindow*       ui;

    // Main Browser Window
    MainWindow*             mainWindow;

    // Current File
    QString                 currentFile;

    // Current Zoom LEvel Index
    int                     zoomLevelIndex;
    // Zoom Level
    qreal                   zoomLevel;

    // Pan Press X
    qreal                   panPressX;
    // Pan Press Y
    qreal                   panPressY;

    // Last Pan Pos X
    qreal                   panPosLastX;
    // Last Pan Pos Y
    qreal                   panPosLastY;

    // Pan Pos X
    qreal                   panPosX;
    // Pan Pos Y
    qreal                   panPosY;

};

#endif // MAINVIEWERWINDOW_H
