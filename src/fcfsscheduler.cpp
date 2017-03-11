//
// Created by remay on 3/9/17.
//
#include "fcfsscheduler.h"

Decision* FCFSScheduler::get_next_thread(const Event* event) {
	Thread *nextThread = NULL;

	if (!threads.empty()) {
		nextThread = threads.front();
		threads.pop();
	}

	char buffer[100];
	sprintf(buffer, "Selected from %u threads; will run to completion of burst", (unsigned int) threads.size() + 1);
	std::string reason = std::string(buffer);

	Decision *decision = new Decision(nextThread, -1, reason);
	return decision;
}

void FCFSScheduler::enqueue(const Event* event, Thread* thread) {
	threads.push(thread);
}