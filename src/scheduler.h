#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "event.h"

// Base Class for all Scheduling Algorithms
class Scheduler {

public:
	virtual Decision* get_next_thread(const Event* event) = 0;
	virtual void enqueue(const Event* event, Thread* thread) = 0;
};

#endif
