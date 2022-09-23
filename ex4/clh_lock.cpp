#include "clh_lock.hpp"

#include <thread>

/**
 * @brief This is a tentative to translate the Java example of the book
 * in C++. The comments next to the variables will indicate which part of
 * the Java code has been translated
 * 
 * - thread_local resources:
 * - - https://en.cppreference.com/w/cpp/language/storage_duration
 * - - https://stackoverflow.com/questions/11983875/what-does-the-thread-local-mean-in-c11
 * 
 */

/*
myNode = new ThreadLocal<QNode>() {
    protected QNode initialValue() {
        return new QNode();
    }
};
*/
thread_local qnode* my_node = new qnode;

/*
myPred = new ThreadLocal<QNode>() {
    protected QNode initialValue() {
        return null;
    }
};
*/
thread_local qnode* my_prev = nullptr;

CLHLock::CLHLock()
{
    /* tail = new AtomicReference<QNode>(new QNode()); */
    qnode* new_node = new qnode;
    new_node->locked = false;
    this->tail.store(new_node);
}

void CLHLock::lock()
{
    my_node->locked.store(true); // QNode qnode = myNode.get(); qnode.locked = true;
    qnode* predecessor = this->tail.exchange(my_node); // QNode pred = tail.getAndSet(qnode);
    my_prev = predecessor; // myPred.set(pred);
    while (predecessor->locked){};
}

void CLHLock::unlock()
{
    my_node->locked.store(false); // QNode qnode = myNode.get(); qnode.locked = false;
    my_node = my_prev; // myNode.set(myPred.get());
}

CLHLock::~CLHLock()
{
    delete tail;
}