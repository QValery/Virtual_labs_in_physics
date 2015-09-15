#ifndef WIDGET3DS_H
#define WIDGET3DS_H

#include <QGLWidget>
#include <GL/glu.h>
#include <QFileInfo>
#include <QImage>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPushButton>
#include <QBitmap>
#include <QLabel>

#include <QDebug>

#include "math.h"
#include "Calculations/mathforgl.h"
#include "lib3dstypes.h"
#include "lib3d.h"
#include "colorbuffer.h"
#include "Utils3dsDevice/Constants.h"
#include "Utils3dsDevice/modelwork.h"
#include "cameraforgl.h"
#include "cursormodification.h"
#include "Utils3dsDevice/pindevicecostants.h"
#include "Utils3dsDevice/wireconnection.h"

class Widget3ds : public QGLWidget
{
    Q_OBJECT
public:
    Widget3ds(QWidget* pwgt = 0);
    ~Widget3ds();

    QString toString();

public slots:
    void updateDispley();
    void redrawScene();
    void clear();

    /// Включить масштабирование
    void setScale(bool on);

    void setStepRotationScene(int step);

    void setWiresConnectionClass(WireConnection *w);

    void setNavigationWidget(bool on);
    void setNavigationWidget();

private slots:

    void navigationButtonClickedUpEye();
    void navigationButtonClickedDownEye();
    void navigationButtonClickedLeftEye();
    void navigationButtonClickedRightEye();

    void navigationButtonClickedUp();
    void navigationButtonClickedDown();
    void navigationButtonClickedLeft();
    void navigationButtonClickedRight();

    void connectDevice(vect3f dev1, vect3f dev2, PinDeviceConstants::PinType type);
    void disconnectDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2);

signals:
    void error(unsigned short type);

    void cameraPosition(vect6f p);

protected:
    virtual void   initializeGL ();
    virtual void   resizeGL (int nWidth, int nHeight);
    virtual void   paintGL ();
    virtual void   mousePressEvent(QMouseEvent* pe);
    virtual void   mouseReleaseEvent(QMouseEvent* pe);
    virtual void   mouseMoveEvent (QMouseEvent* pe);
    virtual void   wheelEvent(QWheelEvent *e);
    virtual void   keyPressEvent(QKeyEvent *k);

private:
    enum MouseMove
    {
        NoInfoMove = 0,
        RotateScene = 1,
        DeviceMoving = 2
    };

    typedef struct ListWires
    {
        QList<GLuint> listsCompleteWires;
        QList<PinDeviceConstants::PinOfDevice> dev1;
        QList<PinDeviceConstants::PinOfDevice> dev2;
    }ListWires;

    GLint _viewport[4];
    ColorBuffer _colorBuf;
    int _selectedDevice;
    bool _isSelectedWire;
    QPoint  _positionPoint;
    QList<GLuint> _listsComplete;
    QList<GLuint> _listsCompleteForSelectBuf;
    QList<Model3ds> _model3ds;
    unsigned char _error;
    float _radSphere;
    bool _scale;
    int _stepRotationScene;
    MouseMove _mouseStatusMove;
    vect6f _limitsCoordinatDevices;
    ModelWork _modelWork;
    unsigned char _tempPinDevice;
    CameraForGL _camera;
    bool _navigationWidget;
    ListWires _listWires;
    WireConnection *_wireConnections = NULL;


    void loadTextures();
    bool loadTexture(Model3ds *model);
    GLuint loadTextureData(MaterialInfo3ds mat);

    GLuint drawRoom();
    void drawModels();
    GLuint drawModel(Model3ds model);

    GLuint drawWire(vect3f p1, vect3f p2, vect3uch color);

    /// Установка начальных значений камеры
    void setDefaultCameraSettings();

    /// Распределение объектов по сцене
    void allocationDevice();

    /// Смещение прибора с индексом modelIndex на колличество пикселей по нужной оси
    void offsetDevice(int modelIndex, int x, int y, int z);

    void rotateDevice(int modelIndex, float rotX,float rotY,float rotZ);

/*------------------------ Выбор объектов ----------------------------*/

    /// Вызывается при щелчке мышью
    int selectObject(QPoint p);

    /// Вызывается для обновления временных данных буфера выделения
    /// intoWire - строить ли провода в буфере?
    void updateSelectBuffers();
    void updateListCompleteSelectBuf(bool intoWire = true);

    GLuint drawModelOnlyFigure(Model3ds model, ColorBuffer *buf);

/*----------------------------------------------------------------------*/

/*----------------------- Панель навигации -------------------------------*/

    void createNavigationWidgetButtons();
    void deleteNavigationWidgetButton();

    QList<vect2f> calculateCircle(float rad, float countPart, float x=0, float y=0);
    GLuint drawCircle(QList<vect2f> vertex);
};

#endif // WIDGET3DS_H
