#include "event.h"

// Out-of-class Static Initializer
std::string Event::TypeArray[] = {
	"THREAD_ARRIVED", "THREAD_DISPATCH_COMPLETED", "PROCESS_DISPATCH_COMPLETED", "CPU_BURST_COMPLETED",
    "IO_BURST_COMPLETED", "THREAD_COMPLETED", "THREAD_PREEMPTED", "DISPATCHER_INVOKED"
};

// Parameterized Constructor
Event::Event(Type type, unsigned int time, Thread* thread, Decision* decision) {
	this->type = type;
	this->time = time;
	this->thread = thread;
	this->decision = decision;
}

// Deconstructor
Event::~Event() {
	// If a decision is attached to the event, release it
	if (decision)
		delete decision;
}

// Returns time of event
unsigned int Event::get_time() const {
	return time;
}

// Returns type of event
Event::Type Event::get_type() const {
	return type;
}

// Returns a pointer to the Decision
const Decision* Event::get_decision() const {
	return decision;
}

// Returns name of a given event
std::string Event::get_type_name(Event::Type type) {
	return TypeArray[type];
}
