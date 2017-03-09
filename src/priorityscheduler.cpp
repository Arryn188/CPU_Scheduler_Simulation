#include "priorityscheduler.h"

// Returns the next thread for execution (or null if none are left)
Decision* PriorityScheduler::get_next_thread(const Event* event) {
	Thread *nextThread = NULL;
	int queueIterator = 0;

	// Pop the first thread from the first non-empty queue
	for (queueIterator = 0; queueIterator < 4; queueIterator++) {
		if (!threads[queueIterator].empty()) {
			nextThread = threads[queueIterator].front();
			threads[queueIterator].pop();
			break;
		}
	}

	// Describe reason for scheduling decision
	char buffer[200];
	sprintf(buffer, "Selected from %s queue with %u threads; will run until completion of burst",
		nextThread ? Process::get_type_name(nextThread->process->get_type()).c_str() : "", (unsigned int) threads[queueIterator].size() + 1);
	std::string reason = std::string(buffer);

	// Instantiate a Decision object to pass back to the Simulation
	Decision *decision = new Decision(nextThread, -1, reason);
	return decision;
}

// Adds a thread to the appropriate ready queue based on the thread's priority
void PriorityScheduler::enqueue(const Event* event, Thread* thread) {
	int priority = thread->process->get_type();
	threads[priority].push(thread);
}