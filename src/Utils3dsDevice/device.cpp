#include "device.h"

Device::Device()
{
    resistance = 0;
}

Device::~Device()
{
    clear();
}

bool Device::isEmptyData() const
{
    if (resistance != 0) return false;
    if (!lenghtLineMinus.isEmpty()) return false;
    if (!lenghtLinePlus.isEmpty()) return false;
    if (!minus.isEmpty()) return false;
    if (!plus.isEmpty()) return false;
    return true;
}

bool Device::isEmpty() const
{
    if (!isEmptyData()) return false;
    if (!name.isEmpty()) return false;
    return true;
}

void Device::replace(const Device d, ReplaceType type)
{
    switch(type)
    {
    case ReplacePinPlusAndMinus:
        this->plus = d.plus;
        this->minus = d.minus;
        break;
    case ReplaceLenghtLineAll:
        this->lenghtLineMinus = d.lenghtLineMinus;
        this->lenghtLinePlus = d.lenghtLinePlus;
        break;
    case ReplaceAllPinsAndLenght:
        this->plus = d.plus;
        this->minus = d.minus;
        this->lenghtLineMinus = d.lenghtLineMinus;
        this->lenghtLinePlus = d.lenghtLinePlus;
        break;
    }
}

void Device::clear()
{
    resistance = 0;
    lenghtLineMinus.clear();
    lenghtLinePlus.clear();
    minus.clear();
    plus.clear();
    name.clear();
}
