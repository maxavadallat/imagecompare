
#include <QDebug>


#include "imagecompareapp.h"
#include "mainwindow.h"
//#include "viewerwindow.h"
#include "constants.h"


//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
    qDebug() << " ";
    qDebug() << "================================================================================";
    qDebug() << " Starting Max Viewer...";
    qDebug() << "================================================================================";
    qDebug() << " ";

    // Init Application
    ImageCompareApp app(argc, argv);

    // Set Application Name
    app.setApplicationName(DEFAULT_APPLICATION_NAME);
    // Set Organization Name
    app.setOrganizationName(DEFAULT_ORGANIZATION_NAME);
    // Set Organization Domain
    app.setOrganizationDomain(DEFAULT_ORGANIZATION_DOMAIN);

    // Init Browser Window
    MainWindow* mainWindow = MainWindow::getInstance();

    // Connect Signal
    QObject::connect(&app, SIGNAL(fileOpenEventOccured(QString)), mainWindow, SLOT(handleFileOpenEvent(QString)));

    // Set Viewer Window Visibility
    //viewerWindow->setVisible(false);

    // Show Browser Window
    mainWindow->showWindow();

    // Execute App
    int result = app.exec();

    // Release Viewer Window
    //viewerWindow->release();

    // Release Browser Window Instance
    mainWindow->release();

    qDebug() << " ";
    qDebug() << "================================================================================";
    qDebug() << " Exiting Max Viewer...";
    qDebug() << "================================================================================";
    qDebug() << " ";

    return result;
}


