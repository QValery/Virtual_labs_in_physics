#include "messageforlab.h"

MessageForLab::MessageForLab(QWidget *parent) :
    QWidget(parent)
{
}

void MessageForLab::updateApp(QString ver)
{
    QMessageBox *mesbox = new QMessageBox(this);
    mesbox->setWindowTitle("Обновление программы");
    QString str;
    bool temp;
    if (ver.isEmpty())
    {
        mesbox->setIcon(QMessageBox::Information);
        str = "Обновлений не требуется";
        temp = false;
    }
    else
    {
        mesbox->setIcon(QMessageBox::Question);
        str.append("Обнаружена новая версия!\nПоследняя версия ").append(ver);
        mesbox->setInformativeText("Хотите обновить?");
        mesbox->addButton("Обновить",QMessageBox::AcceptRole);
        temp = true;
    }
    mesbox->setText(str);
    mesbox->addButton("Отмена",QMessageBox::RejectRole);
    if ((mesbox->exec() == QMessageBox::AcceptRole)&&(temp))
    {
        QDesktopServices::openUrl (QUrl("http://labonphysics.3dn.ru/index/programma_dlja_kompjutera/0-4"));
//        emit downloadUpdateApp();
    }
    delete mesbox;
}

void MessageForLab::aboutApp()
{
    QMessageBox *mesbox = new QMessageBox(this);
    mesbox->setWindowTitle("О программе");
    mesbox->setTextFormat(Qt::RichText);
    mesbox->setText("<h2><p>Виртуальные лабороторные работы по физике</p></h2>");
    QString str;
    str.append("<a href=\"https://vk.com/walera3000\">Куклёнков Валерий</a><br>");
    str.append("<a href=\"https://vk.com/id71841275\">Саяпина Елена</a><br>");
    str.append("<a href=\"https://vk.com/jiososb\">Лутохин Андрей</a>");
    mesbox->setInformativeText(str);
    mesbox->setIconPixmap(QPixmap(":/new/icons/iconFisics.png"));
    mesbox->addButton("Перейти на сайт", QMessageBox::AcceptRole);
    mesbox->addButton("Закрыть", QMessageBox::RejectRole);
    if (mesbox->exec() == QMessageBox::AcceptRole)
        QDesktopServices::openUrl (QUrl("http://labonphysics.3dn.ru/index/programma_dlja_kompjutera/0-4"));
    delete mesbox;
}
