#ifndef PINDEVICECOSTANTS
#define PINDEVICECOSTANTS

namespace PinDeviceConstants
{

enum PinType
{
    PinTypeNoInfo = 0,
    PinMinus = 1,
    PinPlus = 2
};

enum PinOfDevice
{
    PinOfDeviceNoInfo   = 0,
    PinBattery_Plus     = 1,
    PinBattery_Minus    = 2,
    PinSwitch_Left      = 3,
    PinSwitch_Right     = 4,
    PinRheostat_Left    = 5,
    PinRheostat_Right   = 6,
    PinAmpermeter_Plus  = 7,
    PinAmpermeter_Minus = 8,
    PinVoltmeter_Plus   = 9,
    PinVoltmeter_Minus  = 10,
    PinLamp_Left        = 11,
    PinLamp_Right       = 12
};

enum DeviceType
{
    DeviceBattery      = 1,
    DeviceSwitch       = 2,
    DeviceRheostat     = 3,
    DeviceAmpermeter   = 4,
    DeviceVoltmeter    = 5,
    DeviceLamp         = 6
};

}

#endif // PINDEVICECOSTANTS

