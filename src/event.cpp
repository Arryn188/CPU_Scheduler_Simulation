//
// Created by remay on 3/9/17.
//
#include "event.h"
using namespace std;


string Event::TypeArray[] = {"THREAD_ARRIVED", "THREAD_DISPATCH_COMPLETED", "PROCESS_DISPATCH_COMPLETED", "CPU_BURST_COMPLETED", "IO_BURST_COMPLETED", "THREAD_COMPLETED", "THREAD_PREEMPTED", "DISPATCHER_INVOKED"};

Event::Event(Type type, unsigned int time, Thread* thread, Decision* decision) {
	this->type = type;
	this->time = time;
	this->thread = thread;
	this->decision = decision;
}

Event::~Event() {
	if (decision)
		delete decision;
}

unsigned int Event::get_time() const {
	return time;
}

Event::Type Event::get_type() const {
	return type;
}

const Decision* Event::get_decision() const {
	return decision;
}

string Event::get_type_name(Event::Type type) {
	return TypeArray[type];
}
