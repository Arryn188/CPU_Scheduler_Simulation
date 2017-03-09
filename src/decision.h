#ifndef DECISION_H
#define DECISION_H

#include "thread.h"
#include <string>

class Decision {
public:

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

	Thread* thread = NULL;

private:

	unsigned int time_slice = -1;

	std::string explanation;
};

#endif