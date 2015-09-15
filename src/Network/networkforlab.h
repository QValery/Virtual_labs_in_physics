#ifndef NETWORKFORLAB_H
#define NETWORKFORLAB_H

#include <QtNetwork/QtNetwork>
#include <QFile>
#include <QDebug>

class NetworkForLab: public QObject
{
    Q_OBJECT
public:
    NetworkForLab(QObject *ptr);
    ~NetworkForLab();


public slots:

    /// Начать проверку обновлений
    void startCheckUpdates();

    /// Начать обновление программы
    void startAppUpdates();

private:

    /// Загрузка данных
    void downloadData(QString url);

    void downloadUserInfoOfsMoodle();

    void readyRead(QNetworkReply *reply);

    void finishDownloadVersion(QString newVer);

private slots:

    void replyFinished(QNetworkReply* reply);

    void SSLErrors(QNetworkReply* r, QList<QSslError> errors);

signals:

    void error(int err);
    void applicationUpdate(QString newVersion);
};

#endif // NETWORKFORLAB_H
