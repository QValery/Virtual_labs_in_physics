#ifndef WIRECONNECTION_H
#define WIRECONNECTION_H

#include <QObject>
#include "lib3dstypes.h"
#include "pindevicecostants.h"
#include "device.h"

#include <QDebug>

class WireConnection : public QObject
{
    Q_OBJECT
public:
    explicit WireConnection(QObject *parent = 0);
    ~WireConnection();
    void clear();

    bool isConnectedDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2) const;

    Device rheostat() const;
    Device ampermeter() const;
    Device voltmeter() const;
    Device battery() const;
    Device lamp() const;
    Device switcher() const;

    void setLengthWire(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2, float length);


    static vect3uch colorOfPinType(PinDeviceConstants::PinType type);

signals:

    void connectedDeviceOfDoubleStep(vect3f pin1, vect3f pin2, PinDeviceConstants::PinType type);
    void connectedDeviceOfDoubleStep(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2);

    void connectedDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2);
    void connectedDevice();
    void disConnectedDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2);

public slots:

    /// Соединяет 2 контакта в 2 шага по одному на каждом шаге:
    /// первый храниться во временном буфере.
    void connectDeviceDoubleStep(PinDeviceConstants::PinOfDevice dev1, vect3f coordinatDevice = vect3f());
    void clearBufDeviceDoubleStep();

    void connectDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2);
    void disConnectDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2);

private:

    vect3f _tempVect3f;
    PinDeviceConstants::PinOfDevice _tempDevice;

    Device _rheostat;
    Device _ampermeter;
    Device _voltmeter;
    Device _battery;
    Device _lamp;
    Device _switch;

    void connectDevice(QList<PinDeviceConstants::PinOfDevice> mas);
};

#endif // WIRECONNECTION_H
