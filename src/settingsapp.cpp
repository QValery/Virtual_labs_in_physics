#include "settingsapp.h"

SettingsWidget::SettingsWidget(int stepRotationSceneDefault, bool autoUpdateAppDefault, QWidget *ptr):QWidget(ptr)
{
    _autoUpdateApplValue = autoUpdateAppDefault;
    _stepRotationSceneValue = stepRotationSceneDefault;
    _autoUpdateApplValueTemp = autoUpdateAppDefault;
    _stepRotationSceneValueTemp = stepRotationSceneDefault;

    QCommandLinkButton *partDefault = new QCommandLinkButton("Основные", this);
    connect(partDefault,SIGNAL(clicked()),this,SLOT(partDefaultShow()));
    QCommandLinkButton *partScene = new QCommandLinkButton("Сцена", this);
    connect(partScene,SIGNAL(clicked()),this,SLOT(partSceneShow()));

    QVBoxLayout *partsSetting = new QVBoxLayout();
    partsSetting->addWidget(partDefault, 0, Qt::AlignTop);
    partsSetting->addWidget(partScene, 0, Qt::AlignTop);

    _rightSideWindiwSetting = new QWidget(this);
    _typePartSettings = PartRightNull;
    partDefaultShow();

    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addLayout(partsSetting);
    settingsLayout->addWidget(_rightSideWindiwSetting);

    QPushButton *Ok = new QPushButton("ОК", _rightSideWindiwSetting);
    Ok->setShortcut(QKeySequence(Qt::Key_Return));
    connect(Ok,SIGNAL(clicked()),this,SLOT(closeWindowSetting()));
    QPushButton *closeWindowSettingButton = new QPushButton("Отмена", _rightSideWindiwSetting);
    closeWindowSettingButton->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(closeWindowSettingButton,SIGNAL(clicked()),this,SLOT(close()));

    QVBoxLayout *widgetLayout = new QVBoxLayout(this);
    widgetLayout->addLayout(settingsLayout);
    widgetLayout->addWidget(Ok);
    widgetLayout->addWidget(closeWindowSettingButton);

    this->setLayout(widgetLayout);
    this->setWindowModality(Qt::ApplicationModal);
    this->show();
    this->setWindowTitle("Настрйоки");

    partScene->setFixedWidth(partDefault->width()-30);
    partDefault->setFixedWidth(partScene->width());
}

SettingsWidget::~SettingsWidget()
{
}

bool SettingsWidget::autoUpdateApplValue() const
{
    return _autoUpdateApplValueTemp;
}

int SettingsWidget::stepRotationSceneValue() const
{
    return _stepRotationSceneValueTemp;
}

void SettingsWidget::clearRightSideWindiwSetting()
{
    QObjectList list = _rightSideWindiwSetting->children();
    for (int i = 0; i<list.size(); i++)
    {
        list.at(i)->deleteLater();
        delete list.at(i);
    }
}

void SettingsWidget::readRightSideWindiwSetting()
{
    if (_autoUpdateAppl == NULL) return;
    switch(_typePartSettings)
    {
    case PartRightDefault:
        _autoUpdateApplValueTemp = _autoUpdateAppl->isChecked();
        break;
    case PartRightScene:
        _stepRotationSceneValueTemp = _stepRotationScene->value();
        break;
    }
}

void SettingsWidget::partDefaultShow()
{
    if (_typePartSettings == PartRightDefault) return;
    readRightSideWindiwSetting();
    _typePartSettings = PartRightDefault;
    clearRightSideWindiwSetting();

    QVBoxLayout *layout = new QVBoxLayout(_rightSideWindiwSetting);
    _autoUpdateAppl = new QCheckBox("Автообновление приложения", _rightSideWindiwSetting);
    _autoUpdateAppl->setChecked(_autoUpdateApplValueTemp);

    layout->addWidget(_autoUpdateAppl,0, Qt::AlignCenter);
    _rightSideWindiwSetting->setLayout(layout);
}

void SettingsWidget::partSceneShow()
{
    if (_typePartSettings == PartRightScene) return;
    readRightSideWindiwSetting();
    _typePartSettings = PartRightScene;
    clearRightSideWindiwSetting();

    _stepRotationScene = new QSpinBox(_rightSideWindiwSetting);
    _stepRotationScene->setMaximum(40);
    _stepRotationScene->setMinimum(1);
    _stepRotationScene->setValue(_stepRotationSceneValueTemp);

    QFormLayout *layout = new QFormLayout(_rightSideWindiwSetting);
    layout->addRow("Шаг поворота сцены", _stepRotationScene);

    _rightSideWindiwSetting->setLayout(layout);
}

void SettingsWidget::closeWindowSetting()
{
    readRightSideWindiwSetting();
    if (_autoUpdateApplValue != _autoUpdateApplValueTemp)
    {
        emit changed();
    }
    if (_stepRotationSceneValue != _stepRotationSceneValueTemp)
    {
        emit changed();
    }
    close();
}



SettingsApp::SettingsApp(QObject *p):QObject(p)
{
    _autoUpdateApplValue = false;
    _stepRotationSceneValue = 2;
}

SettingsApp::~SettingsApp()
{
    if (_settingsWidget != NULL)
    {
        _settingsWidget->deleteLater();
        delete _settingsWidget;
    }
}

void SettingsApp::openWindow()
{
    _settingsWidget = new SettingsWidget(_stepRotationSceneValue, _autoUpdateApplValue);
    connect(_settingsWidget,SIGNAL(changed()),this,SLOT(changedWindowSetting()));
}

void SettingsApp::openStepSizeSetting()
{
    openWindow();
    _settingsWidget->partSceneShow();
}

bool SettingsApp::autoUpdateApplValue() const
{
    return _autoUpdateApplValue;
}

int SettingsApp::stepRotationSceneValue() const
{
    return _stepRotationSceneValue;
}


void SettingsApp::closeWindowSetting()
{
    qDebug()<<"kjfhkjdrthg";
}

void SettingsApp::changedWindowSetting()
{
    if (_autoUpdateApplValue != _settingsWidget->autoUpdateApplValue())
    {
        _autoUpdateApplValue = _settingsWidget->autoUpdateApplValue();
        emit changedTypeSetting(TypeSettingAutoUpdateApp);
    }
    if (_stepRotationSceneValue != _settingsWidget->stepRotationSceneValue())
    {
        _stepRotationSceneValue = _settingsWidget->stepRotationSceneValue();
        emit changedTypeSetting(TypeSettingStepRotationScene);
    }
    emit changedSetting();
}
