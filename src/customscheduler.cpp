#include "customscheduler.h"

// Returns the next thread for execution (or null if none are left)(
Decision* CustomScheduler::get_next_thread(const Event* event) {
	Thread *nextThread = NULL;
	int i;

	// Iterate through queues, looking for a thread to run
	for (i  = 0; i < NUM_QUEUES; i++) {
		
		// Pop the first thread from the queue
		if (!queues[i].empty()) {
			nextThread = queues[i].front();
			queues[i].pop();
			break;
		}
	}

	// Describe reason for scheduling decision
	char buffer[200];
	sprintf(buffer, "Selected from queue #%u with %u threads; will run for %d time units or until completion of burst, whichever comes sooner",
		(unsigned int) i + 1, (unsigned int) queues[i].size() + 1, CUSTOM_PREEMPTION - i);
	std::string reason = std::string(buffer);

	// Instantiate a Decision object to pass back to the Simulation
	Decision *decision = new Decision(nextThread, CUSTOM_PREEMPTION - i, reason);
	return decision;
}

// Adds a thread to the scheduler's collection of queues
void CustomScheduler::enqueue(const Event* event, Thread* thread) {
	// Check if the thread was preempted
	bool threadPreempted = false;
	if (event->get_type() == Event::Type::THREAD_PREEMPTED)
		threadPreempted = true;

	// Get the previous queue that the thread waited in
	int previousQueue = (int) thread->process->get_type(); // Default to PRIORITY if thread is new
	if (pids.find(thread->process->get_pid()) != pids.end() && pids[thread->process->get_pid()].find(thread->get_id()) != pids[thread->process->get_pid()].end())
		previousQueue = pids[thread->process->get_pid()][thread->get_id()];

	// Decide queue that thread should be assigned to and assign the thread
	int newQueue = previousQueue + (threadPreempted ? 1 : 0);
	newQueue = (newQueue >= NUM_QUEUES ? NUM_QUEUES - 1 : newQueue);
	queues[newQueue].push(thread);

	// Update PID to Queue map
	pids[thread->process->get_pid()][thread->get_id()] = newQueue;
}