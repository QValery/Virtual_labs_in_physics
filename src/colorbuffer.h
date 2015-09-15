#ifndef COLORBUFFER_H
#define COLORBUFFER_H

#include <QList>
#include <QStringList>
#include "lib3dstypes.h"

class ColorBuffer
{
public:
    ColorBuffer();
    ~ColorBuffer();

    void clear();

    /// Генерирует случайный цвет
    static vect3uch randColor();

    /// Добавление цвета в стек
    void pushColor(vect3uch col, QString strNme = QString());
    vect3uch pushRandColor(QString strNme = QString());

    void pushGroupObjectName(int name, QString strNme = QString());
    void endGroupNames();

    /// Колличество объектов в стеке
    int size();

    /// Возвратит порядковый номер цвета в стеке, начиная с 1
    /// 0 если цвет не найден
    int select(vect3uch col);
    int select(const unsigned char *color, unsigned char type = 3);

    int selectOfGroup(const unsigned char *color, unsigned char type = 3);
    int selectOfGroup(const int indexColor);

private:
    typedef struct groupNames
    {
        QString str;
        int indexName;
        QList<int> listColor;
        QStringList listNames;
    }groupNames;

    groupNames _tempGroupNames;
    QList<groupNames> _listGroupNames;
    QList<vect3uch> _listColors;
};

#endif // COLORBUFFER_H
