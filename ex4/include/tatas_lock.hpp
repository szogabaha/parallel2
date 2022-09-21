#ifndef TTALOCK
#define TTALOCK
#include <atomic>

class TTASLock {
    private:
        std::atomic<bool> state;
    public:
        void lock();
        void unlock();
        TTASLock();
};




#endif