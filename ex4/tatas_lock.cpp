#include "tatas_lock.hpp"

TTASLock::TTASLock() : state(false)
{
}

void TTASLock::lock()
{
    while (true)
    {
        while (state.load())
        {
        };
        if (!state.exchange(true))
            return;
    }
}

void TTASLock::unlock()
{
    state.store(false);
}