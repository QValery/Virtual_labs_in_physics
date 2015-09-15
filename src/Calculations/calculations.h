#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QObject>
#include <QDebug>
#include "Utils3dsDevice/Constants.h"
#include "Utils3dsDevice/pindevicecostants.h"
#include "physicsfunction.h"
#include "Utils3dsDevice/wireconnection.h"

#define COUNT_OF_ITERRATION 400

class Calculations: public QObject
{
    Q_OBJECT
public:

    enum ErrorType
    {
        ErrorNo                         = 0,
        ErrorLackDevices                = 1,  //Нехватка переменных. Не все приборы соединены
        ErrorConnectionMutual           = 2,  //Ошибка в соединении приборов. Соединение с самим собой
        ErrorConnectionRuleOfPhysics    = 3,  // Нарушение правил физики
        ErrorNoInforceVarintType        = 4
//        #define ERROR_LOOPING           19  //Зацикливание
    };

    enum VariantType
    {
        VariantNoInfo     = 0,
        VariantVoltage    = 1,
        VariantAmperage   = 2,
        VariantResistance = 3
    };

    explicit Calculations(QObject *ptr = 0);
    Calculations(WireConnection *w, QObject *ptr = 0);

    Calculations::ErrorType errorType() const;
    Calculations::VariantType variant() const;

    bool calculation(Calculations::VariantType var);

signals:

    void lampOn(float on);

    void voltage(float v);
    void amperage(float a);

    /// Возвращает аргумент
    Calculations::ErrorType error(Calculations::ErrorType t);

public slots:

    void clear();

    void setWiresConnection(WireConnection *w);

    void setResistanceOfRheostat(float r);

    void setResistanceOfAmpermeter(float r);

    void setResistanceOfVoltmeter(float r);

    void setEDSBattery(float e);

    void setResistanceOfLamp(float r);

    void setResistanceOfSwitch(float r);

    void calculation();

private slots:

    void connectDevice();

private:

    WireConnection *_wireSettings = NULL;

    Device _rheostat;
    Device _ampermeter;
    Device _voltmeter;
    Device _battery;
    Device _lamp;
    Device _switch;

    float _EDSBattery;
    bool _thisVoltmeter;
    float _lampOn;
    float _amperage;
    float _voltage;

    ErrorType _errorType;
    VariantType _variantType;

    void getInfoFromWireConnectionClass();

    Calculations::ErrorType setErrorType(ErrorType t);

    Calculations::ErrorType scanningForErrors();

    bool scanningForErrorsOfVariant(Calculations::VariantType variant);

    void calculationB();


    /// Возвращает первый попавшийся контакт, кроме exc
    static PinDeviceConstants::PinOfDevice findPinException(QList<PinDeviceConstants::PinOfDevice> mas, PinDeviceConstants::DeviceType exc);

    static void pinOfDevice(PinDeviceConstants::DeviceType dev, PinDeviceConstants::PinOfDevice *minus, PinDeviceConstants::PinOfDevice *plus);

    bool thisVoltmeter(QList<PinDeviceConstants::PinOfDevice> mas);
//    bool thisDevice(QList<unsigned char> mas, unsigned char device);

    /// Вычисляет яркость лампы.
    /// @param Напряжение в лампе
    float lampBrightness(float v);
};

#endif // CALCULATIONS_H
