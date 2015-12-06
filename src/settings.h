#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QVariant>

//==============================================================================
// Settings Settings Controller
//==============================================================================
class SettingsController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool dirty READ getDirty NOTIFY dirtyChanged)

public:

    // Get Instance - Static Constructor
    static SettingsController* getInstance();

    // Release
    void release();

    // Get Value
    QVariant value(const QString& aKey, const QVariant& aDefaultValue = QVariant());
    // Set Value
    void setValue(const QString& aKey, const QVariant& aValue);

    // Begin Global Settings Update
    void beginGlobalSettingsUpdate();
    // Finish Global Settings Update
    void finishGlobalSettingsUpdate();

    // Get Dirty
    bool getDirty();

    // Restore Defaults
    void restoreDefaults();


signals:

    // Dirty Changed
    void dirtyChanged(const bool& aDirty);

    // Begin Global Settings Update Signal
    void globalSettingsUpdateBegin();
    // Finish Global Settings Update Signal
    void globalSettingsUpdateFinished();

    // Setting Has Changed
    void settingHasChanged(const QString& aKey);

    // ...

protected: // Constructor/Destructor

    // Constructor
    explicit SettingsController(QObject* aParent = NULL);

    // Load Settings
    void loadSettings();
    // Save Settings
    void saveSettings();

    // Destructor
    virtual ~SettingsController();

protected:

    // Set Dirty
    void setDirty(const bool& aDirty);

protected:

    // Int Ref Counter
    int                     refCount;

    // Settings
    QSettings               settings;
    // Dirty
    bool                    dirty;

    // ...

};

#endif // SETTINGSCONTROLLER_H
