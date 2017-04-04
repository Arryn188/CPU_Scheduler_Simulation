//
// Created by remay on 4/4/17.
//

#include "rrscheduler.h"
using namespace std;

Decision* RRScheduler::get_next_thread(const Event* event){
    Thread *nextT = NULL;

    if(!threads.empty()){
        nextT = threads.front();
        threads.pop();
    }

    char reason[250];
    sprintf(reason, "Selected from %u threads; will run for %d time units or until completion of burst.", (unsigned int) threads.size() + 1, RRquantum);
    string explain = string(reason);

    Decision *decision = new Decision(nextT, RRquantum, explain);
    return decision;
}

void RRScheduler::enqueue(const Event *event, Thread *thread) {
    threads.push(thread);
}