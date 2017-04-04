//
// Created by remay on 4/4/17.
//

#include "priorityscheduler.h"
using namespace std;

Decision* PriorityScheduler::get_next_thread(const Event* event){
    Thread *nextT = NULL;
    int i = 0;

    for (i = 0; i < 4; i++){
        if(!threads[i].empty()){
            nextT = threads[i].front();
            threads[i].pop();
            break;
        }
    }

    char reason[250];
    sprintf(reason, "Selected from %s queue with %u threads; will run until completion of burst.", nextT ? Process::get_type_name(nextT-> process -> get_type()).c_str() : "", (unsigned int) threads[i].size() + 1);
    string explain = string(reason);

    Decision *decision = new Decision(nextT, -1, explain);
    return decision;
}

void PriorityScheduler::enqueue(const Event* event, Thread* thread){
    int prior = thread->process->get_type();
    threads[prior].push(thread);
}