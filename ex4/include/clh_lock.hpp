#ifndef CLH_HPP
#define CLH_HPP

#include <atomic>

struct qnode
{
    qnode() : locked(true) {}
    std::atomic_bool locked; // ThreadSanitizers detected a data race -> solution: make atomic
};

class CLHLock
{
private:
    std::atomic<qnode*> tail;
public:
    CLHLock();

    void lock();
    void unlock();
    
    ~CLHLock();
};


#endif