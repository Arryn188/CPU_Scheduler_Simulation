//
// Created by remay on 4/4/17.
//

#ifndef SCHED_RRSCHEDULER_H
#define SCHED_RRSCHEDULER_H

using namespace std;
#include "scheduler.h"

const static int RRquantum = 3;

class RRScheduler : public Scheduler{
public:
    Decision* get_next_thread(const Event* event);
    void enqueue(const Event* event, Thread* thread);
private:
    queue<Thread*> threads;
};

#endif //SCHED_RRSCHEDULER_H
