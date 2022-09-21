#ifndef LIST_CG_TATAS_HPP
#define LIST_CG_TATAS_HPP

#include "sorted_list.hpp"
#include "tatas_lock.hpp"
/**
 * Sorted List with coarse-grained mutex
 */

template <typename T>
class list_cg_tatas : public sorted_list<T>
{
private:
    node<T> *first = nullptr;
    TTASLock mutex;

public:
    list_cg_tatas() = default;

    /* insert v into the list */
    void insert(T v)
    { // TODO add mutex
        mutex.lock();
        /* first find position */
        node<T> *pred = nullptr;
        node<T> *succ = first;
        while (succ != nullptr && succ->value < v)
        {
            pred = succ;
            succ = succ->next;
        }

        /* construct new node */
        node<T> *current = new node<T>();
        current->value = v;

        /* insert new node between pred and succ */
        current->next = succ;
        if (pred == nullptr)
        {
            first = current;
        }
        else
        {
            pred->next = current;
        }
        mutex.unlock();
    }

    void remove(T v)
    { // TODO add mutex
        mutex.lock();
        /* first find position */
        node<T> *pred = nullptr;
        node<T> *current = first;
        while (current != nullptr && current->value < v)
        {
            pred = current;
            current = current->next;
        }
        if (current == nullptr || current->value != v)
        {
            /* v not found */
            mutex.unlock();
            return;
        }
        /* remove current */
        if (pred == nullptr)
        {
            first = current->next;
        }
        else
        {
            pred->next = current->next;
        }
        delete current;
        mutex.unlock();
    }

    /* count elements with value v in the list */
    std::size_t count(T v)
    { // TODO add mutexes
        mutex.lock();
        std::size_t cnt = 0;
        /* first go to value v */
        node<T> *current = first;
        while (current != nullptr && current->value < v)
        {
            current = current->next;
        }
        /* count elements */
        while (current != nullptr && current->value == v)
        {
            cnt++;
            current = current->next;
        }
        mutex.unlock();
        return cnt;
    }
};

#endif