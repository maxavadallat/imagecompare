#ifndef MAINVIEWERWINDOW_H
#define MAINVIEWERWINDOW_H

#include <QMainWindow>
#include <QQmlContext>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>

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

    Q_PROPERTY(qreal zoomLevel READ getZoomLevel NOTIFY zoomLevelChanged)

    Q_PROPERTY(qreal panPosX READ getPanPosX WRITE setPanPosY NOTIFY panPosXChanged)
    Q_PROPERTY(qreal panPosY READ getPanPosY WRITE setPanPosY NOTIFY panPosYChanged)

    Q_PROPERTY(qreal imageWidth READ getImageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(qreal imageHeight READ getImageHeight WRITE setImageHeight NOTIFY imageHeightChanged)

    Q_PROPERTY(qreal imageSourceWidth READ getSourceWidth WRITE setSourceWidth NOTIFY sourceWidthChanged)
    Q_PROPERTY(qreal imageSourceHeight READ getSourceHeight WRITE setSourceHeight NOTIFY sourceHeightChanged)


public:

    // Static Constructor
    static ViewerWindow* getInstance();
    // Release Instance
    void release();

    // Get Current File
    QString getCurrentFile();
    // Set Current File
    void setCurrentFile(const QString& aCurrentFile);

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

    // Get Image Width
    qreal getImageWidth();
    // Set Image Width
    void setImageWidth(const qreal& aWidth);

    // Get Image Height
    qreal getImageHeight();
    // Set Image Height
    void setImageHeight(const qreal& aHeight);

    // Get Source  Width
    qreal getSourceWidth();
    // Set Source  Width
    void setSourceWidth(const qreal& aWidth);

    // Get Source  Height
    qreal getSourceHeight();
    // Set Source  Height
    void setSourceHeight(const qreal& aHeight);

signals:

    // Current File Changed Slot
    void currentFileChanged(const QString& aFilePath);

    // Zoom Level Changed Signal
    void zoomLevelChanged(const qreal& aZoomLevel);

    // Pan Pos X Changed Signal
    void panPosXChanged(const qreal& aPanPosX);
    // Pan Pos Y Changed Signal
    void panPosYChanged(const qreal& aPanPosY);

    // Image Width Changed Signal
    void imageWidthChanged(const qreal& aWidth);
    // Image Height changed Signal
    void imageHeightChanged(const qreal& aHeight);

    // Source  Width Changed Signal
    void sourceWidthChanged(const qreal& aSourceWidth);
    // Source  Height Changed Signal
    void sourceHeightChanged(const qreal& aSourceHeight);

    // Window Closed Signal
    void windowClosed();

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
    // Update Menu
    void updateMenu();

    // Save Settings
    void saveSettings();

    // Zoom In
    void zoomIn();
    // Zoom Out
    void zoomOut();
    // Zoom Default
    void zoomDefault();
    // Zoom To Fit
    void zoomToFit();
    // Reset
    void reset();

    // Adjust Zoom Level To Fit
    void adjustZoomLevelToFit();

    // Set Zoom Level
    void setZoomLevel(const qreal& aZoomLevel);

protected:

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);
    // Mouse Wheel Event
    virtual void wheelEvent(QWheelEvent* aEvent);

    // Mouse Presse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Move Event
    virtual void mouseMoveEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    // Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);

    // Close Event
    virtual void closeEvent(QCloseEvent* aEvent);
    // Hide Event
    virtual void hideEvent(QHideEvent* aEvent);

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
    // Zoom Fit
    bool                    zoomFit;

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

    // Left Mouse Button Pressed
    bool                    leftPressed;

    // Image Width
    qreal                   imageWidth;
    // Image Height
    qreal                   imageHeight;

    // Image Width
    qreal                   imageSourceWidth;
    // Image Height
    qreal                   imageSourceHeight;

    // Keyboard Modifiers
    Qt::KeyboardModifiers   keyModifiers;

};

#endif // MAINVIEWERWINDOW_H
