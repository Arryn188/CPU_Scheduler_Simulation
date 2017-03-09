#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H
#include "scheduler.h"
#include <queue>

class FCFSScheduler : public Scheduler {

public:

	Decision* get_next_thread(const Event* event);

	void enqueue(const Event* event, Thread* thread);

private:

	std::queue<Thread*> threads;
};

#endif