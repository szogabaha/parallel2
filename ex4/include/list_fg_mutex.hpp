#ifndef LIST_FG_MUTEX_HPP
#define LIST_FG_MUTEX_HPP

#include "sorted_list.hpp"
#include <mutex>
/**
 * Sorted List with fine-grained mutex
 */

template <typename T>
class fine_node
{
public:
    T value;
    fine_node<T> *next;
    inline void lock()
    {
        this->mtx.lock();
    }
    inline void unlock()
    {
        this->mtx.unlock();
    }

private:
    std::mutex mtx;
};

template <typename T>
class list_fg_mutex : public sorted_list<T>
{
private:
    fine_node<T> *first = nullptr;

public:
    list_fg_mutex() = default;

    /* insert v into the list */
    void insert(T v)
    { // TODO add mutex
        /* first find position */
        fine_node<T> *pred = nullptr;
        fine_node<T> *succ = first;
        // We need to find the first node of the list, so if first is different from nullprt we get it
        while (succ != nullptr && succ->value < v)
        {

            pred = succ;
            succ = succ->next;
        }

        /* construct new fine_node */
        fine_node<T> *current = new fine_node<T>();
        current->value = v;

        /* insert new fine_node between pred and succ */
        current->next = succ;
        if (pred == nullptr)
        {
            first = current;
        }
        else
        {
            pred->next = current;
        }
    }

    void remove(T v)
    { // TODO add mutex
        /* first find position */
        fine_node<T> *pred = nullptr;
        fine_node<T> *current = first;
        current->lock();
        while (current != nullptr && current->value < v)
        {
            pred = current;
            current = current->next;
            pred->unlock();
            current->lock();
        }
        if (current == nullptr || current->value != v)
        {
            current->unlock();
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
        current->unlock();
        delete current;
    }

    /* count elements with value v in the list */
    std::size_t count(T v)
    { // TODO add mutexes
        std::size_t cnt = 0;
        /* first go to value v */
        fine_node<T> *current = first;
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
        return cnt;
    }
};

#endif