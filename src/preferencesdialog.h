#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class PreferencesDialog;
}

class MainWindow;

//==============================================================================
// Preferences Dialog
//==============================================================================
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit PreferencesDialog(QWidget* aParent = NULL);

    // Launch Dialog
    void launchDialog();

    // Destructor
    virtual ~PreferencesDialog();

private slots:

    // Dialog Accepted
    void on_buttonBox_accepted();
    // Button Clicked
    void on_buttonBox_clicked(QAbstractButton* aButton);
    // Dialog Rejected
    void on_buttonBox_rejected();
    // On Show Grid Check Box Clicked Slot
    void on_showGridCheckBox_clicked();
    // On threshold Changed Slot
    void on_thresholdSlider_actionTriggered(int action);
    // On Right Opacity Changed Slot
    void on_rightOpacitySlider_actionTriggered(int action);
    // On Left Opacity Changed Slot
    void on_leftOpacitySlider_actionTriggered(int action);
    // On Hide Sources Check Box Clicked Slot
    void on_hideSourcesCheckBox_clicked();

private:

    // Load Settings
    void loadSettings();
    // Apply Settings
    void applySettins();
    // Save Settings
    void saveSettings();

private:

    // UI
    Ui::PreferencesDialog*  ui;
    // Main Browser Window
    MainWindow*             mainWindow;
};

#endif // PREFERENCESDIALOG_H
