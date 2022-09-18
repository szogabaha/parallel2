#include "Flaky.hpp"

bool Flaky::busy = false;
std::thread::id Flaky::turn;

void Flaky::lock(void)
{
    auto me = std::this_thread::get_id();
    do
    {
        do
        {
            turn = me;
        } while (busy);
        busy = true;
    } while (turn != me);
}

void Flaky::unlock(void)
{
    busy = false;
}