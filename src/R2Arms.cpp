#include "R2Arms.hpp"


Pusher::Pusher(std::function<void(int)> f):
    pwmOut(PA_11),
    dirOut(PA_12),
    limit_switch(PB_12)
{
    wait = f;
}

