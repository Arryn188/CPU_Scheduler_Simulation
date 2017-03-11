#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include <queue>
#include <fstream>
#include "print_opts.h"
#include "decision.h"
#include "scheduler.h"
#include "sys_statistics.h"


class Simulation {
public:

	Simulation(Scheduler *scheduler, Print_opts *print_opts);

	void run(std::string &file);

private:

	std::map<int, Process*> pidMap;

	std::priority_queue<Event*, std::vector<const Event*>, EventComparator> events;

	Scheduler *scheduler;

	Print_opts *print_opts;

	Thread *currentThread = NULL;

	Thread *previousThread = NULL;

	unsigned int thread_switch_overhead;
	unsigned int process_switch_overhead;

	unsigned int dispatch_time = 0;

	unsigned int start_time = 0;
	unsigned int end_time = 0;

	void parseFile(std::string &file);

	void updateStartandEnd(const Event *event);

	void handle_thread_arrived(const Event *event);
	void handle_dispatcher_invoked(const Event *event);
	void handle_dispatch_completed(const Event *event);
	void handle_thread_preempted(const Event *event);
	void handle_cpu_burst_completed(const Event *event);
	void handle_io_burst_completed(const Event *event);
	void handle_thread_completed(const Event *event);

	void printStatistics();
};

#endif