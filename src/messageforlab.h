#ifndef MESSAGEFORLAB_H
#define MESSAGEFORLAB_H

#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QAction>
#include <QMessageBox>
#include <QDebug>

class MessageForLab : public QWidget
{
    Q_OBJECT
public:
    explicit MessageForLab(QWidget *parent = 0);

signals:

    void downloadUpdateApp();

public slots:

    void updateApp(QString ver);

    void aboutApp();
};

#endif // MESSAGEFORLAB_H
