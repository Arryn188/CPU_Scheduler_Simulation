//
// Created by remay on 4/4/17.
//

#ifndef SCHED_PRIORITYSCHEDULER_H
#define SCHED_PRIORITYSCHEDULER_H
#include "scheduler.h"

using namespace std;

class PriorityScheduler: public Scheduler{
public:
    Decision* get_next_thread(const Event* event);
    void enqueue(const Event * event, Thread* thread);
};


#endif //SCHED_PRIORITYSCHEDULER_H
