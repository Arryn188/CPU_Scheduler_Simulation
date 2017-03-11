//
// Created by remay on 3/9/17.
//
#ifndef PRINT_OPTS_H
#define PRINT_OPTS_H

#include "event.h"
#include "sys_statistics.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

class Print_opts {
public:
	Print_opts(bool vb_opt, bool per_thread) {
		verbose = vb_opt;
		eachThread = per_thread;
	}

	void transitions(const Event *event, Thread::State before_state, Thread::State after_state);
	void dispatcher(const Event *event, Thread *thread, std::string message);
	void processes(Process *process) const;
	void stats(Sys_statistics stats) const;

private:
	bool verbose;
	bool eachThread;
};

#endif
