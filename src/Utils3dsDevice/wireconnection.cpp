#include "wireconnection.h"

using namespace PinDeviceConstants;

WireConnection::WireConnection(QObject *parent) : QObject(parent)
{
    _tempDevice = PinOfDeviceNoInfo;
}

WireConnection::~WireConnection()
{
    clear();
}

void WireConnection::clear()
{
    _tempVect3f.clear();
    _tempDevice = PinOfDeviceNoInfo;

    _rheostat.clear();
    _ampermeter.clear();
    _voltmeter.clear();
    _battery.clear();
    _lamp.clear();
    _switch.clear();

}

bool WireConnection::isConnectedDevice(PinOfDevice dev1, PinOfDevice dev2) const
{
    int i, j;
    bool otv = false;
    PinOfDevice tempch;
    for (j=0; j<1; j++)
    {
        switch(dev1)
        {
        case PinBattery_Plus:
            for (i=0; i<_battery.plus.size(); i++)
            {
                if (_battery.plus.at(i) == dev2) otv = true;
            }
            break;

        case PinBattery_Minus:
            for (i=0; _battery.minus.size(); i++)
            {
                if (_battery.minus.at(i) == dev2) otv = true;
            }
            break;

        case PinSwitch_Left:
            for (i=0; _switch.minus.size(); i++)
            {
                if (_switch.minus.at(i) == dev2) otv = true;
            }
            break;

        case PinSwitch_Right:
            for (i=0; _switch.plus.size(); i++)
            {
                if (_switch.plus.at(i) == dev2) otv = true;
            }
            break;

        case PinRheostat_Left:
            for (i=0; _rheostat.minus.size(); i++)
            {
                if (_rheostat.minus.at(i) == dev2) otv = true;
            }
            break;

        case PinRheostat_Right:
            for (i=0; _rheostat.plus.size(); i++)
            {
                if (_rheostat.plus.at(i) == dev2) otv = true;
            }
            break;

        case PinAmpermeter_Plus:
            for (i=0; _ampermeter.plus.size(); i++)
            {
                if (_ampermeter.plus.at(i) == dev2) otv = true;
            }
            break;

        case PinAmpermeter_Minus:
            for (i=0; _ampermeter.minus.size(); i++)
            {
                if (_ampermeter.minus.at(i) == dev2) otv = true;
            }
            break;

        case PinVoltmeter_Plus:
            for (i=0; _voltmeter.plus.size(); i++)
            {
                if (_voltmeter.plus.at(i) == dev2) otv = true;
            }
            break;

        case PinVoltmeter_Minus:
            for (i=0; _voltmeter.minus.size(); i++)
            {
                if (_voltmeter.minus.at(i) == dev2) otv = true;
            }
            break;

        case PinLamp_Left:
            for (i=0; _lamp.minus.size(); i++)
            {
                if (_lamp.minus.at(i) == dev2) otv = true;
            }
            break;

        case PinLamp_Right:
            for (i=0; _lamp.plus.size(); i++)
            {
                if (_lamp.plus.at(i) == dev2) otv = true;
            }
            break;
        default:
            qDebug()<<"Неизвестные переменные девайсов";
            return false;
        }

        if ((j==1)&&(!otv))
        {
            qDebug()<<"Отшибка в соединении приборов при проверке"<<dev1<<" и "<<dev2;
        }
        if (!otv) return false;
        tempch = dev1;
        dev1 = dev2;
        dev2 = tempch;
    }
    return otv;
}

Device WireConnection::rheostat() const
{
    return _rheostat;
}

Device WireConnection::ampermeter() const
{
    return _ampermeter;
}

Device WireConnection::voltmeter() const
{
    return _voltmeter;
}

Device WireConnection::battery() const
{
    return _battery;
}

Device WireConnection::lamp() const
{
    return _lamp;
}

Device WireConnection::switcher() const
{
    return _switch;
}

void WireConnection::setLengthWire(PinOfDevice dev1, PinOfDevice dev2, float length)
{
    int j;
    PinOfDevice tempch;
    for (j=0; j<1; j++)
    {
        switch(dev1)
        {
        case PinBattery_Plus:
            _battery.lenghtLinePlus[dev2] = length;
            break;

        case PinBattery_Minus:
            _battery.lenghtLineMinus[dev2] = length;
            break;

        case PinSwitch_Left:
            _switch.lenghtLineMinus.insert(dev2, length);
            break;

        case PinSwitch_Right:
            _switch.lenghtLinePlus[dev2] = length;
            break;

        case PinRheostat_Left:
            _rheostat.lenghtLineMinus.insert(dev2, length);
            break;

        case PinRheostat_Right:
            _rheostat.lenghtLinePlus[dev2] = length;
            break;

        case PinAmpermeter_Plus:
            _ampermeter.lenghtLinePlus[dev2] = length;
            break;

        case PinAmpermeter_Minus:
            _ampermeter.lenghtLineMinus.insert(dev2, length);
            break;

        case PinVoltmeter_Plus:
            _voltmeter.lenghtLinePlus[dev2] = length;
            break;

        case PinVoltmeter_Minus:
            _voltmeter.lenghtLineMinus.insert(dev2, length);
            break;

        case PinLamp_Left:
            _lamp.lenghtLineMinus.insert(dev2, length);
            break;

        case PinLamp_Right:
            _lamp.lenghtLinePlus[dev2] = length;
            break;
        default:
            qDebug()<<"Неизвестные переменные девайсов";
            return;
        }

        tempch = dev1;
        dev1 = dev2;
        dev2 = tempch;
    }
}

vect3uch WireConnection::colorOfPinType(PinType type)
{
    switch(type)
    {
    case PinPlus: return vect3uch(255,0,0);
    case PinMinus: return vect3uch(0,0,255);
    default: return vect3uch();
    }
    return vect3uch();
}

void WireConnection::connectDeviceDoubleStep(PinOfDevice dev1, vect3f coordinatDevice)
{
    if (_tempDevice == PinOfDeviceNoInfo)
    {
        _tempDevice = dev1;
        _tempVect3f = coordinatDevice;
        return;
    }
    this->connectDevice(_tempDevice, dev1);

    emit connectedDeviceOfDoubleStep(_tempDevice, dev1);
    emit connectedDeviceOfDoubleStep(_tempVect3f, coordinatDevice, PinTypeNoInfo);

    this->clearBufDeviceDoubleStep();
}

void WireConnection::clearBufDeviceDoubleStep()
{
    _tempDevice = PinOfDeviceNoInfo;
    _tempVect3f.clear();
}

void WireConnection::connectDevice(PinOfDevice dev1, PinOfDevice dev2)
{
    QList<PinOfDevice> tempMas;
    PinOfDevice tempch;
    int i, j;
    for (j=0; j<1; j++)
    {
        switch(dev1)
        {
        case PinBattery_Plus:
            if (_battery.plus.isEmpty()) _battery.plus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_battery.plus.size(); i++)
                {
                    tempMas.append(_battery.plus.at(i));
                }
            }
            break;

        case PinBattery_Minus:
            if (_battery.minus.isEmpty()) _battery.minus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_battery.minus.size(); i++)
                {
                    tempMas.append(_battery.minus.at(i));
                }
            }
            break;

        case PinSwitch_Left:
            if (_switch.minus.isEmpty()) _switch.minus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_switch.minus.size(); i++)
                {
                    tempMas.append(_switch.minus.at(i));
                }
            }
            break;

        case PinSwitch_Right:
            if (_switch.plus.isEmpty()) _switch.plus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_switch.plus.size(); i++)
                {
                    tempMas.append(_switch.plus.at(i));
                }
            }
            break;

        case PinRheostat_Left:
            if (_rheostat.minus.isEmpty()) _rheostat.minus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_rheostat.minus.size(); i++)
                {
                    tempMas.append(_rheostat.minus.at(i));
                }
            }
            break;

        case PinRheostat_Right:
            if (_rheostat.plus.isEmpty()) _rheostat.plus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_rheostat.plus.size(); i++)
                {
                    tempMas.append(_rheostat.plus.at(i));
                }
            }
            break;

        case PinAmpermeter_Plus:
            if (_ampermeter.plus.isEmpty()) _ampermeter.plus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_ampermeter.plus.size(); i++)
                {
                    tempMas.append(_ampermeter.plus.at(i));
                }
            }
            break;

        case PinAmpermeter_Minus:
            if (_ampermeter.minus.isEmpty()) _ampermeter.minus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_ampermeter.minus.size(); i++)
                {
                    tempMas.append(_ampermeter.minus.at(i));
                }
            }
            break;

        case PinVoltmeter_Plus:
            if (_voltmeter.plus.isEmpty()) _voltmeter.plus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_voltmeter.plus.size(); i++)
                {
                    tempMas.append(_voltmeter.plus.at(i));
                }
            }
            break;

        case PinVoltmeter_Minus:
            if (_voltmeter.minus.isEmpty()) _voltmeter.minus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_voltmeter.minus.size(); i++)
                {
                    tempMas.append(_voltmeter.minus.at(i));
                }
            }
            break;

        case PinLamp_Left:
            if (_lamp.minus.isEmpty()) _lamp.minus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_lamp.minus.size(); i++)
                {
                    tempMas.append(_lamp.minus.at(i));
                }
            }
            break;

        case PinLamp_Right:
            if (_lamp.plus.isEmpty()) _lamp.plus.append(dev2);
            else
            {
                tempMas.append(dev1);
                tempMas.append(dev2);
                for (i=0; i<_lamp.plus.size(); i++)
                {
                    tempMas.append(_lamp.plus.at(i));
                }
            }
            break;
        default:
            qDebug()<<"Неизвестные переменные девайсов";
            return;
        }

        if (!tempMas.isEmpty())
        {
            connectDevice(tempMas);
        }

        tempch = dev1;
        dev1 = dev2;
        dev2 = tempch;
    }

    emit connectedDevice();
    emit connectedDevice(dev1, dev2);
}

void WireConnection::connectDevice(QList<PinOfDevice> mas)
{
    int i, j;
    for (i=0; i<mas.size(); i++)
    {
        for (j=1; j<mas.size(); j++)
        {
            if (i==j) continue;
            if (isConnectedDevice(mas.at(i), mas.at(j))) continue;
            connectDevice(mas.at(i), mas.at(j));
        }
    }
}

void WireConnection::disConnectDevice(PinOfDevice dev1, PinOfDevice dev2)
{
    int i, j;
    PinOfDevice tempch;
    for (j=0; j<1; j++)
    {
        switch(dev1)
        {
        case PinBattery_Plus:
            for (i=0; i<_battery.plus.size(); i++)
            {
                if (_battery.plus.at(i) == dev2) _battery.plus.removeAt(i);
            }
            break;

        case PinBattery_Minus:
            for (i=0; _battery.minus.size(); i++)
            {
                if (_battery.minus.at(i) == dev2) _battery.minus.removeAt(i);
            }
            break;

        case PinSwitch_Left:
            for (i=0; _switch.minus.size(); i++)
            {
                if (_switch.minus.at(i) == dev2) _switch.minus.removeAt(i);
            }
            break;

        case PinSwitch_Right:
            for (i=0; _switch.plus.size(); i++)
            {
                if (_switch.plus.at(i) == dev2) _switch.plus.removeAt(i);
            }
            break;

        case PinRheostat_Left:
            for (i=0; _rheostat.minus.size(); i++)
            {
                if (_rheostat.minus.at(i) == dev2) _rheostat.minus.removeAt(i);
            }
            break;

        case PinRheostat_Right:
            for (i=0; _rheostat.plus.size(); i++)
            {
                if (_rheostat.plus.at(i) == dev2) _rheostat.plus.removeAt(i);
            }
            break;

        case PinAmpermeter_Plus:
            for (i=0; _ampermeter.plus.size(); i++)
            {
                if (_ampermeter.plus.at(i) == dev2) _ampermeter.plus.removeAt(i);
            }
            break;

        case PinAmpermeter_Minus:
            for (i=0; _ampermeter.minus.size(); i++)
            {
                if (_ampermeter.minus.at(i) == dev2) _ampermeter.minus.removeAt(i);
            }
            break;

        case PinVoltmeter_Plus:
            for (i=0; _voltmeter.plus.size(); i++)
            {
                if (_voltmeter.plus.at(i) == dev2) _voltmeter.plus.removeAt(i);
            }
            break;

        case PinVoltmeter_Minus:
            for (i=0; _voltmeter.minus.size(); i++)
            {
                if (_voltmeter.minus.at(i) == dev2) _voltmeter.minus.removeAt(i);
            }
            break;

        case PinLamp_Left:
            for (i=0; _lamp.minus.size(); i++)
            {
                if (_lamp.minus.at(i) == dev2) _lamp.minus.removeAt(i);
            }
            break;

        case PinLamp_Right:
            for (i=0; _lamp.plus.size(); i++)
            {
                if (_lamp.plus.at(i) == dev2) _lamp.plus.removeAt(i);
            }
            break;
        default:
            qDebug()<<"Неизвестные переменные девайсов";
            return;
        }

        tempch = dev1;
        dev1 = dev2;
        dev2 = tempch;
    }

    emit disConnectedDevice(dev1, dev2);
}
