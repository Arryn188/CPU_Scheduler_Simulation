#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include <queue>
#include <fstream>
#include "logger.h"
#include "decision.h"
#include "scheduler.h"
#include "systemstats.h"


class Simulation {
public:
	// Parameterized Constructor
	Simulation(Scheduler *scheduler, Logger *logger);

	// Runs main event loop given a simulation filename
	void run(std::string &file);

private:
	// Map of PIDs to Processes
	std::map<int, Process*> pidMap;

	// Event Priority Queue
	std::priority_queue<Event*, std::vector<const Event*>, EventComparator> events;

	// Chosen scheduler
	Scheduler *scheduler;

	// Logger instance
	Logger *logger;

	// Thread currently running on processor
	Thread *currentThread = NULL;

	// Thread previously runnining on processor
	Thread *previousThread = NULL;

	// Overheads for thread and process switches
	unsigned int thread_switch_overhead;
	unsigned int process_switch_overhead;

	// Cumulative dispatch time
	unsigned int dispatch_time = 0;

	// Simulation start and end times
	unsigned int start_time = 0;
	unsigned int end_time = 0;

	// Parses simulation file and populates event queue
	void parseFile(std::string &file);

	// Update the start and end points of the simulation iteratively
	void updateStartandEnd(const Event *event);

	// Event Handlers
	void handle_thread_arrived(const Event *event);
	void handle_dispatcher_invoked(const Event *event);
	void handle_dispatch_completed(const Event *event);
	void handle_thread_preempted(const Event *event);
	void handle_cpu_burst_completed(const Event *event);
	void handle_io_burst_completed(const Event *event);
	void handle_thread_completed(const Event *event);

	// Prints simulation statistics
	void printStatistics();
};

#endif