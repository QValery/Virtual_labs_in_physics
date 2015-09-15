#-------------------------------------------------
#
# Project created by QtCreator 2014-09-21T10:07:11
#
#-------------------------------------------------

QT       += core gui
QT          += opengl
QT += network widgets

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Virtual_labs_in_physics
TEMPLATE = app

include(src/src.pri)

#CONFIG += stl_off
CONFIG += no_include_pwd
CONFIG += ordered

#VERSION = 1.1
#QMAKE_TARGET_COMPANY = PenzGTU
#QMAKE_TARGET_PRODUCT = Virtual_labs_in_physics
#QMAKE_TARGET_DESCRIPTION = Virtual_labs_in_physics
#QMAKE_TARGET_COPYRIGHT = (c) Valery Kuklyonkov and Andrey Lutohin

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib3d-bild/release/ -llib3d
#else:
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib3d-bild/release/ -llib3d


INCLUDEPATH += $$PWD/../lib3d-bild/release
DEPENDPATH += $$PWD/../lib3d-bild/release

RESOURCES += \
    res/resoures.qrc
