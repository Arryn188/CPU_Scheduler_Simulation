//
// Created by remay on 4/4/17.
//

#ifndef SCHED_CUSTOMSCHEDULER_H
#define SCHED_CUSTOMSCHEDULER_H

#include "scheduler.h"

using namespace std;

const static int CUSTOMquantum = 5;
const static int NUM_Q = 6;

class CustomScheduler : public Scheduler{
public:
    CustomScheduler() : queues(NUM_Q){}
    Decision* get_next_thread(const Event* event);
    void enqueue(const Event* event, Thread* thread);
    std::map<int, std::map<int, int>> process_ids;

private:
    vector<queue<Thread*>> queues;
};


#endif //SCHED_CUSTOMSCHEDULER_H
