#include "colorbuffer.h"

ColorBuffer::ColorBuffer()
{
}

ColorBuffer::~ColorBuffer()
{
    clear();
}

void ColorBuffer::clear()
{
    _listColors.clear();
    for (int i=0; i<_listGroupNames.size(); i++)
    {
        _listGroupNames[i].listColor.clear();
        _listGroupNames[i].str.clear();
    }
    _listGroupNames.clear();
}

vect3uch ColorBuffer::randColor()
{
    vect3uch col;
    col.r = rand()%255;
    col.g = rand()%255;
    col.b = rand()%255;
    return col;
}

void ColorBuffer::pushColor(vect3uch col, QString strNme)
{
    _tempGroupNames.listColor.append(_listColors.size());
    _tempGroupNames.listNames.append(strNme);
    _listColors.append(col);
}

vect3uch ColorBuffer::pushRandColor(QString strNme)
{
    vect3uch col;
    int i;
    for (i=0; ; i++)
    {
        col= randColor();
        if (select(col) == -1) break;
        if (i > 100)
        {
            col.b = col.g = col.r = 0;
            return col;
        }
    }
    pushColor(col,strNme);
    return col;
}



void ColorBuffer::pushGroupObjectName(int name, QString strNme)
{
    _tempGroupNames.indexName = name;
    _tempGroupNames.str = strNme;
}

void ColorBuffer::endGroupNames()
{
    _listGroupNames.append(_tempGroupNames);
    _tempGroupNames.listColor.clear();
    _tempGroupNames.str.clear();
}


int ColorBuffer::size()
{
    return _listColors.size();
}

int ColorBuffer::select(vect3uch col)
{
    if (_listColors.isEmpty()) return -1;
    int i;
    for (i=0; i<_listColors.size(); i++)
    {
        if (_listColors[i] == col) return i;
    }
    return -1;
}

int ColorBuffer::select(const unsigned char *color, unsigned char type)
{
    if (_listColors.isEmpty()) return -1;
    vect3uch col;

    switch(type)
    {
    case 3:
        col.r = color[0];
        col.g = color[1];
        col.b = color[2];
        break;
    default: return -1;
    }
    return select(col);
}

int ColorBuffer::selectOfGroup(const unsigned char *color, unsigned char type)
{
    return selectOfGroup(select(color, type));
}

int ColorBuffer::selectOfGroup(const int indexColor)
{
    if (indexColor < 0) return -1;
    int i,j;
    for (i=0; i<_listGroupNames.size(); i++)
    {
        for (j=0; j<_listGroupNames.at(i).listColor.size(); j++)
        {
            if (_listGroupNames.at(i).listColor.at(j) == indexColor) return _listGroupNames.at(i).indexName;
        }
    }
    return -1;
}
