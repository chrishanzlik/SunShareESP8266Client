#include <vector>
#ifndef LOOP_MANAGER_H
#define LOOP_MANAGER_H

struct CallbackRegistration
{
    unsigned int interval;
    unsigned long last_exec;
    void (*cb)();
};

class LoopManager
{
public:
    LoopManager();
    void register_handler(void (*cb)(), unsigned int interval);
    void process();

private:
    std::vector<CallbackRegistration> registrations = {};
};

#endif