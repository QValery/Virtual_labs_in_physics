#ifndef SETTINGSAPP_H
#define SETTINGSAPP_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QCommandLinkButton>
#include <QPixmap>
#include <QSettings>

#include <QDebug>

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:

    SettingsWidget(int stepRotationSceneDefault = 1, bool autoUpdateAppDefault = false, QWidget *ptr = 0);
    ~SettingsWidget();

    bool autoUpdateApplValue() const;
    int stepRotationSceneValue() const;

public slots:

    void partDefaultShow();
    void partSceneShow();

signals:

    void changed();

private:

    enum TypePartRightSettings
    {
        PartRightNull = 0,
        PartRightDefault = 1,
        PartRightScene = 2
    };

    QCheckBox *_autoUpdateAppl = NULL;
    QSpinBox *_stepRotationScene = NULL;

    TypePartRightSettings _typePartSettings;
    QWidget *_rightSideWindiwSetting;

    void clearRightSideWindiwSetting();
    void readRightSideWindiwSetting();

    /*-----Данные-----*/

    bool _autoUpdateApplValue;
    bool _autoUpdateApplValueTemp;

    int _stepRotationSceneValue;
    int _stepRotationSceneValueTemp;

    /*-------------------*/

private slots:

    void closeWindowSetting();

};

class SettingsApp : public QObject
{
    Q_OBJECT
public:
    enum TypeSetting
    {
        TypeSettingNull = 0,
        TypeSettingAutoUpdateApp = 1,
        TypeSettingStepRotationScene = 2
    };

    SettingsApp(QObject *p = 0);
    ~SettingsApp();

    bool autoUpdateApplValue() const;
    int stepRotationSceneValue() const;

public slots:

//    void readSettingsOfFile();
//    void writeSettingsOfFile();

    void openWindow();
    void openStepSizeSetting();

signals:

    void changedTypeSetting(SettingsApp::TypeSetting t);
    void changedSetting();

private:

    int _stepRotationSceneValue;
    bool _autoUpdateApplValue;


    SettingsWidget *_settingsWidget = NULL;

private slots:

    void closeWindowSetting();
    void changedWindowSetting();
};

#endif // SETTINGSAPP_H
