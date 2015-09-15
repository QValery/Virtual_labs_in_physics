#include "MainWindow.h"

// ----------------------------------------------------------------------
MainWindow::MainWindow(QWidget* pwgt) : QMainWindow(pwgt)
{
    _widget3ds = new Widget3ds(this);
    _widget3ds->setNavigationWidget();
    this->setCentralWidget(_widget3ds);
    _cameraPositionAction = new QAction(this);
    _cameraPositionAction->setEnabled(false);
    connect(_widget3ds,SIGNAL(cameraPosition(vect6f)),this,SLOT(changePositionCamera(vect6f)));

    _networkPart = new NetworkForLab(this);
    connect(_networkPart,SIGNAL(applicationUpdate(QString)),this,SLOT(writeVersion(QString)));

    _setting = new SettingsApp(this);
    connect(_setting,SIGNAL(changedTypeSetting(SettingsApp::TypeSetting)),this,SLOT(changeSettingApp(SettingsApp::TypeSetting)));


    QAction *exitAction = new QAction("Закрыть программу", this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    QAction *settingsApp = new QAction("Настройки", this);
    settingsApp->setShortcut(QKeySequence("F12", QKeySequence::NativeText));
    connect(settingsApp,SIGNAL(triggered()),_setting,SLOT(openWindow()));

    QMenu *file = new QMenu("Файл", this);
    file->addAction(settingsApp);
    file->addSeparator();
    file->addAction(exitAction);

    QAction *redrawSceneAction = new QAction("Перестроить", this);
    redrawSceneAction->setShortcut(QKeySequence("Ctrl+F5", QKeySequence::NativeText));
    connect(redrawSceneAction,SIGNAL(triggered()),_widget3ds,SLOT(redrawScene()));

    QAction *sceneSizeStepAction = new QAction("Шаг поворота сцены", this);
    sceneSizeStepAction->setShortcut(QKeySequence("E", QKeySequence::NativeText));
    connect(sceneSizeStepAction,SIGNAL(triggered()),_setting,SLOT(openStepSizeSetting()));

    QAction *navigationWidget = new QAction("Панель навигации",this);
    connect(navigationWidget,SIGNAL(triggered()),this,SLOT(navigationPaneleForWidget3ds()));

    QMenu *scene = new QMenu("Сцена", this);
    scene->addAction(redrawSceneAction);
    scene->addSeparator();
    scene->addAction(navigationWidget);
    scene->addAction(sceneSizeStepAction);


    QAction *about = new QAction("О программе", this);
    connect(about,SIGNAL(triggered()),this,SLOT(about()));

    QAction *aboutGraphAction = new QAction("О граф. системе", this);
    connect(aboutGraphAction,SIGNAL(triggered()),this,SLOT(aboutGraph()));

    QAction *aboutProgram = new QAction("О фреймворке", this);
    connect(aboutProgram,SIGNAL(triggered()),this,SLOT(aboutProg()));

    QAction *checkUpdateApp = new QAction("Проверить обновления", this);
    connect(checkUpdateApp,SIGNAL(triggered()),_networkPart,SLOT(startCheckUpdates()));

    QMenu *help = new QMenu("Справка", this);
    help->addAction(about);
    help->addAction(aboutGraphAction);
    help->addAction(aboutProgram);
    help->addSeparator();
    help->addAction(checkUpdateApp);


    QMenuBar *bar = new QMenuBar(this);
    bar->addMenu(file);
    bar->addMenu(scene);
    bar->addMenu(help);
    bar->addAction(_cameraPositionAction);
    this->setMenuBar(bar);
/*
    QToolBar *topTool = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea,topTool);
    topTool->setMovable(false);
*/
    this->setMinimumSize(400, 300);
}

void MainWindow::about()
{
    MessageForLab *mes = new MessageForLab(this);
    mes->aboutApp();
}

void MainWindow::aboutProg()
{
    QMessageBox::aboutQt(this,"О Qt");
}

void MainWindow::aboutGraph()
{
    QMessageBox::information(this,"О графической подсистеме",_widget3ds->toString(),QMessageBox::Ok);
}

void MainWindow::navigationPaneleForWidget3ds()
{
    _widget3ds->setNavigationWidget();
}

void MainWindow::writeVersion(QString ver)
{
    MessageForLab *mes = new MessageForLab(this);
//    connect(mes,SIGNAL(downloadUpdateApp()),NetworkForLab::startAppUpdates());
    connect(mes,SIGNAL(downloadUpdateApp()),_networkPart,SLOT(startAppUpdates()));
    mes->updateApp(ver);
}

void MainWindow::changeSettingApp(SettingsApp::TypeSetting t)
{
    switch(t)
    {
    case SettingsApp::TypeSettingStepRotationScene:
        _widget3ds->setStepRotationScene(_setting->stepRotationSceneValue());
        break;
    }
}

void MainWindow::changePositionCamera(vect6f p)
{
    QString str;
    str.append("Позиция камеры: x = "+QString::number(p.x)+", y = "+QString::number(p.y)+", z = "+QString::number(p.z));
    str.append(" Направление камеры: x = "+QString::number(p.x1)+", y = "+QString::number(p.y1)+", z = "+QString::number(p.z1));
    _cameraPositionAction->setText(str);
}
