#include <QDebug>

#include "settings.h"
#include "constants.h"
#include "defaultsettings.h"


// Settings Singleton
static SettingsController* settingsSingleton = NULL;


//==============================================================================
// Get Instance - Static Constructor
//==============================================================================
SettingsController* SettingsController::getInstance()
{
    // Check Settings  Singleton
    if (!settingsSingleton) {
        // Create Singleton
        settingsSingleton = new SettingsController();
        // Load Settings
        settingsSingleton->loadSettings();
    } else {
        // Inc Ref Count
        settingsSingleton->refCount++;
    }

    return settingsSingleton;
}

//==============================================================================
// Constructor
//==============================================================================
SettingsController::SettingsController(QObject* aParent)
    : QObject(aParent)
    , refCount(1)
    , dirty(false)
{
    qDebug() << "SettingsController::SettingsController";

    // ...

}

//==============================================================================
// Release
//==============================================================================
void SettingsController::release()
{
    // Dec Ref Count
    refCount--;

    // Check Ref Count
    if (refCount <= 0 && settingsSingleton) {
        // Delete Settings Singleton
        delete settingsSingleton;
        settingsSingleton = NULL;
    }
}

//==============================================================================
// Get Value
//==============================================================================
QVariant SettingsController::value(const QString& aKey, const QVariant& aDefaultValue)
{
    return settings.value(aKey, aDefaultValue);
}

//==============================================================================
// Set Value
//==============================================================================
void SettingsController::setValue(const QString& aKey, const QVariant& aValue)
{
    // Set Value
    settings.setValue(aKey, aValue);

    // Set Dirty
    //setDirty(true);

    // Emit Setting Has Changed Signal
    emit settingHasChanged(aKey);
}

//==============================================================================
// Begin Global Settings Update
//==============================================================================
void SettingsController::beginGlobalSettingsUpdate()
{
    // Emit Begin Global Settings Update
    emit globalSettingsUpdateBegin();

    // ...
}

//==============================================================================
// Finish Global Settings Update
//==============================================================================
void SettingsController::finishGlobalSettingsUpdate()
{
    // ...

    // Emit Global Settings Update Finished
    emit globalSettingsUpdateFinished();
}

//==============================================================================
// Get Dirty
//==============================================================================
bool SettingsController::getDirty()
{
    return dirty;
}

//==============================================================================
// Load Settings
//==============================================================================
void SettingsController::loadSettings()
{
    qDebug() << "SettingsController::loadSettings";


    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void SettingsController::saveSettings()
{
    // Check If Not Dirty
    if (!dirty) {
        return;
    }

    qDebug() << "SettingsController::saveSettings";


    // ...

    // Sync
    settings.sync();
}

//==============================================================================
// Restore Defaults
//==============================================================================
void SettingsController::restoreDefaults()
{
    // Emit Begin Global Settings Update
    emit globalSettingsUpdateBegin();

    // ...

    // Reset Dirty
    setDirty(false);

    // Emit Global Settings Update Finished
    emit globalSettingsUpdateFinished();
}

//==============================================================================
// Set Dirty
//==============================================================================
void SettingsController::setDirty(const bool& aDirty)
{
    // Check Dirty
    if (dirty != aDirty) {
        // Set Dirty
        dirty = aDirty;

        // ...

        // Emit Dirty Changed Signal
        emit dirtyChanged(dirty);
    }
}

//==============================================================================
// Destructor
//==============================================================================
SettingsController::~SettingsController()
{
    // Save Settings
    saveSettings();

    qDebug() << "SettingsController::~SettingsController";
}

