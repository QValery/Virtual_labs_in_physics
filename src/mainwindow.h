#ifndef _OGLPyramid_h_
#define _OGLPyramid_h_

#include <QMainWindow>
#include <QKeySequence>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include "widget3ds.h"
#include "Network/networkforlab.h"
#include "messageforlab.h"
#include "settingsapp.h"

#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* pwgt = 0);

private:

    Widget3ds *_widget3ds;
    NetworkForLab *_networkPart;
    SettingsApp *_setting;

    QAction *_cameraPositionAction = NULL;

private slots:
    void about();
    void aboutProg();
    void aboutGraph();
    void writeVersion(QString ver);

    void navigationPaneleForWidget3ds();
    void changeSettingApp(SettingsApp::TypeSetting t);

    void changePositionCamera(vect6f p);
public slots:

};
#endif  //_OGLPyramid_h_
