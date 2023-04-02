#include <Arduino.h>
#include <vector>
#include "loop-manager.h"

LoopManager::LoopManager()
{
}

void LoopManager::register_handler(void (*cb)(), unsigned int interval)
{
    CallbackRegistration reg;
    reg.cb = cb;
    reg.interval = interval;
    reg.last_exec = 0;

    registrations.push_back(reg);
}

void LoopManager::process()
{
    unsigned long currentMillis = millis();
    std::vector<CallbackRegistration>::iterator iter;

    for (iter = registrations.begin(); iter != registrations.end(); ++iter)
    {
        // aware overflow
        if (currentMillis < (*iter).last_exec)
        {
            (*iter).last_exec = 0;
        }

        if (currentMillis - (*iter).last_exec >= (*iter).interval)
        {
            (*iter).cb();

            (*iter).last_exec = millis();
        }
    }
}