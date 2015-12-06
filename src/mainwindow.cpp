#include <QDir>
#include <QDebug>
#include <QVariant>
#include <QFileInfo>
#include <QFileInfoList>
#include <QQmlContext>
#include <QMutexLocker>
#include <QModelIndex>
#include <QSettings>
#include <QFileDialog>

#include <CoreServices/CoreServices.h>

#include "mainwindow.h"
#include "viewerwindow.h"
#include "preferencesdialog.h"
#include "aboutform.h"
#include "renamefiledialog.h"
#include "infodialog.h"
#include "confirmationdialog.h"
#include "dirselectordialog.h"
#include "worker.h"
#include "utility.h"
#include "constants.h"
#include "defaultsettings.h"

#include "ui_mainwindow.h"

// Main Window Singleton
static MainWindow* mainWindow = NULL;

//==============================================================================
// Static Constructor
//==============================================================================
MainWindow* MainWindow::getInstance()
{
    // Check Singleton
    if (!mainWindow) {
        // Create Main Browser Window
        mainWindow = new MainWindow();
    }

    return mainWindow;
}

//==============================================================================
// Release Instance
//==============================================================================
void MainWindow::release()
{
    // Delete Main Browser Window
    delete mainWindow;
    // Reset Singleton
    mainWindow = NULL;
}

//==============================================================================
// Constructor
//==============================================================================
MainWindow::MainWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainWindow)
    , fsModel(NULL)
    , currentDir("")
    , currentFileLeft("")
    , currentFileRight("")
    , lastOpenPath(QDir::homePath())
    , opacityLeft(DEFAULT_IMAGE_OPACITY_LEFT)
    , opacityRight(DEFAULT_IMAGE_OPACITY_RIGHT)
    , zoomFit(false)
    , zoomLevelIndex(DEFAULT_ZOOM_LEVEL_INDEX)
    , zoomLevel(0.00)
    , viewerWindow(NULL)
    , aboutDialog(NULL)
    , dirSelector(NULL)
    , renameFileDialog(NULL)
    , infoDialog(NULL)
    , transferDir("")
    , worker(NULL)
{
    // Setup UI
    ui->setupUi(this);

    // Init
    init();

    // Restore UI
    //restoreUI();

    // ...
}

//==============================================================================
// Init
//==============================================================================
void MainWindow::init()
{
    // Get Root Context
    QQmlContext* leftContext = ui->leftView->rootContext();
    // Set Context Property
    leftContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);
    // Set Side
    ui->leftView->setSide(CONTEXT_PROPERTY_SIDE_VALUE_LEFT);
    // Set Source
    ui->leftView->setSource(QUrl(QML_SOURCE_SIDE_VIEW));

    // Connect Signals
    connect(ui->leftView, SIGNAL(doubleClicked(QString)), this, SLOT(sideViewDoubleClicked(QString)));

    connect(ui->leftView, SIGNAL(mousePressed(QPoint)), this, SLOT(panStart(QPoint)));
    connect(ui->leftView, SIGNAL(mouseMoved(QPoint)), this, SLOT(panMove(QPoint)));
    connect(ui->leftView, SIGNAL(mouseReleased(QPoint)), this, SLOT(panFinished(QPoint)));

    // ...


    // Get Root Context
    QQmlContext* centerContext = ui->centerView->rootContext();
    // Set Context Property
    centerContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);
    // Set Source
    ui->centerView->setSource(QUrl(QML_SOURCE_COMPOSITE_VIEW));

    // Connect Signals
    connect(ui->centerView, SIGNAL(doubleClicked()), this, SLOT(compositorDoubleClicked()));

    connect(ui->centerView, SIGNAL(mousePressed(QPoint)), this, SLOT(panStart(QPoint)));
    connect(ui->centerView, SIGNAL(mouseMoved(QPoint)), this, SLOT(panMove(QPoint)));
    connect(ui->centerView, SIGNAL(mouseReleased(QPoint)), this, SLOT(panFinished(QPoint)));

    // ...


    // Get Root Context
    QQmlContext* rightContext = ui->rightView->rootContext();
    // Set Context Property
    rightContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);
    // Set Side
    ui->rightView->setSide(CONTEXT_PROPERTY_SIDE_VALUE_RIGHT);
    // Set Source
    ui->rightView->setSource(QUrl(QML_SOURCE_SIDE_VIEW));

    // Connect Signals
    connect(ui->rightView, SIGNAL(doubleClicked(QString)), this, SLOT(sideViewDoubleClicked(QString)));

    connect(ui->rightView, SIGNAL(mousePressed(QPoint)), this, SLOT(panStart(QPoint)));
    connect(ui->rightView, SIGNAL(mouseMoved(QPoint)), this, SLOT(panMove(QPoint)));
    connect(ui->rightView, SIGNAL(mouseReleased(QPoint)), this, SLOT(panFinished(QPoint)));

    // Set Zoom Level Index
    zoomDefault();

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI()
{
    qDebug() << "MainWindow::restoreUI";

    // Init Settings
    QSettings settings;

//    // Get Splitter Value
//    QByteArray splitterValue = settings.value(SETTINGS_KEY_MAIN_SPLITTER).toByteArray();

//    // Check Splitter Value
//    if (!splitterValue.isNull()) {
//        // Restore Main Splitter
//        ui->mainSplitter->restoreState(splitterValue);
//    }

//    // Get Splitter Value
//    splitterValue = settings.value(SETTINGS_KEY_LEFT_SPLITTER).toByteArray();

//    // Check Splitter Value
//    if (!splitterValue.isNull()) {
//        // Restore Left Pane Splitter
//        ui->leftPaneSplitter->restoreState(splitterValue);
//    }

    // Get Last Dir From Settings
    lastOpenPath = settings.value(SETTINGS_KEY_LAST_DIR).toString();
    // Check Last Dir
    if (!QFile::exists(lastOpenPath)) {
        // Reset Last Dir
        lastOpenPath = QDir::homePath();
    }

    // Set Current Dir
    setCurrentDir(lastOpenPath);

    // ...

    // Update Menu
    updateMenu();

    // ...

    // Grab Keyboard Focus
    grabKeyboard();
}

//==============================================================================
// Update Menu
//==============================================================================
void MainWindow::updateMenu()
{
    // Set Button Enabled
    ui->zoomOutButton->setEnabled(zoomLevelIndex > 0);
    ui->zoomInButton->setEnabled(zoomLevelIndex < DEFAULT_ZOOM_LEVEL_INDEX_MAX);
    ui->zoomToFitButton->setEnabled(!zoomFit);

    // ...
}

//==============================================================================
// Set Current Dir
//==============================================================================
void MainWindow::setCurrentDir(const QString& aCurrentDir)
{
    // Check Current Dir
    if (currentDir != aCurrentDir) {
        // Stop Worker Thread
        stopWorkerThread();

        // Check If Dir Exists
        if (QFile::exists(aCurrentDir)) {
            //qDebug() << "MainWindow::setCurrentDir - aCurrentDir: " << aCurrentDir;

            // Set Current Dir
            currentDir = aCurrentDir;
        } else {
            //qDebug() << "MainWindow::setCurrentDir - homePath: " << QDir::homePath();

            // Set Current Dir
            currentDir = QDir::homePath();
        }

        // Emit Current Dir Changed Signal
        emit currentDirChanged(currentDir);

        // Set Window Title
        setWindowTitle(QString(DEFAULT_APPLICATION_NAME) + QString(" - ") + currentDir);

        // Show Status Bar Message
        //ui->statusBar->showMessage(QString("Current Dir Changed to: %1. %2 Images found.").arg(currentDir).arg(browserDataModel.count()));

        // ...
    }
}

//==============================================================================
// Get Current Dir
//==============================================================================
QString MainWindow::getCurrentDir()
{
    return currentDir;
}

//==============================================================================
// Get Current File Left
//==============================================================================
QString MainWindow::getCurrentFileLeft()
{
    return currentFileLeft;
}

//==============================================================================
// Set Current File Left
//==============================================================================
void MainWindow::setCurrentFileLeft(const QString& aCurrentFile)
{
    // Check Current File
    if (currentFileLeft != aCurrentFile) {
        qDebug() << "MainWindow::setCurrentFileLeft - aCurrentFile: " << aCurrentFile;
        // Set Current File
        currentFileLeft = aCurrentFile;

        // Emit Current File Changed Signal
        emit currentFileLeftChanged(currentFileLeft);

        // ...
    }
}

//==============================================================================
// Get Current File Right
//==============================================================================
QString MainWindow::getCurrentFileRight()
{
    return currentFileRight;
}

//==============================================================================
// Set Current File Right
//==============================================================================
void MainWindow::setCurrentFileRight(const QString& aCurrentFile)
{
    // Check Current File
    if (currentFileRight != aCurrentFile) {
        qDebug() << "MainWindow::setCurrentFileRight - aCurrentFile: " << aCurrentFile;
        // Set Current File
        currentFileRight = aCurrentFile;

        // Emit Current File Changed Signal
        emit currentFileRightChanged(currentFileRight);

        // ...
    }
}

//==============================================================================
// Get Left Image Opacity for Compositor View
//==============================================================================
qreal MainWindow::getPpacityLeft()
{
    return opacityLeft;
}

//==============================================================================
// Set Left Image Opacity for Compositor View
//==============================================================================
void MainWindow::setOpacityLeft(const qreal& aOpacity)
{
    // Check Opacity
    if (opacityLeft != aOpacity) {
        // Set Opacity
        opacityLeft = aOpacity;
        // Emit Opacity Changed Signal
        emit opacityLeftChanged(opacityLeft);
    }
}

//==============================================================================
// Get Right Image Opacity for Compositor View
//==============================================================================
qreal MainWindow::getOpacityRight()
{
    return opacityRight;
}

//==============================================================================
// Set Right Image Opacity for Compositor View
//==============================================================================
void MainWindow::setOpacityRight(const qreal& aOpacity)
{
    // Check Opacity
    if (opacityRight != aOpacity) {
        // Set Opacity
        opacityRight = aOpacity;
        // Emit Opacity Changed Signal
        emit opacityRightChanged(opacityRight);
    }
}

//==============================================================================
// Get Zoom Level
//==============================================================================
qreal MainWindow::getZoomLevel()
{
    return zoomLevel;
}

//==============================================================================
// Set Zoom Level
//==============================================================================
void MainWindow::setZoomLevel(const qreal& aZoomLevel)
{
    // Check Current Zoom LEvel
    if (zoomLevel != aZoomLevel) {
        //qDebug() << "MainWindow::setZoomLevel - aZoomLevel: " << aZoomLevel * 100 << "%";

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
// Refresh View
//==============================================================================
void MainWindow::refreshView(const int& aIndex)
{
    qDebug() << "MainWindow::refreshView";

    // ...
}

//==============================================================================
// Update Pan Positions
//==============================================================================
void MainWindow::updatePanPositions()
{
    qDebug() << "MainWindow::updatePanPositions";

    // Set Pan Pos X for Boundaries Adjusted
    setPanPosX(panPosX);
    // Set Pan Pos Y for Boundaries Adjusted
    setPanPosY(panPosY);

}

//==============================================================================
// Get Pan Pos X
//==============================================================================
qreal MainWindow::getPanPosX()
{
    return panPosX;
}

//==============================================================================
// Set Pan Pos X
//==============================================================================
void MainWindow::setPanPosX(const qreal& aPanPosX)
{
    // Check Center View
    if (!ui->centerView) {
        return;
    }

    // Calculate Boundary
    qreal bound = qMax(ui->centerView->getCompositeWidth() / 2 - (qreal)ui->centerView->width() / 2, 0.0);

    //qDebug() << "MainWindow::setPanPosX - bound: " << bound;

    // Calculate Min Bound
    qreal minBound = -bound;
    // Calculate Max Bound
    qreal maxBound = bound;

    // Get New Pan Pos X
    qreal newPanPosX = qBound(minBound, aPanPosX, maxBound);

    // Check Current Pan Pos X
    if (panPosX != newPanPosX) {
        //qDebug() << "MainWindow::setPanPosX - newPanPosX: " << newPanPosX;

        // Set Current Pan Pos X
        //panPosX = aPanPosX;
        panPosX = newPanPosX;

        // Emit Pan Pos X Changed Signal
        emit panPosXChanged(panPosX);

        // ...
    }
}

//==============================================================================
// Get Pan Pos Y
//==============================================================================
qreal MainWindow::getPanPosY()
{
    return panPosY;
}

//==============================================================================
// Set Pan Pos Y
//==============================================================================
void MainWindow::setPanPosY(const qreal& aPanPosY)
{
    // Check Center View
    if (!ui->centerView) {
        return;
    }

    // Calculate Boundary
    qreal bound = qMax(ui->centerView->getCompositeHeight() / 2 - (qreal)ui->centerView->height() / 2, 0.0);

    //qDebug() << "MainWindow::setPanPosY - bound: " << bound;

    // Calculate Min Bound
    qreal minBound = -bound;
    // Calculate Max Bound
    qreal maxBound = bound;

    // Get New Pan Pos Y
    qreal newPanPosY = qBound(minBound, aPanPosY, maxBound);

    // Check Current Pan Pos X
    if (panPosY != newPanPosY) {
        //qDebug() << "MainWindow::setPanPosY - newPanPosY: " << newPanPosY;

        // Set Current Pan Pos X
        panPosY = newPanPosY;

        // Emit Pan Pos X Changed Signal
        emit panPosYChanged(panPosY);

        // ...
    }
}

//==============================================================================
// Init Worker
//==============================================================================
void MainWindow::initWorker()
{
    // Check Worker Thread
    if (workerThread.isRunning()) {
        // Terminate Worker Thread
        workerThread.terminate();
    }

    // Check Worker
    if (!worker) {
        qDebug() << "MainWindow::initWorker";
        // Create Worker
        worker = new Worker();

        // Connect Signals
        connect(worker, SIGNAL(resultReady(int,int)), this, SLOT(workerResultReady(int,int)), Qt::QueuedConnection);
        connect(this, SIGNAL(operateWorker(int)), worker, SLOT(doWork(int)));
        connect(worker, SIGNAL(fileRenamed(QString)), this, SLOT(fileRenamed(QString)));
        connect(worker, SIGNAL(refreshView()), this, SLOT(refreshView()));

        // ...

        // Move To Thread
        worker->moveToThread(&workerThread);
    }

    // Start Worker Thread
    workerThread.start();
}

//==============================================================================
// Save Settings
//==============================================================================
void MainWindow::saveSettings()
{
    qDebug() << "MainWindow::saveSettings";

    // Init Settings
    QSettings settings;

    // Save Last Dir
    settings.setValue(SETTINGS_KEY_LAST_DIR, currentDir);

    // Save Last Files
    settings.setValue(SETTINGS_KEY_LAST_FILE_LEFT, currentFileLeft);
    settings.setValue(SETTINGS_KEY_LAST_FILE_RIGHT, currentFileRight);

    // Save Main Splitter State
    //settings.setValue(SETTINGS_KEY_MAIN_SPLITTER, ui->mainSplitter->saveState());

    // Save Left Splitter
    //settings.setValue(SETTINGS_KEY_LEFT_SPLITTER, ui->leftPaneSplitter->saveState());

    settings.setValue(SETTINGS_KEY_LAST_DIR, lastOpenPath);

    // Get Maximized State
    bool viewMaximized = windowState() == Qt::WindowMaximized;

    // Save Maximized State
    settings.setValue(SETTINGS_KEY_VIEW_MAXIMIZED, viewMaximized);

    // ...
}

//==============================================================================
// Rotate Current Image Left
//==============================================================================
void MainWindow::rotateLeft()
{
    qDebug() << "MainWindow::rotateLeft";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTRotateFilesLeft);
}

//==============================================================================
// Rotate Current Image Right
//==============================================================================
void MainWindow::rotateRigth()
{
    qDebug() << "MainWindow::rotateRigth";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTRotateFilesRight);
}

//==============================================================================
// Flip Current Image Horizontally
//==============================================================================
void MainWindow::flipHorizontally()
{
    qDebug() << "MainWindow::flipHorizontally";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTFlipFilesHorizontally);
}

//==============================================================================
// Flip Current Image Vertically
//==============================================================================
void MainWindow::flipVertically()
{
    qDebug() << "MainWindow::flipVertically";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTFlipFilesVertically);
}

//==============================================================================
// Delete Current/Selected File(s)
//==============================================================================
void MainWindow::deleteFile()
{
    qDebug() << "MainWindow::deleteFile";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTDeleteFiles);
}

//==============================================================================
// Delete Current/Selected File
//==============================================================================
void MainWindow::renameFile()
{
    qDebug() << "MainWindow::renameFile";

    // Init Generic Dialog
    QDialog* dialog = NULL;

//    // Check If Model Has Selection
//    if (hasSelection()) {

//        // Check Rename Series Dialog
//        if (!renameSeriesDialog) {
//            // Create Rename Series Dialog
//            renameSeriesDialog = new RenameSeriesDialog;
//        }

//        // Set Dialog
//        dialog = renameSeriesDialog;

//    } else {

//        // Check REname File Dialog
//        if (!renameFileDialog) {
//            // Create Rename File Dialog
//            renameFileDialog = new RenameFileDialog;
//        }

//        // Init index
//        int index = popupIndex >= 0 ? popupIndex : currentIndex ;
//        // Set File Name
//        QString fileName = static_cast<BrowserDataObject*>(browserDataModel[index])->fileName;
//        // Set File Name
//        renameFileDialog->setFileName(fileName);

//        // Set Dialog
//        dialog = renameFileDialog;
//    }

//    // Check Dialog
//    if (dialog && dialog->exec()) {

//        // Evaluate File Rename Pattern if Has Selection
//        if (hasSelection() && !evaluateFileRenamePattern()) {
//            return;
//        }

//        // Do Rename File
//        //doRenameFile();

//        // Init Worker
//        initWorker();

//        // Emit Operate Worker Signal
//        emit operateWorker(OTRenameFiles);
//    }
}

//==============================================================================
// Copy To Current/Selected File(s)
//==============================================================================
void MainWindow::copyToDirectory()
{
    qDebug() << "MainWindow::copyToDirectory";

    // Check Dir Selector
    if (!dirSelector) {
        // Create Dir Selector
        dirSelector = new DirSelectorDialog();
    }

    // Check Dir Selector
    if (dirSelector) {
        // Set Title
        dirSelector->setWindowTitle(QString("Copy To Directory..."));

        // Exec Dir Selector Dialog
        if (dirSelector->exec()) {
            // Init Worker
            initWorker();

            // Emit Operate Worker Signal
            emit operateWorker(OTCopyToFiles);
        }
    }
}

//==============================================================================
// Move To Current/Selected File(s)
//==============================================================================
void MainWindow::moveToDirectory()
{
    qDebug() << "MainWindow::moveToDirectory";

    // Check Dir Selector
    if (!dirSelector) {
        // Create Dir Selector
        dirSelector = new DirSelectorDialog();
    }

    // Check Dir Selector
    if (dirSelector) {
        // Set Title
        dirSelector->setWindowTitle(QString("Move To Directory..."));

        // Exec Dir Selector Dialog
        if (dirSelector->exec()) {
            // Do Move To Directory
            //doMoveToDirectory();

            // Init Worker
            initWorker();

            // Emit Operate Worker Signal
            emit operateWorker(OTMoveToFiles);
        }
    }
}

//==============================================================================
// Rotate Current/Selected Image(s) Left
//==============================================================================
void MainWindow::doRotateLeft()
{
//    // Init index
//    int index = currentIndex ;

//    // Check Popup Index
//    if (popupIndex != -1) {
//        qDebug() << "MainWindow::doRotateLeft - popupIndex: " << popupIndex;

//        // Set Index
//        index = popupIndex;
//        // Reset Popup Index
//        popupIndex = -1;

//    } else {
//        qDebug() << "MainWindow::doRotateLeft - currentIndex: " << currentIndex;
//    }

//    // Rotate By Index
//    rotateFileByIndex(index, RDTLeft);
}

//==============================================================================
// Rotate Current/Selected Image(s) Right
//==============================================================================
void MainWindow::doRotateRigth()
{
//    // Init index
//    int index = currentIndex ;

//    // Check Popup Index
//    if (popupIndex != -1) {
//        qDebug() << "MainWindow::doRotateRigth - popupIndex: " << popupIndex;

//        // Set Index
//        index = popupIndex;
//        // Reset Popup Index
//        popupIndex = -1;

//    } else {
//        qDebug() << "MainWindow::doRotateRigth - currentIndex: " << currentIndex;
//    }

//    // Rotate By Index
//    rotateFileByIndex(index, RDTRight);
}

//==============================================================================
// Flip Current/Selected Image(s) Horizontally
//==============================================================================
void MainWindow::doFlipHorizontally()
{
//    // Init index
//    int index = currentIndex ;

//    // Check Popup Index
//    if (popupIndex != -1) {
//        qDebug() << "MainWindow::doFlipHorizontally - popupIndex: " << popupIndex;

//        // Set Index
//        index = popupIndex;
//        // Reset Popup Index
//        popupIndex = -1;

//    } else {
//        qDebug() << "MainWindow::doFlipHorizontally - currentIndex: " << currentIndex;
//    }

//    // Flip By Index
//    flipFileByIndex(index, FDTHorizontal);
}

//==============================================================================
// Flip Current/Selected Image(s) Vertically
//==============================================================================
void MainWindow::doFlipVertically()
{
//    // Init index
//    int index = currentIndex ;

//    // Check Popup Index
//    if (popupIndex != -1) {
//        qDebug() << "MainWindow::doFlipVertically - popupIndex: " << popupIndex;

//        // Set Index
//        index = popupIndex;
//        // Reset Popup Index
//        popupIndex = -1;

//    } else {
//        qDebug() << "MainWindow::doFlipVertically - currentIndex: " << currentIndex;
//    }

//    // Flip By Index
//    flipFileByIndex(index, FDTVertical);
}

//==============================================================================
// Delete Current/Selected File(s)
//==============================================================================
void MainWindow::doDeleteFile()
{
//    // Init index
//    int index = currentIndex ;

//    // Check Popup Index
//    if (popupIndex != -1) {
//        qDebug() << "MainWindow::doDeleteFile - popupIndex: " << popupIndex;

//        // Set Index
//        index = popupIndex;
//        // Reset Popup Index
//        popupIndex = -1;

//    } else {
//        qDebug() << "MainWindow::doDeleteFile - currentIndex: " << currentIndex;
//    }

//    // Delete File By Index
//    deleteFileByIndex(index);
}

//==============================================================================
// Rename Current/Selected File
//==============================================================================
void MainWindow::doRenameFile()
{
//    // Init index
//    int index = currentIndex ;

//    // Check Popup Index
//    if (popupIndex != -1) {
//        qDebug() << "MainWindow::doRenameFile - popupIndex: " << popupIndex;

//        // Set Index
//        index = popupIndex;
//        // Reset Popup Index
//        popupIndex = -1;

//    } else {
//        qDebug() << "MainWindow::doRenameFile - currentIndex: " << currentIndex;
//    }

//    // Rename File By Index
//    renameFileByIndex(index, renameFileDialog->getFileName());
}

//==============================================================================
// Copy Current/Selected File(s)
//==============================================================================
void MainWindow::doCopyToDirectory()
{
//    // Check Dir Selector & Selected Dir
//    if (dirSelector && dirSelector->getSelectedDir() != currentDir) {
//        qDebug() << "MainWindow::doCopyToDirectory - selectedDir: " << dirSelector->getSelectedDir();

//        // Reset Transfer Options
//        transferOptions = 0;

//        // Check If Browser Grid Has Selections
//        if (hasSelection()) {
//            // Get Browser Data Model Count
//            int bdmCount = browserDataModel.count();
//            // Get Selected Item Count
//            int selectedCount = numberOfSelected();
//            // Init Selected Index
//            int selectedIndex = 0;

//            // Got Thru Model
//            for (int i=0; i<bdmCount; i++) {
//                // Get Item
//                BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
//                // Check If Selected
//                if (item->fileSelected) {
//                    // Reset File Selected
//                    //item->fileSelected = false;
//                    // Copy By Index
//                    copyFileByIndex(i, dirSelector->getSelectedDir());
//                    // Inc Selected Index
//                    selectedIndex++;
//                    // Emit Progress Changed Signal
//                    emit progressChanged(selectedCount, selectedIndex);
//                }
//            }

//        } else {
//            // Init index
//            int index = currentIndex ;

//            // Check Popup Index
//            if (popupIndex != -1) {
//                qDebug() << "MainWindow::doCopyToDirectory - popupIndex: " << popupIndex;

//                // Set Index
//                index = popupIndex;
//                // Reset Popup Index
//                popupIndex = -1;

//            } else {
//                qDebug() << "MainWindow::doCopyToDirectory - currentIndex: " << currentIndex;
//            }

//            // Copy To By Index
//            copyFileByIndex(index, dirSelector->getSelectedDir());

//        }
//    } else {
//        qDebug() << "MainWindow::doCopyToDirectory - Target Dir is the same as current...";
//    }
}

//==============================================================================
// Move Current/Selected File(s)
//==============================================================================
void MainWindow::doMoveToDirectory()
{
//    // Check Dir Selector & Selected Dir
//    if (dirSelector && dirSelector->getSelectedDir() != currentDir) {
//        qDebug() << "MainWindow::doMoveToDirectory - selectedDir: " << dirSelector->getSelectedDir();

//        // Reset Transfer Options
//        transferOptions = 0;

//        // Check If Browser Grid Has Selections
//        if (hasSelection()) {

//            // Get Browser Data Model Count
//            int bdmCount = browserDataModel.count();
//            // Get Selected Item Count
//            int selectedCount = numberOfSelected();
//            // Init Selected Index
//            int selectedIndex = 0;
//            // Init Last Index
//            int lastIndex = -1;

//            // Got Thru Model
//            for (int i=bdmCount-1; i>=0; i--) {
//                // Get Item
//                BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);

//                // Check If Selected
//                if (item->fileSelected) {
//                    // Set Last Index
//                    lastIndex = i;
//                    // Move By Index
//                    moveFileByIndex(i, dirSelector->getSelectedDir(), false, false);
//                    //moveFileByIndex(i, dirSelector->getSelectedDir());
//                    // Inc Selected Index
//                    selectedIndex++;
//                    // Emit Progress Changed Signal
//                    emit progressChanged(selectedCount, selectedIndex);
//                }
//            }

//            // Check Worker
//            if (worker) {
//                // Emit Populat Browser Model
//                emit worker->populateBrowserModel(lastIndex);
//            } else {
//                // Populate Browser Data Model
//                populateBrowserModel(lastIndex);
//            }
///*
//            // Check Last Index
//            if (lastIndex != -1) {
//                // Set Current Index
//                setCurrentIndex(lastIndex);
//            }
//*/
//            // Get Deleted Items Count
//            while (deletedItems.count() > 0) {
//                // Get Last Item
//                QObject* lastItem = deletedItems.takeLast();
//                // Delete Last Item
//                lastItem->deleteLater();
//            }

//        } else {
//            // Init index
//            int index = currentIndex ;

//            // Check Popup Index
//            if (popupIndex != -1) {
//                qDebug() << "MainWindow::doMoveToDirectory - popupIndex: " << popupIndex;

//                // Set Index
//                index = popupIndex;
//                // Reset Popup Index
//                popupIndex = -1;

//            } else {
//                qDebug() << "MainWindow::doMoveToDirectory - currentIndex: " << currentIndex;
//            }

//            // Move By Index
//            moveFileByIndex(index, dirSelector->getSelectedDir());

//        }
//    } else {
//        qDebug() << "MainWindow::doMoveToDirectory - Target Dir is the same as current...";
//    }
}

//==============================================================================
// Compare Images
//==============================================================================
void MainWindow::compareImages()
{
//    qDebug() << "MainWindow::compareImages";

//    // Check Comapre Images Dialog
//    if (!compareDialog) {
//        // Create Dialog
//        compareDialog = new CompareDialog();
//    }

//    // Init Left Image
//    QString leftImage = QString("");
//    // Init Right Image
//    QString rightImage = QString("");
//    // Init Title Left
//    QString titleLeft = QString("");
//    // Init title Right
//    QString titleRight = QString("");
//    // Init Item
//    BrowserDataObject* item = NULL;

//    // Check If Has Selection
//    if (hasSelection()) {
//        // Get Browser Data Model Count
//        int bdmCount = browserDataModel.count();

//        // Go Thru Items
//        for (int i=0; i<bdmCount; i++) {
//            // Get Item
//            item = static_cast<BrowserDataObject*>(browserDataModel[i]);

//            // Check If Item Selected
//            if (item->fileSelected) {
//                // Check Left Image
//                if (leftImage.isEmpty()) {
//                    // Set Left Image
//                    leftImage = currentDir + QString("/") + item->fileName;
//                    // Set Left Title
//                    titleLeft = item->fileName;
//                } else {
//                    // Check Right Image
//                    if (rightImage.isEmpty()) {
//                        // Set Right Image
//                        rightImage = currentDir + QString("/") + item->fileName;
//                        // Set Right Title
//                        titleRight = item->fileName;
//                    } else {
//                        break;
//                    }
//                }
//            }
//        }
//    } else {
//        // Get Item
//        item = static_cast<BrowserDataObject*>(browserDataModel[currentIndex]);
//        // Get Current File Name
//        leftImage = currentDir + QString("/") + item->fileName;
//        // Set Left Title
//        titleLeft = item->fileName;
//        // Check Popup Index
//        if (popupIndex != -1) {
//            // Get Item
//            item = static_cast<BrowserDataObject*>(browserDataModel[popupIndex]);
//            // Set Right Title
//            titleRight = item->fileName;
//            // Set Right Image
//            rightImage = currentDir + QString("/") + item->fileName;
//            // Reset Popup Index
//            popupIndex = -1;
//        }
//    }


    // ...
}

//==============================================================================
// Zoom In
//==============================================================================
void MainWindow::zoomIn()
{
    // Check Center View
    if (!ui->centerView) {
        return;
    }

    // Check Composite Sizes
    if (ui->centerView->getSourceCompositeWidth() <= 0.0 || ui->centerView->getSourceCompositeHeight() <= 0.0) {
        return;
    }

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
            //qDebug() << "MainWindow::zoomIn";

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
void MainWindow::zoomOut()
{
    // Check Center View
    if (!ui->centerView) {
        return;
    }

    // Check Composite Sizes
    if (ui->centerView->getSourceCompositeWidth() <= 0.0 || ui->centerView->getSourceCompositeHeight() <= 0.0) {
        return;
    }

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
void MainWindow::zoomDefault()
{
    //qDebug() << "MainWindow::zoomDefault";
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
void MainWindow::zoomToFit()
{
    // Check Center View
    if (!ui->centerView) {
        return;
    }

    // Check Composite Sizes
    if (ui->centerView->getSourceCompositeWidth() <= 0.0 || ui->centerView->getSourceCompositeHeight() <= 0.0) {
        return;
    }

    // Check Zoom Fit
    if (!zoomFit) {
        //qDebug() << "MainWindow::zoomToFit";

        // Set Zoom Fit
        zoomFit = true;

        // Calculate Width Ratio
        qreal widthRatio = ui->centerView->getSourceCompositeWidth() / ui->centerView->width();
        // Calculate Height Ratio
        qreal heightRatio = ui->centerView->getSourceCompositeHeight() / ui->centerView->height();

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

    // Update Menu
    updateMenu();
}

//==============================================================================
// Reset Zoom & Pan Position
//==============================================================================
void MainWindow::reset()
{
    qDebug() << "MainWindow::reset";

    // Reset Pan Position
    setPanPosX(0.0);
    setPanPosY(0.0);

    // Zoom Default
    zoomDefault();

    // Update Menu
    updateMenu();
}

//==============================================================================
// Delete File By Index
//==============================================================================
void MainWindow::deleteFileByIndex(const int& aIndex, const bool& aDeleteItem, const bool& aNotify)
{
//    // Get Model Count
//    int bdmCount = browserDataModel.count();

//    // Check Index
//    if (aIndex >=0 && aIndex < bdmCount) {
//        // Init File Name
//        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

//        // Init File Info
//        QFileInfo fileInfo(fileName);

//        // Check File Info - Check Settings
//        if (fileInfo.exists()) {

//            qDebug() << "MainWindow::deleteFileByIndex - aIndex: " << aIndex;

//            // Init Dir
//            QDir dir(fileInfo.canonicalPath());

//            // Delete File
//            if (!dir.remove(fileName)) {
//                qDebug() << "MainWindow::deleteFileByIndex - aIndex: " << fileName << " - ERROR!";
//            }

//            // Get Item
//            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel.takeAt(aIndex));

//            // Check Notify
//            if (aNotify) {
//                // Emit Deleting Index Signal
//                emit deletingIndex(aIndex);

//                // Check Worker
//                if (worker) {
//                    // Emit Populate Browser Data Model
//                    emit worker->populateBrowserModel(aIndex);
//                } else {
//                    // Populate Browser Data Model
//                    populateBrowserModel(aIndex);
//                }

//                // Emit Index Removed Signal
//                emit indexDeleted(aIndex);

//                // Set Current Index
//                //setCurrentIndex(aIndex);
//            }

//            // Check Delete Item
//            if (aDeleteItem) {
//                // Delte Item - Have to do it last
//                //delete item;
//                item->deleteLater();
//            } else {
//                // Append To Deleted Item List
//                deletedItems << item;
//            }
//        }
//    }
}

//==============================================================================
// Rotate File By Index
//==============================================================================
void MainWindow::rotateFileByIndex(const int& aIndex, int aDirection, const bool& aNotify)
{
//    // Get Model Count
//    int bdmCount = browserDataModel.count();

//    // Check Index
//    if (aIndex >=0 && aIndex < bdmCount) {
//        // Init File Name
//        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

//        // Init Image
//        QImage image(fileName);

//        // Init Transform
//        QTransform transform;
//        // Set Rotation
//        transform.rotate(aDirection == RDTLeft ? TRANSFORM_ROTATE_LEFT : TRANSFORM_ROTATE_RIGHT);

//        // Get Transformed Image
//        image = image.transformed(transform);

//        // Save Image
//        image.save(fileName);

//        // For Refresh

//        // Check Notify
//        if (aNotify) {
//            // Emit File Updated Signal
//            emit fileUpdated(aIndex);
//        }
//    }
}

//==============================================================================
// Flip File By Index
//==============================================================================
void MainWindow::flipFileByIndex(const int& aIndex, int aDirection, const bool& aNotify)
{
//    // Get Model Count
//    int bdmCount = browserDataModel.count();

//    // Check Index
//    if (aIndex >=0 && aIndex < bdmCount) {
//        // Init File Name
//        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

//        // Init Image
//        QImage image(fileName);
//        // Get Mirrored Image
//        image = image.mirrored(aDirection == FDTHorizontal, aDirection == FDTVertical);

//        // Save Image
//        image.save(fileName);

//        // For Refresh

//        // Check Notify
//        if (aNotify) {
//            // Emit File Updated Signal
//            emit fileUpdated(aIndex);
//        }
//    }
}

//==============================================================================
// Copy File By Index
//==============================================================================
bool MainWindow::copyFileByIndex(const int& aIndex, const QString& aTargetDir, const bool& aNotify)
{
//    Q_UNUSED(aNotify);

//    // Get Model Count
//    int bdmCount = browserDataModel.count();

//    // Check Index
//    if (aIndex >=0 && aIndex < bdmCount) {
//        // Init Source File Name
//        QString sourceFileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

//        // Init Target File Name
//        QString targetFileName(aTargetDir);
//        // Add Separator
//        targetFileName += "/";
//        // Add Current File Name
//        targetFileName += static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

//        // Init Current File
//        QFile file(sourceFileName);
//        // Init Target File
//        QFile targetFile(targetFileName);

//        // Check Target File - TODO: Check Settings

//        // Check If Target File Exists
//        if (targetFile.exists()) {
//            //qDebug("MainWindow::copyFileByIndex - Overwriting...");

//            // Check Transfer Options - No To All
//            if (transferOptions & DEFAULT_TRANSFER_OPTIONS_NO_TO_ALL) {
//                return false;
//            }

//            // Check Transfer Options - Yes To All
//            if (!(transferOptions & DEFAULT_TRANSFER_OPTIONS_YES_TO_ALL)) {

//                // Check Compare Dialog
//                if (!compareDialog) {

//                    // Create Compare Dialog
//                    //compareDialog = new CompareDialog();

//                    qWarning() << "MainWindow::copyFileByIndex - NO COMPARE DIALOG!!";

//                    return false;
//                }

//                // Set Current Dir
//                compareDialog->setCurrentDir(currentDir);

//                // Set Left Image File
//                compareDialog->setLeftImage(sourceFileName);
//                // Set Right Image File
//                compareDialog->setRightImage(targetFileName);

//                // Set Window Title
//                //compareDialog->setWindowTitle(QString("Compare Images: %1 <-> %2").arg(titleLeft).arg(titleRight));
//                compareDialog->setWindowTitle(QString("Target File Exists: %1. Overwrite?").arg(targetFileName));

//                // ...

//                // Configure Buttons
////                compareDialog->configureButtons(QDialogButtonBox::Yes | QDialogButtonBox::YesToAll | QDialogButtonBox::No | QDialogButtonBox::NoToAll | QDialogButtonBox::Abort);

//                // Exec
//                compareDialog->exec();

//                // Check Action Index
//                switch (compareDialog->actionIndex) {
//                    case 1:
//                        // Yes To All

//                        // Set Transfer Options
//                        transferOptions |= DEFAULT_TRANSFER_OPTIONS_YES_TO_ALL;

//                    case 0:
//                        // Yes

//                    break;

//                    case 3:
//                        // No To All

//                        // Set Transfer Options
//                        transferOptions |= DEFAULT_TRANSFER_OPTIONS_NO_TO_ALL;

//                    case 2:
//                    default:

//                        // No
//                        return false;
//                    break;
//                }
//            }

//            // Delete Target File
//            if (!targetFile.remove()) {
//                qDebug("MainWindow::copyFileByIndex - ERROR CLEARING TARGET FILE!");

//                return false;
//            }
//        }

//        // Copy File(s)
//        if (!file.copy(targetFileName)) {
//            qDebug("MainWindow::copyFileByIndex - ERROR COPY FILE!");

//            return false;
//        }

//        return true;
//    }

    return false;
}

//==============================================================================
// Move File By Index
//==============================================================================
void MainWindow::moveFileByIndex(const int& aIndex, const QString& aTargetDir, const bool& aDeleteItem, const bool& aNotify)
{
//    Q_UNUSED(aNotify);

//    // Copy File by Index
//    if (copyFileByIndex(aIndex, aTargetDir, aNotify)) {
//        // Init File Name
//        QString sourceFileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;
//        // Init Source File
//        QFile sourceFile(sourceFileName);
//        // Remove
//        if (!sourceFile.remove()) {
//            qDebug("MainWindow::moveFileByIndex - ERROR REMOVING SOURCE FILE!");

//            return;
//        }
//    } else {
//        //qDebug("MainWindow::moveFileByIndex - ERROR COPY SOURCE FILE!");

//        return;
//    }

//    // Get Item
//    BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel.takeAt(aIndex));

//    // Check Notify
//    if (aNotify) {
//        // Emit Deleting Index Signal
//        emit deletingIndex(aIndex);

//        // Check Worker
//        if (worker) {
//            // Emit Populate Browser Data Model
//            emit worker->populateBrowserModel(aIndex);
//        } else {
//            // Populate Browser Data Model
//            populateBrowserModel(aIndex);
//        }

//        // Emit Index Removed Signal
//        emit indexDeleted(aIndex);

//        // Set Current Index
//        //setCurrentIndex(aIndex);
//    }

//    if (aDeleteItem) {
//        // Delte Item - Have to do it last
//        //delete item;
//        item->deleteLater();
//    } else {
//        // Append To Deleted Items
//        deletedItems << item;

//        qDebug() << "MainWindow::moveFileByIndex - added deleted item";
//    }
}

//==============================================================================
// Rename File By Index
//==============================================================================
void MainWindow::renameFileByIndex(const int& aIndex, const QString& aFileName, const bool& aNotify)
{
//    // Get Model Count
//    int bdmCount = browserDataModel.count();

//    // Check Index
//    if (aIndex >=0 && aIndex < bdmCount) {
//        // Get Item
//        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[aIndex]);

//        // Init Full File Name
//        QString fileName = currentDir + QString("/") + item->fileName;

//        // Init Target File Name
//        QString targetFileName(currentDir);
//        // Add Separator
//        targetFileName += "/";
//        // Add Target File Name
//        targetFileName += aFileName;

//        // Init Current File
//        QFile file(fileName);
//        // Init Target File
//        QFile targetFile(targetFileName);

//        // Check Target File - TODO: Check Settings
//        if (targetFile.exists()) {
//            qDebug("MainWindow::renameFileByIndex - Overwriting...");

//            // Show Image Compare Dialog - Check Settings

//            // Delete Target File
//            if (!targetFile.remove()) {
//                qDebug("MainWindow::renameFileByIndex - ERROR CLEARING TARGET FILE!");

//                return;
//            }
//        }

//        // Rename File
//        if (file.rename(targetFileName)) {
//            // Set Browser Data Model File Name
//            item->setFileName(aFileName);

//            // Check Index
//            if (aIndex == currentIndex) {
//                // Update Current File
//                setCurrentFile(targetFileName);
//            }

//        } else {
//            qDebug("MainWindow::renameFileByIndex - ERROR RENAME FILE!");
//        }

//        // Check Notify
//        if (aNotify) {
//            // Emit File Updated Signal
//            emit fileUpdated(aIndex);

//            // Check Worker
//            if (worker) {
//                // Emit File Renamed Signal
//                emit worker->fileRenamed(aFileName);
//            } else {
//                // Call File Renamed Slot
//                fileRenamed(aFileName);
//            }
//        }
//    }
}

//==============================================================================
// Evaluate File Rename Pattern
//==============================================================================
bool MainWindow::evaluateFileRenamePattern()
{
//    // Check REname Series Dialog
//    if (!renameSeriesDialog) {
//        return false;
//    }

//    // Get Pattern
//    QString pattern = renameSeriesDialog->getPattern();

//    // Check Pattern
//    if (pattern.isEmpty()) {

//        // Check Info Dialog
//        if (!infoDialog) {
//            // Create Info Dialog
//            infoDialog = new InfoDialog;
//        }

//        // Show Invalid Pattern Dialog
//        infoDialog->showDialogWithText(QString("Invalid File Name Pattern!"));

//        return false;
//    }

//    // Get Index Of #
//    int hashIndex = pattern.indexOf("#");

//    // Check Hash Index
//    if (hashIndex < 0) {
//        // Check Info Dialog
//        if (!infoDialog) {
//            // Create Info Dialog
//            infoDialog = new InfoDialog;
//        }

//        // Show Invalid Pattern Dialog
//        infoDialog->showDialogWithText(QString("Invalid File Name Pattern!"));

//        return false;
//    }

//    // Set Prev Hash Index
//    int prevHashIndex = hashIndex;

//    // Go Thru Pattern Checking Hash Indexes
//    while (hashIndex >= 0) {
//        // Get Next Hash Index
//        hashIndex = pattern.indexOf("#", prevHashIndex + 1);

//        // Check Hash Index
//        if (hashIndex - prevHashIndex > 1) {
//            // Check Info Dialog
//            if (!infoDialog) {
//                // Create Info Dialog
//                infoDialog = new InfoDialog;
//            }

//            // Show Invalid Pattern Dialog
//            infoDialog->showDialogWithText(QString("Invalid File Name Pattern!"));

//            return false;
//        }

//        // Set Prev Hash Index
//        prevHashIndex = hashIndex;
//    }

    // ...

    return true;
}

//==============================================================================
// Launch Viewer
//==============================================================================
void MainWindow::launchViewer(const QString& aFilePath)
{
    // Check Viewer Window
    if (!viewerWindow) {
        // Get Instance
        viewerWindow = ViewerWindow::getInstance();
    }

    // Init File Info
    QFileInfo fileInfo(aFilePath);

    // Check File Path
    if (!aFilePath.isEmpty() && fileInfo.exists()) {
        qDebug() << "MainWindow::launchViewer - aFilePath: " << aFilePath;

        // Set Current File
        viewerWindow->setCurrentFile(aFilePath);
        // Show Viewer Window Full Screen
        viewerWindow->showWindow();
    }
}

//==============================================================================
// Hide Viewer
//==============================================================================
void MainWindow::hideViewer()
{
    // Check Viewer Window
    if (viewerWindow) {
        qDebug() << "MainWindow::hideViewer";

        // Hide Viewer Window
        viewerWindow->hideWindow();
    }

    // Show Current Borwser Window
    show();

    // ...
}

//==============================================================================
// Toggle Viewer Fullscreen
//==============================================================================
void MainWindow::toggleViewerFullScreen()
{
    // Check Viewer Window
    if (viewerWindow) {
        // Check Full Screen Status
        if (viewerWindow->windowState() == Qt::WindowFullScreen) {

            // Show
            viewerWindow->showNormal();
            // Save Settings
            viewerWindow->saveSettings();
            // Show Browser Window
            show();
            // Raise Window
            viewerWindow->raise();
            // Activate Window To Regain Focus
            viewerWindow->activateWindow();

        } else {
            // Show
            viewerWindow->showFullScreen();
            // Save Settings
            viewerWindow->saveSettings();
            // Hide
            hide();
        }
    }
}

//==============================================================================
// Launch Preferences
//==============================================================================
void MainWindow::launchPreferences()
{
    qDebug() << "MainWindow::launchPreferences";

    // Init Preferences Dialog
    PreferencesDialog pDialog;

    // Launch Dialog
    pDialog.launchDialog();

    // ...
}

//==============================================================================
// Launch About Dialog
//==============================================================================
void MainWindow::launchAboutDialog()
{
    qDebug() << "MainWindow::launchPreferences";

    // Check About Dialog
    if (!aboutDialog) {
        // Create About Dialog
        aboutDialog = new AboutForm();
    }

    // Launch Dialog
    aboutDialog->show();

    // ...
}

//==============================================================================
// Launch File Open Left
//==============================================================================
void MainWindow::launchFileOpen(const QString& aSide)
{
    qDebug() << "MainWindow::launchFileOpen - aSide: " << aSide;

    // Init Filter Text
    QString filterText = QString("Image files (%1)").arg(DEFAULT_SUPPORTED_FORMATS_FILTER);

    // Init File Dialog
    QFileDialog fileDialog(this, tr("Open Image"), lastOpenPath, filterText);

    // Set File Mode
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    // Exec File Dialog
    if (fileDialog.exec()) {
        // Set Last Open Path
        lastOpenPath = fileDialog.directory().absolutePath();

        // Check Side
        if (aSide == CONTEXT_PROPERTY_SIDE_VALUE_LEFT) {
            // Set Current File Left
            setCurrentFileLeft(fileDialog.selectedFiles()[0]);
        } else {
            // Set Current File Right
            setCurrentFileRight(fileDialog.selectedFiles()[0]);
        }
    }
}

//==============================================================================
// Show Window
//==============================================================================
void MainWindow::showWindow()
{
    // Restore UI
    restoreUI();

    qDebug() << "MainWindow::showWindow";

    // Init Settings
    QSettings settings;

    // Get MaximizedState
    bool maximizedState = settings.value(SETTINGS_KEY_VIEW_MAXIMIZED).toBool();

    // Check MAximized State
    if (maximizedState) {
        // Show Maximized
        showMaximized();
    } else {
        // Show
        show();
    }

    // ...
}

//==============================================================================
// Hide Window
//==============================================================================
void MainWindow::hideWindow()
{
    qDebug() << "MainWindow::hideWindow";

    // Save Settings
    saveSettings();

    // Hide
    hide();
}

//==============================================================================
// Show Status Text
//==============================================================================
void MainWindow::showStatusText(const QString& aStatusText, const int& aTimeout)
{
    // Show Message
    ui->statusbar->showMessage(aStatusText, aTimeout);
}

//==============================================================================
// Handle File Open Event Slot
//==============================================================================
void MainWindow::handleFileOpenEvent(const QString& aFileName)
{
    qDebug() << "MainWindow::handleFileOpenEvent - aFileName: " << aFileName;

    // Show Status Text
    //showStatusText(aFileName);

    // Launch Viewer
    //launchViewer(aFileName);

    // ...
}

//==============================================================================
// Stop Worker
//==============================================================================
void MainWindow::stopWorkerThread()
{
    if (workerThread.isRunning()) {
        qDebug() << "MainWindow::stopWorkerThread";
        // Terminate
        workerThread.terminate();
        // Wait
        //workerThread.wait();
    }
}

//==============================================================================
// Worker Result Ready Slot
//==============================================================================
void MainWindow::workerResultReady(const int& aOperation, const int& aResult)
{
    qDebug() << "MainWindow::workerResultReady - aOperation: " << aOperation << " - aResult: " << aResult;

    // Quit
    workerThread.quit();

    // Switch Operation
    switch (aOperation) {

        default:

        break;
    }

    // ...
}

//==============================================================================
// File Renamed Slot
//==============================================================================
void MainWindow::fileRenamed(const QString& aFileName)
{
    qDebug() << "MainWindow::fileRenamed - aFileName: " << aFileName;

    // ...
}
/*
//==============================================================================
// Left Image Mouse Pressed Slot
//==============================================================================
void MainWindow::imageLeftMousePressed(const QPoint& aPos)
{

}

//==============================================================================
// Left Image Mouse Moved Slot
//==============================================================================
void MainWindow::imageLeftMouseMoved(const QPoint& aPos)
{

}

//==============================================================================
// Left Image Mouse Released Slot
//==============================================================================
void MainWindow::imageLeftMouseReleased(const QPoint& aPos)
{

}

//==============================================================================
// Compositor Mouse Pressed Slot
//==============================================================================
void MainWindow::compositorMousePressed(const QPoint& aPos)
{

}

//==============================================================================
// Compositor Mouse Moved Slot
//==============================================================================
void MainWindow::compositorMouseMoved(const QPoint& aPos)
{

}

//==============================================================================
// Compositor Mouse Released Slot
//==============================================================================
void MainWindow::compositorMouseReleased(const QPoint& aPos)
{

}

//==============================================================================
// Right Image Mouse Pressed Slot
//==============================================================================
void MainWindow::imageRightMousePressed(const QPoint& aPos)
{

}

//==============================================================================
// Right Image Mouse Moved Slot
//==============================================================================
void MainWindow::imageRightMouseMoved(const QPoint& aPos)
{

}

//==============================================================================
// Right Image Mouse Released Slot
//==============================================================================
void MainWindow::imageRightMouseReleased(const QPoint& aPos)
{

}
*/
//==============================================================================
// Pan Start Slot
//==============================================================================
void MainWindow::panStart(const QPoint& aPos)
{
    //qDebug() << "MainWindow::panStart - aPos: " << aPos;

    // Set Pan Press X
    panPressX = aPos.x();
    // Set Pan Press Y
    panPressY = aPos.y();

    // Set Last Pan Pos X
    panPosLastX = panPosX;
    // Set Last Pan Pos Y
    panPosLastY = panPosY;

    // ...
}

//==============================================================================
// Pan Move Slot
//==============================================================================
void MainWindow::panMove(const QPoint& aPos)
{
    //qDebug() << "MainWindow::panMove - aPos: " << aPos;

    // Set Pan Pos X
    setPanPosX(aPos.x() - panPressX + panPosLastX);
    // Set Pan Pos Y
    setPanPosY(aPos.y() - panPressY + panPosLastY);

    // ...
}

//==============================================================================
// Pan Finished Slot
//==============================================================================
void MainWindow::panFinished(const QPoint& aPos)
{
    //qDebug() << "MainWindow::panFinished - aPos: " << aPos;

    // Reset Pan Press X
    panPressX = 0;
    // Reset Pan Press Y
    panPressY = 0;

    // Set Last Pan Pos X
    panPosLastX = panPosX;
    // Set Last Pan Pos Y
    panPosLastY = panPosY;

    // ...
}

//==============================================================================
// Compositor Double Clicked Slot
//==============================================================================
void MainWindow::compositorDoubleClicked()
{
    qDebug() << "MainWindow::compositorDoubleClicked";

    // ...
}

//==============================================================================
// Side Viev Double Clicked Slot
//==============================================================================
void MainWindow::sideViewDoubleClicked(const QString& aSide)
{
    launchViewer(aSide == CONTEXT_PROPERTY_SIDE_VALUE_LEFT ? currentFileLeft : currentFileRight);
}

//==============================================================================
// Open Left File Button Clicked Slot
//==============================================================================
void MainWindow::on_openLeftButton_clicked()
{
    launchFileOpen(CONTEXT_PROPERTY_SIDE_VALUE_LEFT);
}

//==============================================================================
// Open Right File Button Clicked Slot
//==============================================================================
void MainWindow::on_openRightButton_clicked()
{
    launchFileOpen(CONTEXT_PROPERTY_SIDE_VALUE_RIGHT);
}

//==============================================================================
// Launch Preferences
//==============================================================================
void MainWindow::on_actionPreferences_triggered()
{
    // Launch Preferences
    launchPreferences();
}

//==============================================================================
// Preferences Button Clicked Slot
//==============================================================================
void MainWindow::on_optionsButton_clicked()
{
    launchPreferences();
}

//==============================================================================
// Action About Triggered
//==============================================================================
void MainWindow::on_actionAbout_triggered()
{
    // Launch About Dialog
    launchAboutDialog();
}

//==============================================================================
// Action Rotate Left Triggered
//==============================================================================
void MainWindow::on_actionRotate_Left_triggered()
{
    // Rotate Let
    rotateLeft();
}

//==============================================================================
// Action Rotate Right Triggered
//==============================================================================
void MainWindow::on_actionRotate_Right_triggered()
{
    // Rotate Right
    rotateRigth();
}

//==============================================================================
// Action Flip Horizontally Triggered
//==============================================================================
void MainWindow::on_actionFlip_Horizontally_triggered()
{
    // Flip Horizontally
    flipHorizontally();
}

//==============================================================================
// Action Flip Vertically Triggered
//==============================================================================
void MainWindow::on_actionFlip_Vertically_triggered()
{
    // Flip Vertically
    flipVertically();
}

//==============================================================================
// Action Delete Triggered
//==============================================================================
void MainWindow::on_actionDelete_triggered()
{
    // Delete File
    deleteFile();
}

//==============================================================================
// Action Rename Triggered
//==============================================================================
void MainWindow::on_actionRename_triggered()
{
    // Rename Current/Selected File(s)
    renameFile();
}

//==============================================================================
// Reset Zoom & Panning Pos Button Clicked Slot
//==============================================================================
void MainWindow::on_resetButton_clicked()
{
    reset();
}

//==============================================================================
// Zoom Out Button Clicked Slot
//==============================================================================
void MainWindow::on_zoomOutButton_clicked()
{
    zoomOut();
}

//==============================================================================
// Zoom In Button Clicked Slot
//==============================================================================
void MainWindow::on_zoomInButton_clicked()
{
    zoomIn();
}

//==============================================================================
// Zoom To Fit Button Clicked Slot
//==============================================================================
void MainWindow::on_zoomToFitButton_clicked()
{
    zoomToFit();
}

//==============================================================================
// Exit Button Clicked Slot
//==============================================================================
void MainWindow::on_exitButton_clicked()
{
    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Action Quit Triggered
//==============================================================================
void MainWindow::on_actionQuit_triggered()
{
    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Key Press Event
//==============================================================================
void MainWindow::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Set Key Modifiers
        keyModifiers = aEvent->modifiers();
    }

    QMainWindow::keyPressEvent(aEvent);
}

//==============================================================================
// Key Release Event
//==============================================================================
void MainWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        switch (aEvent->key()) {
            case Qt::Key_Equal:
            case Qt::Key_Plus:
                // Zoom In
                zoomIn();
            break;

            case Qt::Key_Minus:
                // Zoom Out
                zoomOut();
            break;

            case Qt::Key_Space:
                // Reset Zoom Level & Pan Position
                reset();
            break;

            case Qt::Key_Escape:
                if (aEvent->modifiers() & Qt::ShiftModifier) {
                    // Minimize Window
                    showMinimized();
                }
            break;

            case Qt::Key_F10:
                // Exit AppLication
                QApplication::exit();
            break;

            default:
            break;
        }

        // Set Key Modifiers
        keyModifiers = aEvent->modifiers();
    }

    QMainWindow::keyReleaseEvent(aEvent);
}

//==============================================================================
// Timer Event
//==============================================================================
void MainWindow::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {

        // ...

    }
}

//==============================================================================
// Mouse Wheel Event
//==============================================================================
void MainWindow::wheelEvent(QWheelEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        //qDebug() << "MainWindow::wheelEvent - angleDelta: " << aEvent->angleDelta();
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
// Destructor
//==============================================================================
MainWindow::~MainWindow()
{
    // Terminate Worker Thread
    stopWorkerThread();

    // Save Settings
    saveSettings();

    // Delete Left View To Avoid Crash
    delete ui->leftView;

    // Delete Center View To Avoid Crash
    delete ui->centerView;

    // Delete Right View To Avoid Crash
    delete ui->rightView;

    // Delete UI
    delete ui;

    // Reset File System Model
    fsModel = NULL;

    // Check About Dialog
    if (aboutDialog) {
        // Delete Dialog
        delete aboutDialog;
        aboutDialog = NULL;
    }

    // Check Dir Selector
    if (dirSelector) {
        // Delete Dialog
        delete dirSelector;
        dirSelector = NULL;
    }

    // Check Info Dialog
    if (infoDialog) {
        // Delete Dialog
        delete infoDialog;
        infoDialog = NULL;
    }

    qDebug() << "MainWindow::~MainWindow";
}



