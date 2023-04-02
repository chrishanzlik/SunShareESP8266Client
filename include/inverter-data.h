#ifndef INVERTER_DATA_H
#define INVERTER_DATA_H

#include <Arduino.h>

struct InverterData
{
    int battery;
    double batteryLoad;
    double load;
    double grid;
    double yield;
    String timestamp;
};

#endif