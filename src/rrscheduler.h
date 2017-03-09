#ifndef RRSCHEDULER_H
#define RRSCHEDULER_H
#include "scheduler.h"
#include <queue>

const static int RR_PREEMPTION = 3;

// RRScheduler implements the round-robin scheduler described in the textbook
class RRScheduler : public Scheduler {

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