//
// Created by remay on 3/9/17.
//

#include "thread.h"
using namespace std;

string Thread::StateArray[] = {"NEW", "READY", "RUNNING", "BLOCKED", "EXIT"};

Thread::Thread(int id, Process* parent, unsigned int arrival_time) {
	this->id = id;
	this->process = parent;
	this->arr_time = arrival_time;
}

int Thread::get_id() {
	return id;
}

void Thread::set_ready(unsigned int time) {
	prev_state = curr_state;
	curr_state = State::READY;

	if (prev_state == State::RUNNING || prev_state == State::BLOCKED) {
		do_update(time);
	}
	last_state_change = time;
}

unsigned int Thread::set_running(unsigned int time) {
	prev_state = curr_state;
	curr_state = State::RUNNING;

	if (s_time == -1) {
		s_time = time;
	}

	last_state_change = time;
	return bursts.front()->get_length();
}

unsigned int Thread::set_blocked(unsigned int time) {
	do_update(time);
	if (bursts.size() == 0) {
		return -1;
	}
	prev_state = curr_state;
	curr_state = State::BLOCKED;

	last_state_change = time;
	return bursts.front()->get_length();
}

void Thread::set_finished(unsigned int time) {
	prev_state = curr_state;
	curr_state = State::EXIT;
	e_time = time;
	last_state_change = time;
}

void Thread::pushBurst(Burst* burst) {
	bursts.push(burst);
}
unsigned int Thread::get_arrival_time() {
	return arr_time;
}

unsigned int Thread::get_service_time() {
	return service_time;
}

unsigned int Thread::get_io_time() {
	return io_time;
}

unsigned int Thread::get_end_time() {
	return e_time;
}

unsigned int Thread::get_response_time() {
	return s_time - arr_time;
}

unsigned int Thread::get_turnaround_time() {
	return e_time - arr_time;
}

string Thread::get_state_name(Thread::State thread_state) {
	return StateArray[thread_state];
}


void Thread::do_update(unsigned int time) {
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