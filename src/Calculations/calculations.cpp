#include "calculations.h"

using namespace PinDeviceConstants;

Calculations::Calculations(QObject *ptr): QObject(ptr)
{
    clear();
}

Calculations::Calculations(WireConnection *w, QObject *ptr): QObject(ptr)
{
    clear();
    setWiresConnection(w);
}

Calculations::ErrorType Calculations::errorType() const
{
    return _errorType;
}

Calculations::VariantType Calculations::variant() const
{
    return _variantType;
}

void Calculations::clear()
{
    _wireSettings = NULL;
    _EDSBattery = EDS_BATTERY;
    _thisVoltmeter = false;
    _lampOn = 0;
    _amperage = 0;
    _voltage = 0;
    _errorType = ErrorNo;
    _variantType = VariantNoInfo;

    if (!_rheostat.isEmpty()) _rheostat.clear();
    if (!_ampermeter.isEmpty()) _ampermeter.clear();
    if (!_voltmeter.isEmpty()) _voltmeter.clear();
    if (!_battery.isEmpty()) _battery.clear();
    if (!_lamp.isEmpty()) _lamp.clear();
    if (!_switch.isEmpty()) _switch.clear();
}

void Calculations::setWiresConnection(WireConnection *w)
{
    if (_wireSettings != NULL)
    {
        disconnect(_wireSettings,SIGNAL(connectedDevice()),this,SLOT(connectDevice()));
    }
    _wireSettings = w;
    if (_wireSettings != NULL)
    {
        connect(_wireSettings,SIGNAL(connectedDevice()),this,SLOT(connectDevice()));
    }
}

void Calculations::connectDevice()
{
    if (_wireSettings == NULL) return;
    getInfoFromWireConnectionClass();

    calculation();
}

void Calculations::getInfoFromWireConnectionClass()
{
    _rheostat.replace(_wireSettings->rheostat(),Device::ReplaceAllPinsAndLenght);
    _ampermeter.replace(_wireSettings->ampermeter(),Device::ReplaceAllPinsAndLenght);
    _voltmeter.replace(_wireSettings->voltmeter(),Device::ReplaceAllPinsAndLenght);
    _battery.replace(_wireSettings->battery(),Device::ReplaceAllPinsAndLenght);
    _lamp.replace(_wireSettings->lamp(),Device::ReplaceAllPinsAndLenght);
    _switch.replace(_wireSettings->switcher(),Device::ReplaceAllPinsAndLenght);
}

Calculations::ErrorType Calculations::setErrorType(ErrorType t)
{
    if (t == ErrorNo)
    {
        _errorType = ErrorNo;
        return t;
    }

    _errorType = t;
    emit error(t);
    return t;
}

void Calculations::setResistanceOfRheostat(float r)
{
    if (r>=0) _rheostat.resistance = r;
}

void Calculations::setResistanceOfAmpermeter(float r)
{
    if (r>=0) _ampermeter.resistance = r;
}

void Calculations::setResistanceOfVoltmeter(float r)
{
    if (r>=0) _voltmeter.resistance = r;
}

void Calculations::setEDSBattery(float e)
{
    if (e>=0) _EDSBattery = e;
}

void Calculations::setResistanceOfLamp(float r)
{
    if (r>=0) _lamp.resistance = r;
}

void Calculations::setResistanceOfSwitch(float r)
{
    if (r>=0) _switch.resistance = r;
}


void Calculations::calculation()
{
    if (setErrorType(scanningForErrors()) == ErrorNo) calculationB();
}

bool Calculations::calculation(Calculations::VariantType variant)
{
    _variantType = variant;
    if (!scanningForErrorsOfVariant(variant))
    {
        setErrorType(ErrorNoInforceVarintType);
        return false;
    }
    _errorType = ErrorNo;
    calculation();
    return true;
}

Calculations::ErrorType Calculations::scanningForErrors()
{
    if (_battery.minus.isEmpty()) return ErrorLackDevices;
    if (_battery.plus.isEmpty()) return ErrorLackDevices;

    int i;
    for (i=0; i<_battery.minus.size(); i++)
    {
        if (_battery.minus.at(i) == PinBattery_Plus) return ErrorConnectionMutual;
        if (_battery.minus.at(i) == PinAmpermeter_Plus) return ErrorConnectionRuleOfPhysics;
        if (_battery.minus.at(i) == PinVoltmeter_Plus) return ErrorConnectionRuleOfPhysics;
    }
    for (i=0; i<_battery.plus.size(); i++)
    {
        if (_battery.plus.at(i) == PinBattery_Minus) return ErrorConnectionMutual;
        if (_battery.plus.at(i) == PinAmpermeter_Minus) return ErrorConnectionRuleOfPhysics;
        if (_battery.plus.at(i) == PinVoltmeter_Minus) return ErrorConnectionRuleOfPhysics;
    }
    return ErrorNo;
}

bool Calculations::scanningForErrorsOfVariant(Calculations::VariantType var)
{
    switch(var)
    {
    case VariantVoltage:
        if (_voltmeter.plus.isEmpty()) return false;
        if (_voltmeter.minus.isEmpty()) return false;
        break;

    case VariantAmperage:
        if (_ampermeter.plus.isEmpty()) return false;
        if (_ampermeter.minus.isEmpty()) return false;
        break;

    case VariantResistance:
        if (_rheostat.minus.isEmpty()) return false;
        if (_rheostat.plus.isEmpty()) return false;
        break;
    default: break;
    }
    return true;
}

void Calculations::calculationB()
{
    PinOfDevice tempNameDevice;
//    Device tempDevice;
    int i;
    bool amperageTemp, lampBool = false;
    float Om = 0, OmVolt = 0;
    QList<PinOfDevice> tempMas = _battery.plus;
    tempNameDevice = findPinException(tempMas, DeviceVoltmeter);
    for (i=0; tempNameDevice != PinBattery_Minus; i++)
    {
        amperageTemp = thisVoltmeter(tempMas);
        switch(tempNameDevice)
        {
        case PinSwitch_Left:
            tempMas = _switch.plus;
            Om+=_switch.resistance;
            if (amperageTemp) OmVolt+=_switch.resistance;
            break;

        case PinSwitch_Right:
            tempMas = _switch.minus;
            Om+=_switch.resistance;
            if (amperageTemp) OmVolt+=_switch.resistance;
            break;

        case PinRheostat_Left:
            tempMas = _rheostat.plus;
            Om+=_rheostat.resistance;
            if (amperageTemp) OmVolt+=_rheostat.resistance;
            break;

        case PinRheostat_Right:
            tempMas = _rheostat.minus;
            Om+=_rheostat.resistance;
            if (amperageTemp) OmVolt+=_rheostat.resistance;
            break;

        case PinAmpermeter_Plus:
            tempMas = _ampermeter.minus;
            Om+=_ampermeter.resistance;
            if (amperageTemp) OmVolt+=_ampermeter.resistance;
            break;

        case PinAmpermeter_Minus:
            tempMas = _ampermeter.plus;
            Om+=_ampermeter.resistance;
            if (amperageTemp) OmVolt+=_ampermeter.resistance;
            break;

        case PinLamp_Left:
            tempMas = _lamp.plus;
            Om+=_lamp.resistance;
            if (amperageTemp) OmVolt+=_lamp.resistance;
            lampBool = true;
            _lampOn = lampBrightness(PhysicsFunction::voltage(PhysicsFunction::amperage(_EDSBattery, Om), _lamp.resistance));
            break;

        case PinLamp_Right:
            tempMas = _lamp.minus;
            Om+=_lamp.resistance;
            if (amperageTemp) OmVolt+=_lamp.resistance;
            lampBool = true;
            _lampOn = lampBrightness(PhysicsFunction::voltage(PhysicsFunction::amperage(_EDSBattery, Om), _lamp.resistance));
            break;

        default:
            lampBool = false;
            i = COUNT_OF_ITERRATION;
            break;
        }

        tempNameDevice = findPinException(tempMas, DeviceVoltmeter);
        if (tempNameDevice == PinOfDeviceNoInfo)
        {
            setErrorType(ErrorLackDevices);
            return;
        }

        if (lampBool) _lampOn = true; else _lampOn = false;

        if (i > COUNT_OF_ITERRATION)
        {
            _lampOn = 0;
            return;
        }
    }

    if (scanningForErrorsOfVariant(VariantAmperage)) _amperage = PhysicsFunction::amperage(_EDSBattery, Om);
    if (scanningForErrorsOfVariant(VariantVoltage)) _voltage = PhysicsFunction::voltage(_amperage, OmVolt);

    if (!lampBool) _lampOn = 0;
}

PinOfDevice Calculations::findPinException(QList<PinOfDevice> mas, DeviceType exc)
{
    if (mas.isEmpty()) return PinOfDeviceNoInfo;
    PinOfDevice pin1, pin2;
    pinOfDevice(exc, &pin1, &pin2);
    for (int i=0; i<mas.size(); i++)
    {
        if ((mas.at(i) != pin1)&&(mas.at(i) != pin2)) return mas.at(i);
    }
    return PinOfDeviceNoInfo;
}

void Calculations::pinOfDevice(DeviceType dev, PinOfDevice *minus, PinOfDevice *plus)
{
    switch(dev)
    {
    case DeviceAmpermeter:
        *minus = PinAmpermeter_Minus;
        *plus = PinAmpermeter_Plus;
        break;
    case DeviceBattery:
        *minus = PinBattery_Minus;
        *plus = PinBattery_Plus;
        break;
    case DeviceLamp:
        *minus = PinLamp_Left;
        *plus = PinLamp_Right;
        break;
    case DeviceRheostat:
        *minus = PinRheostat_Left;
        *plus = PinRheostat_Right;
        break;
    case DeviceSwitch:
        *minus = PinSwitch_Left;
        *plus = PinSwitch_Right;
        break;
    case DeviceVoltmeter:
        *minus = PinVoltmeter_Minus;
        *plus = PinVoltmeter_Plus;
        break;
    default:
        minus = NULL;
        plus = NULL;
        break;
    }
}

bool Calculations::thisVoltmeter(QList<PinOfDevice> mas)
{
    int i;
    if (_thisVoltmeter)
    {
        for (i=0; i<mas.size(); i++)
        {
            if (mas.at(i) == PinVoltmeter_Minus)
            {
                _thisVoltmeter = false;
            }
        }
    }
    else
    {
        for (i=0; i<mas.size(); i++)
        {
            if (mas.at(i) == PinVoltmeter_Plus)
            {
                _thisVoltmeter = true;
            }
        }
    }
    return _thisVoltmeter;
}

float Calculations::lampBrightness(float v)
{
    return 1;
}
