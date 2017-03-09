#ifndef SYSTEMSTATS_H
#define SYSTEMSTATS_H

// The SystemStats struct holds various computed statistics about the performance of the system
struct SystemStats {

/* All arrays reference the order of Process::Type */

	// Count of each type of thread (SYSTEM, INTERACTIVE, NORMAL, BATCH)
	int count[4];

	// Average response time of each type of thread (SYSTEM, INTERACTIVE, NORMAL, BATCH)
	double response[4];

	// Average turnaround time of each type of thread (SYSTEM, INTERACTIVE, NORMAL, BATCH)
	double turnaround[4];

	// Total elapsed time
	int elapsed;

	// Total service time
	int service;

	// Total IO time
	int io;

	// Total dispatch time;
	int dispatch;

	// Total idle time
	int idle;

	// CPU utilization
	double utilization;

	// CPU efficiency
	double efficiency;
};

#endif