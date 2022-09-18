#ifndef FLAKY_HPP
#define FLAKY_HPP

#include <thread>

class Flaky
{
public:
    static void lock(void);
    static void unlock(void);

private:
    static std::thread::id turn;
    static bool busy;
};


#endif