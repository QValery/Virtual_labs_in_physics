#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QMap>
#include <QString>

#include "pindevicecostants.h"

class Device
{
public:

    enum ReplaceType
    {
        ReplacePinPlusAndMinus = 1,
        ReplaceLenghtLineAll = 2,
        ReplaceAllPinsAndLenght = 3,
    };

    Device();
    ~Device();
    void clear();

    /// true, если пусто
    /// Поле name не учитывается
    bool isEmptyData() const;
    bool isEmpty() const;

    void replace(const Device d, ReplaceType type);

    QString name;
    QList<PinDeviceConstants::PinOfDevice> plus;
    QMap<PinDeviceConstants::PinOfDevice, float> lenghtLinePlus;

    QList<PinDeviceConstants::PinOfDevice> minus;
    QMap<PinDeviceConstants::PinOfDevice, float> lenghtLineMinus;
    float resistance;
};

#endif // DEVICE_H
