#include <Flaky.hpp>

#include <iostream>
#include <mutex>

#define THREAD_NUMBER 12

#ifdef STD
#define LOCK() mutex.lock()
#define UNLOCK() mutex.unlock();
#else
#define LOCK() Flaky::lock();
#define UNLOCK() Flaky::unlock();
#endif

static long long counter = 0;

std::mutex mutex;

bool run = true;

void inc()
{
    bool r = true;
    while (r)
    {
        LOCK();
        ++counter;
        r = run;
        UNLOCK();
    }
}

void dec()
{
    bool r = true;
    while (r)
    {
        LOCK();
        --counter;
        r = run;
        UNLOCK();
    }
}

void print()
{
    bool r = true;
    while (r)
    {
        LOCK();
        printf("%lld\n", counter);
        r = run;
        UNLOCK();
    }
}

int main()
{
    std::thread t1(inc);
    std::thread t2(dec);
    std::thread t3(print);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    LOCK();
    run = false;
    UNLOCK();

    t1.join();
    t2.join();
    t3.join();
}