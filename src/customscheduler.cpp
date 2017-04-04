//
// Created by remay on 4/4/17.
//

#include "customscheduler.h"
using namespace std;

Decision* CustomScheduler::get_next_thread(const Event* event){
    Thread *nextT = NULL;
    int i = 0;

    for(i = 0; i < NUM_Q; i++){
        if(!queues[i].empty()){
            nextT = queues[i].front();
            queues[i].pop();
            break;
        }
    }

    char reason[250];
    sprintf(reason, "Selected from queue %u with %u threads, will run for %d time units or until completion of burst.", (unsigned int) i + 1, (unsigned int) queues[i].size() + 1, CUSTOMquantum - i);
    string explain = string(reason);

    Decision *decision = new Decision(nextT, CUSTOMquantum - i, explain);
    return decision;
}

void CustomScheduler::enqueue(const Event* event, Thread* thread){
    bool threadPreempt = false;
    if(event -> get_type() == Event::Type::THREAD_PREEMPTED){
        threadPreempt = true;
    }

    int prevQ = (int) thread -> process -> get_type();
    if(process_ids.find(thread->process->get_pid()) != process_ids.end() && process_ids[thread -> process -> get_pid()].find(thread -> get_id()) != process_ids[thread -> process -> get_pid()].end()){
        prevQ = process_ids[thread -> process -> get_pid()][thread -> get_id()];
    }

    int newQ = prevQ + (threadPreempt ? 1 : 0);
    newQ = (newQ >= NUM_Q ? NUM_Q - 1 : newQ);
    queues[newQ].push(thread);

    process_ids[thread -> process -> get_pid][thread -> get_id()] = newQ;
}