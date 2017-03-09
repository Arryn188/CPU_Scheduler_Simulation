#include "rrscheduler.h"

// Returns the next thread for execution (or null if none are left)
Decision* RRScheduler::get_next_thread(const Event* event) {
	Thread *nextThread = NULL;

	// Pop the first thread from the queue
	if (!threads.empty()) {
		nextThread = threads.front();
		threads.pop();
	}

	// Describe reason for scheduling decision
	char buffer[200];
	sprintf(buffer, "Selected from %u threads; will run for %d time units or until completion of burst, whichever comes sooner",
		(unsigned int) threads.size() + 1, RR_PREEMPTION);
	std::string reason = std::string(buffer);

	// Instantiate a Decision object to pass back to the Simulation
	Decision *decision = new Decision(nextThread, RR_PREEMPTION, reason);
	return decision;
}

// Adds a thread to the scheduler's ready queue
void RRScheduler::enqueue(const Event* event, Thread* thread) {
	threads.push(thread);
}