#ifndef THREAD_H
#define THREAD_H

#include "burst.h"
#include "process.h"
#include <queue>
#include <cassert>

#define NUM_STATES 5

// Forward Declaration for Process class
class Process;

class Thread {
public:
	// Thread/Process State Enumeration
	enum State {
		NEW, READY, RUNNING, BLOCKED, EXIT
	};

	// Constructor
	Thread(int id, Process* parent, unsigned int arrival_time);

	// Returns id
	int get_id();

	// Returns arrival time
	unsigned int get_arrival_time();

	// Returns CPU time
	unsigned int get_service_time();

	// Returns IO time
	unsigned int get_io_time();

	// Returns end time
	unsigned int get_end_time();

	// Returns response time
	unsigned int get_response_time();

	// Returns turnaround time
	unsigned int get_turnaround_time();

	// Returns a state name
	static std::string get_state_name(State state);

	// Sets a thread to ready
	void set_ready(unsigned int time);

	// Sets a thread to running, returns cpu burst length
	unsigned int set_running(unsigned int time);

	// Sets a thread to blocked, returns io burst length (or -1 if done)
	unsigned int set_blocked(unsigned int time);

	// Sets a thread to finished
	void set_finished(unsigned int time);

	// Push a burst onto the burst queue
	void pushBurst(Burst* burst);

	// Parent process of thread
	Process* process;

private:
	// PID
	int id;

	// Burst associated with thread
	std::queue<Burst*> bursts;

	// Current state of thread
	State current_state = NEW;

	// Previous state of thread
	State previous_state = NEW;

	// Thread arrival time
	int arrival_time = -1;

	// Time of first execution
	int start_time = -1;

	// Time of last execution
	int end_time = -1;

	// Time spent executing
	unsigned int service_time = 0;

	// Time spent doing I/O
	unsigned int io_time = 0;

	// Time of last state change
	 int last_state_change = -1;

    // Corresponding Name Array for States
    static std::string StateArray[NUM_STATES];

    // Updates the time counters (service_time and io_time) and managaes Burst queue
    void updateCounters(unsigned int time);
};

#endif
