#-------------------------------------------------
#
# Project created by QtCreator 2014-09-21T10:07:11
#
#-------------------------------------------------

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += $$PWD/main.cpp\
        $$PWD/mainwindow.cpp \
    $$PWD/widget3ds.cpp \
    $$PWD/Calculations/calculations.cpp \
    $$PWD/Calculations/physicsfunction.cpp \
    $$PWD/Utils3dsDevice/device.cpp \
    $$PWD/Calculations/mathforgl.cpp \
    $$PWD/colorbuffer.cpp \
    $$PWD/Network/networkforlab.cpp \
    $$PWD/messageforlab.cpp \
    $$PWD/settingsapp.cpp \
    $$PWD/Utils3dsDevice/modelwork.cpp \
    $$PWD/cameraforgl.cpp \
    $$PWD/cursormodification.cpp \
    $$PWD/Utils3dsDevice/wireconnection.cpp

HEADERS  += $$PWD/mainwindow.h \
    $$PWD/widget3ds.h \
    $$PWD/Utils3dsDevice/Constants.h \
    $$PWD/Calculations/calculations.h \
    $$PWD/Calculations/physicsfunction.h \
    $$PWD/Utils3dsDevice/device.h \
    $$PWD/Calculations/mathforgl.h \
    $$PWD/colorbuffer.h \
    $$PWD/Network/networkforlab.h \
    $$PWD/messageforlab.h \
    $$PWD/settingsapp.h \
    $$PWD/Utils3dsDevice/modelwork.h \
    $$PWD/cameraforgl.h \
    $$PWD/cursormodification.h \
    $$PWD/Utils3dsDevice/pindevicecostants.h \
    $$PWD/Utils3dsDevice/wireconnection.h

INCLUDEPATH += $$PWD/../../lib3d-bild/release
DEPENDPATH += $$PWD/../../lib3d-bild/release

#LIBS += -L$$PWD/../../lib3d-bild/release/ -llib3d

RESOURCES += \
    res/resoures.qrc
