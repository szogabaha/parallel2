#ifndef LIST_FG_MUTEX_HPP
#define LIST_FG_MUTEX_HPP

#include "sorted_list.hpp"
#include <mutex>
/**
 * Sorted List with fine-grained mutex
 */

#define DEBUG_PRINT 1
#undef DEBUG_PRINT

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
    std::mutex list_access;

public:
    list_fg_mutex() = default;

    /* insert v into the list */
    void insert(T v)
    {
        list_access.lock();
        if (first != nullptr)
        {
            // the list has at least one element
            // we let the threads spinning on that
            list_access.unlock();
        }
        else
        {
            // Keep the lock
            // we only want one thread to insert the first element
            // of the list
        }

        /* first find position */
        fine_node<T> *pred = nullptr;
        fine_node<T> *succ = first;
        if (first != nullptr)
            succ->lock();
        // We need to find the first node of the list, so if first is different from nullprt we get it
        while (succ != nullptr && succ->value < v)
        {
            if (pred != nullptr)
                pred->unlock();
            pred = succ;
            succ = succ->next;
            if (succ != nullptr)
                succ->lock();
        }
        /* We have two possible values of succ at the end of the loop
            1) succ == nullptr
                1.1) pred == nullptr -->
                    - `pred` does not have lock
                    - `succ` does not have lock
                    - the only lock to release is the `list_access` lock
                1.2) pred != nullptr -->
                    - we insert an element at the end of the list (preinsert)
                    - we still have the lock of `pred`
            2) succ != nullptr
                2.1) pred == nullptr -->
                    - only one element in the list or preinsert in the list
                    - `pred` does not have a lock
                    - `succ` must release the lock after the op
                2.2) pred != nullptr -->
                    - we are in the middle of the list
                    - `pred` has a lock
                    - `succ` has a lock
        */

        /* construct new fine_node */
        fine_node<T> *current = new fine_node<T>();
        current->value = v;

        /* insert new fine_node between pred and succ */
        current->next = succ;
#ifdef DEBUG_PRINT
        if (succ == nullptr)
        {
            if (pred == nullptr)
            {
                printf("succ && pred NULL\n");
                first = current;
                list_access.unlock();
            }
            else
            {
                printf("succ NULL\n");
                pred->next = current;
                pred->unlock();
            }
        }
        else
        {
            if (pred == nullptr)
            {
                printf("pred NULL\n");
                first = current;
                succ->unlock();
            }
            else
            {
                printf("succ && pred not null\n");
                pred->next = current;
                succ->unlock();
                pred->unlock();
            }
        }
#else
        if (succ == nullptr)
        {
            if (pred == nullptr)
            {
                first = current;
                list_access.unlock();
            }
            else
            {
                pred->next = current;
                pred->unlock();
            }
        }
        else
        {
            if (pred == nullptr)
            {
                first = current;
                succ->unlock();
            }
            else
            {
                pred->next = current;
                succ->unlock();
                pred->unlock();
            }
        }
#endif
    }

    void remove(T v)
    {                       // TODO add mutex
        list_access.lock(); // FIXME make it fine-grained
        if (first == nullptr)
        {
            // there are no elements in the list
            list_access.unlock();
            return;
        }
        else
        {
            list_access.unlock();
        }
        /* first find position */
        fine_node<T> *pred = nullptr;
        fine_node<T> *current = first;
        if (first != nullptr)
            current->lock();
        while (current != nullptr && current->value < v)
        {
            if (pred != nullptr)
                pred->unlock();
            pred = current;
            current = current->next;
            if (current != nullptr)
                current->lock();
        }
        /* We have two possible values of current at the end of the loop
            1) current == nullptr
                1.1) pred == nullptr -->
                    - not possible, we've already checked if the list is not empty
                1.2) pred != nullptr -->
                    - we want to delete an element at the end of the list
                    - we still have the lock of `pred`
            2) current != nullptr
                2.1) pred == nullptr -->
                    - we wanto to delete the first element of the list
                    - `pred` does not have a lock
                    - `current` must release the lock after the op
                2.2) pred != nullptr -->
                    - we are in the middle of the list
                    - `pred` has a lock
                    - `current` has a lock
        */
        if (current == nullptr || current->value != v)
        {
            if (current != nullptr) // since there is an OR operator
            {
                // i.e we want to delete 0 in the list [1, 2, 3]
                // we locked current, 0 < 1 but current=first != nullptr
                // since the element doesn't exist, we release the lock
                current->unlock();
                if(pred != nullptr)
                    pred->unlock();
            }
            else
            {
                // i.e we want to delete 11 in the list [1, 2, 3]
                // after the loop, current = nullptr but the fact current->value < 11 still hold
                // threfore current->value != and since current==nullptr is not locked
                // but in the loop we locked the predecessor, so we unlock it and return
                pred->unlock();
            }
            return;
        }
        /* remove current */
        if (pred == nullptr)
        {
            // the value correspond to the first element of the queue
            // current has been assigned to first, and then locked
            first = current->next;
            current->unlock();
            delete current;
            return;
        }
        else
        {
            // we are in the the list, with current and pred not null
            pred->next = current->next;
            current->unlock();
            pred->unlock();
            delete current;
            return;
        }
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

    void visit(void)
    {
        fine_node<T> *current = first;
        while (current != nullptr)
        {
            printf("-%d", current->value);
            current = current->next;
        }
        printf("\n");
    }
};

#endif