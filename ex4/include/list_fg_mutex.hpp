#ifndef LIST_FG_MUTEX_HPP
#define LIST_FG_MUTEX_HPP

#include "sorted_list.hpp"
#include <mutex>

/* struct for list nodes */
template<typename T>
class fine_node {
	private:
		std::mutex mutex;
	public:
		T value;
		fine_node<T>* next;

		void lock() {
			this->mutex.lock();
		}

		void unlock() {
			this->mutex.unlock();
		}
		virtual bool isblocking() {
			return false;
		}
};

template<typename T>
class end_node: public fine_node<T> {
	public:
		bool isblocking() {
			return true;
		}
};
/**
 * Sorted List with fine-grained mutex
 */

template <typename T>
class list_fg_mutex : public sorted_list<T> {
private:
	fine_node<T> *first;
	fine_node<T> *last;

public:
	list_fg_mutex(){
		first = new end_node<T>();
		last = new end_node<T>();
		first->next = last;
	}

	~list_fg_mutex() {
		delete first;
		delete last;
	}
	/* insert v into the list */
	void insert(T v) {
		/* first find position */
		first->lock();
		fine_node<T>* pred = first;
		fine_node<T>* succ = first->next;
		succ->lock();
		while(!succ->isblocking() && succ->value < v) {
			pred->unlock();
			pred = succ;
			succ = succ->next;
			succ->lock();
		}

		/* construct new fine_node */
		fine_node<T>* current = new fine_node<T>();
		current->value = v;

		/* insert new node between pred and succ */
		current->next = succ;
		pred->next = current;
		pred->unlock();
		succ->unlock();
	}


	void remove(T v) {
		/* first find position */
		first->lock();
		fine_node<T>* pred = first;
		fine_node<T>* succ = first->next;
		succ->lock();
		while(!succ->isblocking() && succ->value < v) {
			pred->unlock();
			pred = succ;
			succ = succ->next;
			succ->lock();
		}

		if (succ->isblocking()) {
			/* v not found */
			succ->unlock();
			pred->unlock();
			return;
		}
		/* remove current */
		pred->next = succ->next;
		succ->unlock();
		pred->unlock();
		delete succ;
	}

	/* count elements with value v in the list */
	std::size_t count(T v) {
		std::size_t cnt = 0;
		/* first find position */
		first->lock();
		fine_node<T>* pred = first;
		fine_node<T>* succ = first->next;
		succ->lock();
		while(!succ->isblocking() && succ->value < v) {
			pred->unlock();
			pred = succ;
			succ = succ->next;
			succ->lock();
		}

		/* count elements */
		while (!succ->isblocking() && succ->value == v) {
			cnt++;
			pred->unlock();
			succ->next->lock();
			pred = succ;
			succ = succ->next;
		}
		pred->unlock();
		succ->unlock();
		return cnt;
	}
};

#endif