#ifndef CURSORMODIFICATION_H
#define CURSORMODIFICATION_H

#include <QPixmap>
#include <QCursor>

class CursorModification
{
public:

    enum CursorTypes
    {
        CursorTypeDefault = 0,
        CursorTypePlus = 1,
        CursorTypeMinus = 2
    };

    static QCursor cursor(CursorTypes type);
};

#endif // CURSORMODIFICATION_H
