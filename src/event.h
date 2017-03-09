#ifndef EVENT_H
#define EVENT_H

#include "decision.h"
#include "thread.h"

#define NUM_EVENTS 8

class Event {
public:

	enum Type { THREAD_ARRIVED, THREAD_DISPATCH_COMPLETED, PROCESS_DISPATCH_COMPLETED, CPU_BURST_COMPLETED, IO_BURST_COMPLETED, THREAD_COMPLETED, THREAD_PREEMPTED, DISPATCHER_INVOKED };

	Event(Type type, unsigned int time, Thread* thread, Decision* decision);

	~Event();

	unsigned int get_time() const;

	Type get_type() const;

	const Decision* get_decision() const;

	static std::string get_type_name(Type type);

	Thread* thread;

private:

	Type type;

	unsigned int time;

	const Decision* decision;

    static std::string TypeArray[NUM_EVENTS];

};

struct EventComparator {
	bool operator()(const Event* e1, const Event* e2) {
		return e1->get_time() >= e2->get_time();
	}
};

#endif