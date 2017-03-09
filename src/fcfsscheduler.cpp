#include "fcfsscheduler.h"

// Returns the next thread for execution (or null if none are left)
Decision* FCFSScheduler::get_next_thread(const Event* event) {
	Thread *nextThread = NULL;

	// Pop the first thread from the queue
	if (!threads.empty()) {
		nextThread = threads.front();
		threads.pop();
	}

	// Describe reason for scheduling decision
	char buffer[100];
	sprintf(buffer, "Selected from %u threads; will run to completion of burst", (unsigned int) threads.size() + 1);
	std::string reason = std::string(buffer);

	// Instantiate a Decision object to pass back to the Simulation
	Decision *decision = new Decision(nextThread, -1, reason);
	return decision;
}

// Adds a thread to the scheduler's ready queue
void FCFSScheduler::enqueue(const Event* event, Thread* thread) {
	threads.push(thread);
}