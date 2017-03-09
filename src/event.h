#ifndef EVENT_H
#define EVENT_H

#include "decision.h"
#include "thread.h"

#define NUM_EVENTS 8

class Event {
public:
	// Event Type Enumeration
	enum Type {
		THREAD_ARRIVED, THREAD_DISPATCH_COMPLETED, PROCESS_DISPATCH_COMPLETED, CPU_BURST_COMPLETED,
		IO_BURST_COMPLETED, THREAD_COMPLETED, THREAD_PREEMPTED, DISPATCHER_INVOKED
	};

	// Constructor
	Event(Type type, unsigned int time, Thread* thread, Decision* decision);

	// Destructor
	~Event();

	// Returns time of event
	unsigned int get_time() const;

	// Returns type of event
	Type get_type() const;

	// Returns a pointer to the decision
	const Decision* get_decision() const;

	// Returns name of a given event
	static std::string get_type_name(Type type);

	// Associated thread (if applicable)
	Thread* thread;

private:
	// Type of Event
	Type type;

	// Time at which event occurs
	unsigned int time;

	// Associated scheduling decision (if applicable)
	const Decision* decision;

    // Corresponding Name Array
    static std::string TypeArray[NUM_EVENTS];

};

// EventComparator is a callback that orders events based on their time of occurence
struct EventComparator {
	bool operator()(const Event* e1, const Event* e2) {
		return e1->get_time() >= e2->get_time();
	}
};

#endif