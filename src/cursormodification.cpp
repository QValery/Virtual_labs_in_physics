#include "cursormodification.h"


QCursor CursorModification::cursor(CursorTypes type)
{
    QPixmap map;
    QSize size(32,32);
    switch(type)
    {
    case CursorTypeDefault: return QCursor(Qt::ArrowCursor);
    case CursorTypePlus:
        map.load(":/new/cursors/icon_plus.png");
        map = map.scaled(size);
        return QCursor(map);
    case CursorTypeMinus:
        map.load(":/new/cursors/icon_minus.png");
        map = map.scaled(size);
        return QCursor(map);
    }
    return QCursor();
}
