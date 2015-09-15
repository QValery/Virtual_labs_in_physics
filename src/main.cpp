#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/new/icons/iconApp.ico"));

    QCoreApplication::setOrganizationName("Kuklyonkov_V.S");
    QCoreApplication::setOrganizationDomain("https://vk.com/walera3000");
    QCoreApplication::setApplicationName("Лабораторная работа по физике");
    QCoreApplication::setApplicationVersion("0.1");

    MainWindow w;
    w.showMaximized();
    /*
    if (QApplication::arguments().contains(QLatin1String("-maximize")))
            w.showMaximized();
        else if (QApplication::arguments().contains(QLatin1String("-fullscreen")))
            w.showFullScreen();
        else
            w.show();
*/
    return a.exec();
}
