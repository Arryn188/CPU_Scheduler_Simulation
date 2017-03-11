//
// Created by remay on 3/9/17.
//

#include "thread.h"

std::string Thread::StateArray[] = {
	"NEW", "READY", "RUNNING", "BLOCKED", "EXIT"
};

Thread::Thread(int id, Process* parent, unsigned int arrival_time) {
	this->id = id;
	this->process = parent;
	this->arrival_time = arrival_time;
}

int Thread::get_id() {
	return id;
}

void Thread::set_ready(unsigned int time) {
	previous_state = current_state;
	current_state = State::READY;

	if (previous_state == State::RUNNING || previous_state == State::BLOCKED) {
		updateCounters(time);
	}
	last_state_change = time;
}

unsigned int Thread::set_running(unsigned int time) {
	previous_state = current_state;
	current_state = State::RUNNING;

	if (start_time == -1)
		start_time = time;

	last_state_change = time;

	return bursts.front()->get_length();
}

unsigned int Thread::set_blocked(unsigned int time) {
	updateCounters(time);
	if (bursts.size() == 0)
		return -1;
	previous_state = current_state;
	current_state = State::BLOCKED;

	last_state_change = time;
	return bursts.front()->get_length();
}

void Thread::set_finished(unsigned int time) {
	previous_state = current_state;
	current_state = State::EXIT;

	end_time = time;

	last_state_change = time;
}

void Thread::pushBurst(Burst* burst) {
	bursts.push(burst);
}
unsigned int Thread::get_arrival_time() {
	return arrival_time;
}

unsigned int Thread::get_service_time() {
	return service_time;
}

unsigned int Thread::get_io_time() {
	return io_time;
}

unsigned int Thread::get_end_time() {
	return end_time;
}

unsigned int Thread::get_response_time() {
	return start_time - arrival_time;
}

unsigned int Thread::get_turnaround_time() {
	return end_time - arrival_time;
}

std::string Thread::get_state_name(Thread::State state) {
	return StateArray[state];
}


void Thread::updateCounters(unsigned int time) {
	Burst *burst = bursts.front();

	if (burst->get_length() <= time - last_state_change) {
		bursts.pop();
	}

	else {
		burst->decrement_length(time - last_state_change);
	}

	if (burst->get_type() == Burst::Type::CPU) {
		service_time += time - last_state_change;
	}
	else if (burst->get_type() == Burst::Type::IO) {
		io_time += time - last_state_change;
	}
}