#ifndef INVERTER_DATA_PROVIDER_H
#define INVERTER_DATA_PROVIDER_H

#include "inverter-data.h"
#include <Arduino.h>

class InverterDataProvider
{
public:
    InverterDataProvider(String endpointUrl);
    struct Response
    {
        InverterData data;
        String message;
        bool message_only;
        bool is_high_gain;
    };
    Response provide();

private:
    String url;
};

#endif