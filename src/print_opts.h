#ifndef PRINT_OPTS_H
#define PRINT_OPTS_H

#include "event.h"
#include "sys_statistics.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>


class Print_opts {
public:
	Print_opts(bool verbose, bool per_thread) {
		this->verbose = verbose;
		this->per_thread = per_thread;
	}

	void print_state_transition(const Event* event, Thread::State before_state, Thread::State after_state);

	void print_dispatch_invoked_message(const Event *event, Thread *thread, std::string message);

	void print_process_details(Process* process) const;

	void print_statistics(Sys_statistics stats) const;

private:

	bool verbose;

	bool per_thread;

	bool color;
};

#endif
