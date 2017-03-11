//
// Created by remay on 3/9/17.
//
#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include <queue>
#include <fstream>
#include "print_opts.h"
#include "decision.h"
#include "scheduler.h"
#include "sys_statistics.h"

using namespace std;

class Simulation {
public:

	Simulation(Scheduler *scheduler, Print_opts *print_opts);
	void run(std::string &file);

private:
	map<int, Process*> pidMap;
	priority_queue<Event*, vector<const Event*>, comparison> events;
	Scheduler *scheduler;
	Print_opts *print_opts;
	Thread *currentThread = NULL;
	Thread *previousThread = NULL;
	unsigned int thread_switch_overhead;
	unsigned int process_switch_overhead;
	unsigned int dispatch_time = 0;
	unsigned int start_time = 0;
	unsigned int end_time = 0;

	void fileIn(std::string &file);
	void updateTimes(const Event *event);
	void thread_arrived_update(const Event *event);
	void dispatcher_invoked_update(const Event *event);
	void dispatch_complete_update(const Event *event);
	void thread_preempted_update(const Event *event);
	void cpu_burst_complete_update(const Event *event);
	void io_burst_complete_update(const Event *event);
	void thread_complete_update(const Event *event);
	void printStats();
};

#endif