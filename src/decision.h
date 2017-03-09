#ifndef DECISION_H
#define DECISION_H

#include "thread.h"
#include <string>

// Defines a Single Scheduling Decision
class Decision {
public:
	// Paramaterized Constructor
	Decision(Thread* thread, unsigned int time_slice = -1, std::string explanation = "") {
		this->thread = thread;
		this->time_slice = time_slice;
		this->explanation = explanation;
	}

	unsigned int get_slice() const {
		return time_slice;
	}

	std::string get_explanation() {
		return explanation;
	}

	// Next thread (or null if none left) to run
	Thread* thread = NULL;

private:
	// Amount of time before thread should be preempted (or -1 for no preemption)
	unsigned int time_slice = -1;

	// Message comtaining scheduling choice
	std::string explanation;
};

#endif