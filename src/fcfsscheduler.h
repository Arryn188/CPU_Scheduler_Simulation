#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H
#include "scheduler.h"
#include <queue>

// FCFSScheduler implements the first-come-first-served scheduler described in the textbook
class FCFSScheduler : public Scheduler {

public:
	// Returns next thread (or null if none left) that should be executed
	Decision* get_next_thread(const Event* event);

	// Adds a thread to the scheduler's ready queue
	void enqueue(const Event* event, Thread* thread);

private:
	// Queue populated by arriving threads, scheduled threads drawn from the front
	std::queue<Thread*> threads;
};

#endif