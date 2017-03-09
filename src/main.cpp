#include "simulation.h"
#include "fcfsscheduler.h"
#include "rrscheduler.h"
#include "priorityscheduler.h"
#include "customscheduler.h"
#include <getopt.h>
#include <string> 
#include <iostream>

// Function prototypes
void parseOptions(int argc, char** argv, bool* perThread, bool* verbose, bool* color, std::string *algorithm, std::string* simFile);
Scheduler* getScheduler(std::string &algorithm);

int main(int argc, char** argv) {
	// Containers for command line options
	bool perThreadOutput(false), verboseOutput(false), colorOutput(false);
	std::string algorithm, simulationFile;

	// Parse command line options using getoptlong
	parseOptions(argc, argv, &perThreadOutput, &verboseOutput, &colorOutput, &algorithm, &simulationFile);

	// Create a logger instance
	Logger* logger = new Logger(verboseOutput, perThreadOutput, colorOutput);

	// Default to FCFS algorithm if none is specified
	if (algorithm.empty())
		algorithm = "FCFS";

	// Generate a scheduler instance given a scheduling algorithm name
	Scheduler* scheduler = getScheduler(algorithm);

	// Invalid scheduler name
	if (!scheduler) {
		std::cerr << "Invalid identifier for a scheduling algorithm: " << algorithm << std::endl;
		exit(-1);
	}

	// Create simulation instance
	Simulation mySimulation(scheduler, logger);

	// Run simulation event loop
	mySimulation.run(simulationFile);

	// Clean-up Allocated Memory
	delete logger;

	// Exit
	return 0;
}

// parseOptions parses the command line options using getoptlong
void parseOptions(int argc, char** argv, bool* perThread, bool* verbose, bool *color, std::string *algorithm, std::string* simFile) {
	// Long option definitions
	static struct option long_options[] = {
	    {"per-thread",       no_argument, NULL, 't'},
	    {   "verbose",       no_argument, NULL, 'v'},
	    { "algorithm", required_argument, NULL, 'a'},
	    {     "color",       no_argument, NULL, 'c'},
	    {      "help",       no_argument, NULL, 'h'},
	    {           0,                 0,    0,   0}
  	};

  	// getopt loop
	while(true) {
		// Counter for get-opt
		int option_index(0);

		// Get next option and corresponding argument
		int flag_char = getopt_long(argc, argv, "tva:ch", long_options, &option_index);

		// No more options -> Quit
		if (flag_char == -1) break;

		switch(flag_char) {
		// Per-Thread Statistics
		case 't':
			*perThread = true; break;

		// Verbose Output
		case 'v':
			*verbose = true; break;

		// Algorithm Choice
		case 'a':
			*algorithm = std::string(optarg); break;

		// Colorized Output
		case 'c':
			*color = true; break;

		// Print Help
		case 'h':
			std::cout << "Simulator v1.0 -- by Ryan Hunt" << std::endl << std::endl
				 << "Usage: ./simulator [flags] [simulation_file]" << std::endl << std::endl
				 << "	-t, --per_thread			Output additional per-thread statistics for arrival time, service time, etc." << std::endl
				 << "	-v, --verbose 				Output information about every state-changing event and scheduling decision." << std::endl
				 << "	-a, --algorithm				The scheduling algorithm to use. One of FCFS, RR, PRIORITY, or CUSTOM." << std::endl
			 	 << "	-c, --color				Display statistics with color highlighting." << std::endl
				 << "	-h, --help 				Display a help message about these flags and exit." << std::endl;
			exit(0);

		// Error Case
		case '?':
			break;

		// Unhandled Flag
		default:
			exit(-1);
		}
	}

	// Parse non-option arguments

	// Too many non-option arguments
	if (optind + 1 < argc) {
		std::cerr << "Too many non-option argument provided. Run './simulator -h' for more information." << std::endl;
		exit(-1);
	}

	// Not enough arguments
	if (optind >= argc) {
		std::cerr << "Not enough non-option arguments provided (did you forget to provide a simulation file?). Run './simulator -h' for more information." << std::endl;
		exit(-1);
	}

	// Corrent number of arguments -> Get filename
	*simFile = std::string(argv[optind]);
}

// getScheduler returns an instance of a scheduler given the name of the algorithm, or NULL if the algorithm doesn't exist
Scheduler* getScheduler(std::string &algorithm) {
	// First Come, First Serve Scheduler
	if (algorithm == "FCFS")
		return new FCFSScheduler();

	// Round Robin Scheduler
	else if (algorithm == "RR")
		return new RRScheduler();

	// Priority Scheduler
	else if (algorithm == "PRIORITY")
		return  new PriorityScheduler();

	// Custom Scheduler
	else if (algorithm == "CUSTOM")
		return new CustomScheduler();

	// Invalid Nmme -- Return NULL
	else 
		return NULL;
}
