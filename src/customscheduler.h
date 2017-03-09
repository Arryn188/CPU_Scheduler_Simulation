#ifndef CUSTOMSCHEDULER_H
#define CUSTOMSCHEDULER_H
#include "scheduler.h"
#include <queue>
#include <vector>
#include <map>

const static int NUM_QUEUES = 6;
const static int CUSTOM_PREEMPTION = 7;

// CustomScheduler implements a slightly tweaked version of the MLFQ algorithm described in the textbook
class CustomScheduler : public Scheduler {

public:
	// Default Constructor
	CustomScheduler() : queues(NUM_QUEUES) {}

	// Returns next thread (or null if none left) that should be executed
	Decision* get_next_thread(const Event* event);

	// Adds a thread to the scheduler's collection of queues
	void enqueue(const Event* event, Thread* thread);

private:
	// Multilevel vector of queues populated by arriving threads, scheduled threads drawn from the highest non-empty queue
	std::vector<std::queue<Thread*>> queues;

	// Map to keep track of thread aging/demotion. Maps a pid to an map of thread ids to previous queue levels.
	std::map<int, std::map<int, int>> pids;
};

#endif