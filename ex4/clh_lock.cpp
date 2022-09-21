#include "clh_lock.hpp"

#include <thread>

static thread_local qnode* my_node = nullptr;
static thread_local qnode* my_prev = nullptr;

CLHLock::CLHLock()
{
    qnode* new_node = new qnode;
    new_node->locked = false;
    new_node->my_pred = nullptr;
    this->tail.store(new_node);
}

void CLHLock::lock()
{
    my_node = new qnode;
    my_node->locked = true;
    qnode* predecessor = this->tail.exchange(my_node);
    my_prev = predecessor;
    while (predecessor->locked){};
}

void CLHLock::unlock()
{
    qnode* node = my_node;
    node->locked = false;
    my_node = my_prev;
}

CLHLock::~CLHLock()
{
}