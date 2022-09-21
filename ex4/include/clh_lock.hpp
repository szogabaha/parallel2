#ifndef CLH_HPP
#define CLH_HPP

#include <atomic>

struct qnode
{
    bool locked;
    qnode* my_pred;
};

class CLHLock
{
private:
    std::atomic<qnode*> tail;
public:
    CLHLock(/* args */);

    void lock();
    void unlock();
    ~CLHLock();
};


#endif