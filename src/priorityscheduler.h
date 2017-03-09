#ifndef PRIORITYSCHEDULER_H
#define PRIORITYSCHEDULER_H
#include "scheduler.h"
#include <queue>

// PriorityScheduler implements the round-robin scheduler described in the textbook
class PriorityScheduler : public Scheduler {

public:
	// Returns next thread (or null if none left) that should be executed
	Decision* get_next_thread(const Event* event);

	// Adds a thread to the scheduler's ready queue
	void enqueue(const Event* event, Thread* thread);

private:
	// Array of queues populated by arriving threads organized by priority
	std::queue<Thread*> threads[4] = {std::queue<Thread*>(), std::queue<Thread*>(), std::queue<Thread*>(), std::queue<Thread*>()};
};

#endif