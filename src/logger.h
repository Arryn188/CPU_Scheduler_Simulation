#ifndef LOGGER_H
#define LOGGER_H

#include "event.h"
#include "systemstats.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

// Color Definitions
#define RST  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"

class Logger {
public:
	// Constructor
	Logger(bool verbose, bool per_thread, bool color) {
		this->verbose = verbose;
		this->per_thread = per_thread;
		this->color = color;
	}

	// If verbose is true, prints a state-transition message
	void print_state_transition(const Event* event, Thread::State before_state, Thread::State after_state);

	// If verbose is true, prints a custom diespatcher invoked message
	void print_dispatch_invoked_message(const Event *event, Thread *thread, std::string message);

	// if per_thread is true, prints information about a process and its threads
	void print_process_details(Process* process) const;

	// Prints overall simulation statistics
	void print_statistics(SystemStats stats) const;

private:
	// Determines verbose/non-verbose mode
	bool verbose;

	// Determines whether or not to print per-process statistics
	bool per_thread;

	// Print the output with color highlighting
	bool color;
};

#endif
