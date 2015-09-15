#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QList>
#include <QMap>

//#define DEFAULT_VARIABLE    0
#define BATTERY_PLUS        1
#define BATTERY_MINUS       2
#define SWITCH_LEFT         3
#define SWITCH_RIGHT        4
#define RHEOSTAT_LEFT       5
#define RHEOSTAT_RIGHT      6
#define AMPERMETER_PLUS     7
#define AMPERMETER_MINUS    8
#define VOLTMETER_PLUS      9
#define VOLTMETER_MINUS     10
#define LAMP_LEFT           11
#define LAMP_RIGHT          12

#define VOLTAGE     13
#define AMPERAGE    14
#define RESISTANCE  15

#define ERROR_OVER_RANGE        16  //Посторонние переменные, перегруженные переменные
#define ERROR_LACK_VARIABLES    17  //Нехватка переменных. Не все приборы соединены
#define ERROR_CONNECTION        18  //Ошибка в соединении приборов
#define ERROR_LOOPING           19  //Зацикливание
#define ERROR_NO                20

#define RESISTANCE_OF_LAMP          3
#define RESISTANCE_OF_AMPERMETER    0.27
#define RESISTANCE_OF_VOLTMETER     10
#define RESISTANCE_OF_SWITCH        0.009
#define EDS_BATTERY                 12


#include "device.h"


#endif // CONSTANTS_H
