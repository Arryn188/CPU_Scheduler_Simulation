#include "thread.h"

// Out-of-class Static Initializer
std::string Thread::StateArray[] = {
	"NEW", "READY", "RUNNING", "BLOCKED", "EXIT"
};

// Paramterized Constructor
Thread::Thread(int id, Process* parent, unsigned int arrival_time) {
	this->id = id;
	this->process = parent;
	this->arrival_time = arrival_time;
}

// Returns id
int Thread::get_id() {
	return id;
}

void Thread::set_ready(unsigned int time) {
	// Update thread state
	previous_state = current_state;
	current_state = State::READY;

	// Update previously running or blocked, update the Burst queue
	if (previous_state == State::RUNNING || previous_state == State::BLOCKED) {
		updateCounters(time);
	}

	// Update last_state_change time
	last_state_change = time;
}

unsigned int Thread::set_running(unsigned int time) {
	// Assert previously ready
	assert(current_state == State::READY);

	// Update thread state
	previous_state = current_state;
	current_state = State::RUNNING;

	// Update start_time if this is the first time the thread has executed
	if (start_time == -1)
		start_time = time;

	// Update last_state_change time
	last_state_change = time;

	// Return the length of the current CPU burst
	return bursts.front()->get_length();
}

unsigned int Thread::set_blocked(unsigned int time) {
	// Assert previously running
	assert(current_state == State::RUNNING);

	// Update Service tim counter
	updateCounters(time);

	// Return now if thread is exiting
	if (bursts.size() == 0)
		return -1;

	// Update thread state
	previous_state = current_state;
	current_state = State::BLOCKED;

	//Update last_state_change time
	last_state_change = time;

	// Return the length of the current IO burst (or -1 if the thread is done)
	return bursts.front()->get_length();
}

void Thread::set_finished(unsigned int time) {
	// Assert previously running
	assert(current_state == State::RUNNING);

	// Update thread state
	previous_state = current_state;
	current_state = State::EXIT;

	// Update end_time
	end_time = time;

	// Update last_state_change time
	last_state_change = time;
}

// pushBursts adds a burst to the Burst queue
void Thread::pushBurst(Burst* burst) {
	// Push the burst onto the queue
	bursts.push(burst);
}

// get_arrival_time returns arrival time
unsigned int Thread::get_arrival_time() {
	return arrival_time;
}

// get_service_time returns CPU time
unsigned int Thread::get_service_time() {
	return service_time;
}

// get_io_time returns IO time
unsigned int Thread::get_io_time() {
	return io_time;
}

// get_end_time returns end time
unsigned int Thread::get_end_time() {
	return end_time;
}

// get_response_time returns response time
unsigned int Thread::get_response_time() {
	// Assert that thread has exited
	assert(current_state == State::EXIT);

	// Return response time
	return start_time - arrival_time;
}

// get_turnaround_time returns turnaround time
unsigned int Thread::get_turnaround_time() {
	// Assert that thread has exited
	assert(current_state == State::EXIT);

	// Return turnaround time
	return end_time - arrival_time;
}

// get_state_name is a static member that returns the name given a particular state
std::string Thread::get_state_name(Thread::State state) {
	return StateArray[state];
}

// updateCounters should be called after an IO/CPU burst. It updates the Burst queue and the service/io time counters
void Thread::updateCounters(unsigned int time) {
	// Retrieve the current burst that is executing/blocking
	Burst *burst = bursts.front();

	// Pop the burst if it was completed
	if (burst->get_length() <= time - last_state_change)
		bursts.pop();
	
	// Update the burst it was preempted
	else
		burst->decrement_length(time - last_state_change);

	// Update the appropriate counter based on the type of burst
	if (burst->get_type() == Burst::Type::CPU)
		service_time += time - last_state_change;
	else if (burst->get_type() == Burst::Type::IO)
		io_time += time - last_state_change;
}