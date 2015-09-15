#include "cameraforgl.h"

AnimationCameraGL::AnimationCameraGL(QObject *ptr):QObject(ptr)
{
    _timer = new QTimer(this);
    clear();
    connect(_timer,SIGNAL(timeout()),this,SLOT(nextCoordinat()));
}

AnimationCameraGL::AnimationCameraGL(QString file, int intervalMsec, QObject *ptr):QObject(ptr)
{
    _timer = new QTimer(this);
    connect(_timer,SIGNAL(timeout()),this,SLOT(nextCoordinat()));
    _timer->setInterval(intervalMsec);
    clear();
    readFilePoints(file);
}

AnimationCameraGL::~AnimationCameraGL()
{
    clear();
    delete _timer;
}

void AnimationCameraGL::clear()
{
    if (!_listPoints.isEmpty()) _listPoints.clear();
    _currentIndex = -1;
    _autoDestruction = false;
    if (_timer->isActive()) _timer->stop();
}

vect6f AnimationCameraGL::currentCoordinat() const
{
    if (_currentIndex < 0) return vect6f();
    return _listPoints.at(_currentIndex);
}

vect3f AnimationCameraGL::currentEyeCoordinat() const
{
    if (_currentIndex < 0) return vect3f();
    return _listPoints.at(_currentIndex).xyz();
}

vect3f AnimationCameraGL::currentCenterCoordinat() const
{
    if (_currentIndex < 0) return vect3f();
    return _listPoints.at(_currentIndex).x1y1z1();
}

int AnimationCameraGL::size() const
{
    return _listPoints.size();
}

void AnimationCameraGL::setAutoDestruction(bool on)
{
    _autoDestruction = on;
}

void AnimationCameraGL::setAnimationsPoint(QString file)
{
    readFilePoints(file);
}

void AnimationCameraGL::setTimeInterval(int i)
{
    _timer->setInterval(i);
}

void AnimationCameraGL::startAnimation()
{
    if (_listPoints.isEmpty()) return;
    _timer->start();
}

void AnimationCameraGL::pauseAnimation()
{
    _timer->stop();
}

void AnimationCameraGL::stopAnimation()
{
    _timer->stop();
    _currentIndex = -1;
}

void AnimationCameraGL::readFilePoints(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream stream(&file);
    stream.setIntegerBase(10);
    _listPoints.clear();
    _currentIndex = -1;

    QString str;
    vect6f vect;
    bool ok;

    while(!stream.atEnd())
    {
        stream>>str;
        vect.x = str.toFloat(&ok);
        if (!ok) break;

        stream>>str;
        vect.y = str.toFloat(&ok);
        if (!ok) break;

        stream>>str;
        vect.z = str.toFloat(&ok);
        if (!ok) break;

        stream>>str;
        vect.x1 = str.toFloat(&ok);
        if (!ok) break;

        stream>>str;
        vect.y1 = str.toFloat(&ok);
        if (!ok) break;

        stream>>str;
        vect.z1 = str.toFloat(&ok);
        if (!ok) break;

        _listPoints.append(vect);
    }

    file.close();
}

void AnimationCameraGL::nextCoordinat()
{
    _currentIndex++;
    if (_currentIndex >= _listPoints.size())
    {
        _timer->stop();
        emit done();
        if (_autoDestruction) delete this;
        return;
    }
    emit changeCoordinats();
    emit changeCoordinats(_listPoints.at(_currentIndex).xyz(), _listPoints.at(_currentIndex).x1y1z1());
}



CameraForGL::CameraForGL()
{
    _radius = 0;
    _up.y = 1;
}

CameraForGL::~CameraForGL()
{
    clear();
}

void CameraForGL::clear()
{
    _radius = 0;
    _eye.clear();
    _center.clear();
    _up.clear();
    _up.y = 1;
}

vect3f CameraForGL::eye() const
{
    return _eye;
}

vect3f CameraForGL::center() const
{
    return _center;
}

vect3f CameraForGL::up() const
{
    return _up;
}

float CameraForGL::radius() const
{
    return _radius;
}

void CameraForGL::setEye(float x, float y, float z)
{
    _eye.x = x;
    _eye.y = y;
    _eye.z = z;

    recalculation(true);
}

void CameraForGL::setEye(vect3f v)
{
    setEye(v.x, v.y, v.z);
}

void CameraForGL::setCenter(float x, float y, float z)
{
    _center.x = x;
    _center.y = y;
    _center.z = z;

    recalculation(false, true);
}

void CameraForGL::setCenter(vect3f v)
{
    setCenter(v.x, v.y, v.z);
}

void CameraForGL::setUp(float x, float y, float z)
{
    _up.x = x;
    _up.y = y;
    _up.z = z;
}

void CameraForGL::setUp(vect3f v)
{
    setUp(v.x, v.y, v.z);
}

void CameraForGL::setRadius(float r)
{
    if (r>0) _radius = r;
}

void CameraForGL::setEdgesPoint(vect3f firstPoint, vect3f lastPoint)
{
    _firstEdgesPoint = firstPoint;
    _lastEdgesPoint = lastPoint;

    recalculation(true, true);
}



void CameraForGL::offsetEye(float offsGor, float offsVert)
{
    if (((_lastEdgesPoint.x - _firstEdgesPoint.x) == 0)||((_lastEdgesPoint.y - _firstEdgesPoint.y) == 0)) return;

    if (offsGor != 0)
    {
        vect2f coord;
        coord.y = _eye.z;
        coord.x = _eye.x;

        coord = offsetEllipseOnPoint(vect2f(_firstEdgesPoint.x, _firstEdgesPoint.z), vect2f(_lastEdgesPoint.x, _lastEdgesPoint.z), _radius, coord, offsGor);
        if (coord.x != -1)
        {
            _eye.z = coord.y;
            _eye.x = coord.x;
        }
    }
    if (offsVert != 0)
    {
        float tempf = _eye.y + offsVert;
        if (_firstEdgesPoint.y > _lastEdgesPoint.y)
        {
            if ((tempf < _firstEdgesPoint.y)&&(tempf > _lastEdgesPoint.y))
                _eye.y = tempf;
        }
        else
        {
            if ((tempf > _firstEdgesPoint.y)&&(tempf < _lastEdgesPoint.y))
                _eye.y = tempf;
        }
    }
}

void CameraForGL::offsetCenter(float offsX, float offsY, float offsZ)
{
    if (((_lastEdgesPoint.x - _firstEdgesPoint.x) == 0)||((_lastEdgesPoint.y - _firstEdgesPoint.y) == 0)) return;

    register float tempf;

    if (offsX != 0)
    {
        tempf = _center.x + offsX;
        if (_firstEdgesPoint.x > _lastEdgesPoint.x)
        {
            if ((tempf < _firstEdgesPoint.x)&&(tempf > _lastEdgesPoint.x))
                _center.x = tempf;
        }
        else
        {
            if ((tempf > _firstEdgesPoint.x)&&(tempf < _lastEdgesPoint.x))
                _center.x = tempf;
        }
    }
    if (offsY != 0)
    {
        tempf = _center.y + offsY;
        if (_firstEdgesPoint.y > _lastEdgesPoint.y)
        {
            if ((tempf < _firstEdgesPoint.y)&&(tempf > _lastEdgesPoint.y))
                _center.y = tempf;
        }
        else
        {
            if ((tempf > _firstEdgesPoint.y)&&(tempf < _lastEdgesPoint.y))
                _center.y = tempf;
        }
    }
    if (offsZ != 0)
    {
        tempf = _center.z + offsZ;
        if (_firstEdgesPoint.z > _lastEdgesPoint.z)
        {
            if ((tempf < _firstEdgesPoint.z)&&(tempf > _lastEdgesPoint.z))
                _center.z = tempf;
        }
        else
        {
            if ((tempf > _firstEdgesPoint.z)&&(tempf < _lastEdgesPoint.z))
                _center.z = tempf;
        }
    }
}


vect2f CameraForGL::offsetEllipseOnPoint(vect2f point1, vect2f point2, float radius, vect2f current, float offs)
{
    float height = fabsf(point2.x - point1.x);
    float width = fabsf(point2.y - point1.y);

    if (point2.x > point1.x) current.x -= point1.x;
    else current.x -= point2.x;
    if (point2.y > point1.y) current.y -= point1.y;
    else current.y -= point2.y;

    vect2f y = offsetEllipse(height,width,radius,current,offs);
    y.x += point1.x;
    y.y += point1.y;
    if (point2.y < 0) y.y*=-1;

    return y;
}

vect2f CameraForGL::offsetEllipse(float height, float width, float radius, vect2f current, float offs)
{
    unsigned char side;
    bool pryamSistCoord;

    if (fabsf(radius / offs) < 10)
    {
        float napr = 1;
        if (offs < 0) napr = -1;
        while (fabsf(radius / offs) < 10)
        {
            current = offsetEllipse(height, width, radius, current, radius / 10 * napr);
            offs -= radius / 10 * napr;
        }
    }

    if ((current.y > (width - radius/2))||(current.y < (radius/2)))
    {
        if((current.x > (radius/2))&&(current.x < (height - radius/2)))
            pryamSistCoord = true;
    }
    else pryamSistCoord = false;

    vect2f newCoord;

    if (pryamSistCoord)
    {
        if (current.y < (width/2))
        {
            offs*=-1;
            side = 'l';
        }
        else
        {
            side = 'r';
        }

        if ((current.x + offs) > height)
        {
            newCoord.x = height;
            qDebug()<<"newCoord.x = height";
        }
        else if ((current.x + offs) < 0)
        {
            newCoord.x = 0;
            qDebug()<<"newCoord.x = 0";
        }
        else
            newCoord.x = current.x + offs;

        newCoord.y = fellipse(height, width, radius, side, newCoord.x);
    }
    else
    {
        if (current.x < (height/2))
        {
            side = 'l';
        }
        else
        {
            offs *=-1;
            side = 'r';
        }

        if ((current.y + offs) > width)
        {
            newCoord.y = width;
            qDebug()<<"newCoord.y = width";
        }
        else if ((current.y + offs) < 0)
        {
            newCoord.y = 0;
            qDebug()<<"newCoord.y = 0";
        }
        else
            newCoord.y = current.y + offs;

        newCoord.x = fellipse(width, height, radius, side, newCoord.y);
//        newCoord.x = fellipseCoord(vect2f(_firstEdgesPoint.x, _firstEdgesPoint.z), vect2f(_lastEdgesPoint.x, _lastEdgesPoint.z), radius, side, newCoord.y);
    }
//    qDebug()<<pryamSistCoord<<newCoord.x<<newCoord.y;
/*    QString str;
    str.append(QString::number(newCoord.x)+'\t');
    str.append(QString::number(newCoord.y)+"\r\n");
    str.replace('.',',');
    QFile file("E:/вуз/Дипломник/физика & математика/Орбита для камеры/coord.txt");
    if (file.open(QIODevice::Append))
    {
        file.write(str.toUtf8());
        file.close();
    }
*/
    return newCoord;
}

void CameraForGL::recalculation(bool eye, bool center)
{
    float height = _lastEdgesPoint.x - _firstEdgesPoint.x;
    float width = _lastEdgesPoint.y - _firstEdgesPoint.y;
    if (((height) == 0)||((width) == 0)) return;
    if (_radius == 0)
    {
        if (height > width)
        {
            _radius = width/10;
        }
        else
        {
            _radius = height/10;
        }
    }

    if (eye) offsetEye(1);

    if (center) offsetCenter(1);
}

/*---------------------Математика--------------------*/

float CameraForGL::fheight(float height, float width, float radius, unsigned char side, float x_height)
{
    if ((side != 'l')&&(side != 'r')) return -1;
    float Ox = height - radius;
    if (x_height < 0) return -1;
    float y = -1, Oy;
    if ((height - x_height) > radius)
    {
        if (side == 'l')
            y = 0;
        else
            y = width;
        return y;
    }
    if (side == 'l')
    {
        Oy = radius;
        y = -sqrtf(radius*radius - (x_height - Ox)*(x_height - Ox)) + Oy;
    }
    else
    {
        Oy = width - radius;
        y = sqrtf(radius*radius - (x_height - Ox)*(x_height - Ox)) + Oy;
    }
    return y;
}

float CameraForGL::fellipse(float height, float width, float radius, unsigned char side, float x_height)
{
    if ((side != 'l')&&(side != 'r')) return -1;
    register float y;
    if ((x_height == 0)&&(side == 'l'))
    {
        y = width/2;
        return y;
    }
    if ((x_height == height)&&(side == 'r'))
    {
        y = width/2;
        return y;
    }
    register float Ox;
    if ((height - x_height) < radius)
        Ox = height - radius;
    else if (x_height < radius)
        Ox = radius;
    else
    {
        if (side == 'l')
            y = 0;
        else
            y = width;
        return y;
    }
    float Oy;
    if (side == 'l')
    {
        Oy = radius;
        y = -sqrtf(radius*radius - (x_height - Ox)*(x_height - Ox)) + Oy;
    }
    else
    {
        Oy = width - radius;
        y = sqrtf(radius*radius - (x_height - Ox)*(x_height - Ox)) + Oy;
    }
    if ((x_height == 0)&&(side == 'r'))
        y = width/2;
    if ((x_height == height)&&(side == 'l'))
        y = width/2;
    return y;
}

float CameraForGL::fellipseCoord(vect2f point1, vect2f point2, float radius, unsigned char side, float x_height)
{
    float height = fabsf(point2.x - point1.x);
    float width = fabsf(point2.y - point1.y);
    x_height = fabsf(x_height-point1.x);
    float y = fellipse(height,width,radius,side,x_height) + point1.y;
    if (point2.y < 0) y = -y;
    return y;
}
