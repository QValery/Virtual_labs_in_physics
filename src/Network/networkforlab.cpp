#include "networkforlab.h"

NetworkForLab::NetworkForLab(QObject *ptr) : QObject(ptr)
{
//    downloadUserInfoOfsMoodle();
}

NetworkForLab::~NetworkForLab()
{
}

/*------------------ Загрузка данных ---------------------*/

void NetworkForLab::downloadData(QString url)
{
    if (url.isEmpty()) return;
    QNetworkAccessManager *http = new QNetworkAccessManager (this);
    connect(http, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    http->get(QNetworkRequest(QUrl(url)));
}

void NetworkForLab::downloadUserInfoOfsMoodle()
{
    QNetworkAccessManager *http = new QNetworkAccessManager (this);
    connect(http, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(http,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(SSLErrors(QNetworkReply*,QList<QSslError>)));
    QNetworkRequest *req = new QNetworkRequest();
    req->setUrl(QUrl("https://edu.pgta.ru/login/index.php"));
    req->setAttribute(QNetworkRequest::CookieLoadControlAttribute, true);
    http->put(*req,QString("username=s27959&password=10011995&input%20type=%22submit%22").toLatin1());
}

void NetworkForLab::replyFinished(QNetworkReply *reply)
{
    if (reply->error()==QNetworkReply::NoError)
    {
        readyRead(reply);
    }
    else emit error(reply->error());
    qDebug()<<reply->error();
}

void NetworkForLab::SSLErrors(QNetworkReply *r, QList<QSslError> errors)
{
    for (int i=0; i<errors.size(); i++)
    {
        qDebug()<<errors.at(i);
    }
}

void NetworkForLab::readyRead(QNetworkReply *reply)
{
    QString fileName;
    QFileInfo inf(reply->url().toString());
    fileName = inf.fileName();
    if (fileName == "version.txt")
    {
        finishDownloadVersion(reply->readAll());
        return;
    }
    else if ((fileName == "LabworkOnElectric")||(fileName == "lib3d"))
    {
        QFile file;
        file.setFileName(QCoreApplication::applicationDirPath()+'/'+fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            emit error(1000);
            return;
        }
        file.write(reply->readAll());
        file.close();
        if (fileName == "LabworkOnElectric") file.rename(fileName+".exe");
        else file.rename(fileName+".lib");
    }
    else
    {
        QFile file;
        file.setFileName("E:/"+fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            emit error(1000);
            return;
        }
        file.write(reply->readAll());
        file.close();
    }
    return;
}

/*----------------------------------------------------------*/

/*--------------------- Обновления приложения --------------*/

void NetworkForLab::startCheckUpdates()
{
    downloadData("http://labonphysics.3dn.ru/version.txt");
}

void NetworkForLab::finishDownloadVersion(QString newVer)
{
    QString thisVersion = QCoreApplication::applicationVersion();
    if (thisVersion != newVer) emit applicationUpdate(newVer);
    else emit applicationUpdate("");
}

void NetworkForLab::startAppUpdates()
{
    downloadData("http://labonphysics.3dn.ru/LabworkOnElectric");
    downloadData("http://labonphysics.3dn.ru/lib3d");
    qDebug()<<"undateClick";
    return;
}

/*----------------------------------------------------------*/
