//
// Created by remay on 3/9/17.
//
#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H

#include "scheduler.h"
#include <queue>

using namespace std;

class FCFSScheduler : public Scheduler {

public:
	Decision* get_next_thread(const Event* event);
	void enqueue(const Event* event, Thread* thread);

private:
	queue<Thread*> threads;
};

#endif