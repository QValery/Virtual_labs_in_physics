#ifndef CAMERAFORGL_H
#define CAMERAFORGL_H

#include "lib3dstypes.h"
#include "math.h"

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QDebug>

class AnimationCameraGL : public QObject
{
    Q_OBJECT
public:

    explicit AnimationCameraGL(QObject *ptr = 0);
    AnimationCameraGL(QString file, int intervalMsec, QObject *ptr = 0);
    ~AnimationCameraGL();
    void clear();

    vect6f currentCoordinat() const;
    vect3f currentEyeCoordinat() const;
    vect3f currentCenterCoordinat() const;
    int size() const;

public slots:

    void setAutoDestruction(bool on);

    void setAnimationsPoint(QString file);
    void setTimeInterval(int i);

    void startAnimation();
    void pauseAnimation();
    void stopAnimation();

signals:

    void changeCoordinats();
    void changeCoordinats(vect3f eye, vect3f center);
    void done();

private:

    bool _autoDestruction;
    QTimer *_timer;

    QList<vect6f> _listPoints;
    int _currentIndex;

    void readFilePoints(QString fileName);

private slots:

    void nextCoordinat();

};

class CameraForGL
{
public:
    CameraForGL();
    ~CameraForGL();
    void clear();

    vect3f eye() const;
    vect3f center() const;
    vect3f up() const;
    float radius() const;

    void setEye(float x, float y, float z);
    void setEye(vect3f v);
    void setCenter(float x, float y, float z);
    void setCenter(vect3f v);
    void setUp(float x, float y, float z);
    void setUp(vect3f v);

    /// Устанавливаем радиус скругления углов
    void setRadius(float r);

    /// Ограничения
    void setEdgesPoint(vect3f firstPoint, vect3f lastPoint);

    /// 'l' - влево
    /// 'r' - вправо
    /// 'h' - вверх
    /// 'b' - вниз
//    void offset(unsigned char side);

    void offsetEye(float offsGor, float offsVert = 0);

    void offsetCenter(float offsX, float offsY = 0, float offsZ =0);

    /// Приближение камеры
//    void loop(float step);

private:

    vect3f _eye;
    vect3f _center;
    vect3f _up;
    float _radius;
    vect3f _firstEdgesPoint;
    vect3f _lastEdgesPoint;

    vect2f offsetEllipseOnPoint(vect2f point1, vect2f point2, float radius, vect2f current, float offs);

    vect2f offsetEllipse(float height, float width, float radius, vect2f current, float offs);

    void recalculation(bool eye, bool center = false);

    /*--------------------Математика----------------------*/

    float fheight(float height, float width, float radius, unsigned char side, float x_height);
    float fellipse(float height, float width, float radius, unsigned char side, float x_height);
    float fellipseCoord(vect2f point1, vect2f point2, float radius, unsigned char side, float x_height);
};

#endif // CAMERAFORGL_H
