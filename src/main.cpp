//
// Created by remay on 3/9/17.
//
#include "simulation.h"
#include "fcfsscheduler.h"
#include <getopt.h>

using namespace std;

void parseOptions(int argc, char** argv, bool* perThread, bool* verbose, string *algorithm, string* simFile);
Scheduler* getScheduler(std::string &algorithm);

int main(int argc, char** argv) {

	bool perThreadOutput(false), verboseOutput(false);
	string algorithm, simulationFile;

	parseOptions(argc, argv, &perThreadOutput, &verboseOutput, &algorithm, &simulationFile);

	Print_opts* print_opts = new Print_opts(verboseOutput, perThreadOutput);

	if (algorithm.empty()) {
		algorithm = "FCFS";
	}

	Scheduler* scheduler = getScheduler(algorithm);

	if (!scheduler) {
		cout << "User inputted incorrect algorithm name: " << algorithm << endl;
		exit(-1);
	}

	Simulation mySimulation(scheduler, print_opts);

	mySimulation.run(simulationFile);

	delete print_opts;

	return 0;
}


void parseOptions(int argc, char** argv, bool* perThread, bool* verbose, string *algorithm, string* simFile) {
	static struct option long_options[] = {
	    {"per-thread",       no_argument, NULL, 't'},
	    {   "verbose",       no_argument, NULL, 'v'},
	    { "algorithm", required_argument, NULL, 'a'},
	    {      "help",       no_argument, NULL, 'h'},
	    {           0,                 0,    0,   0}
  	};

	while(true) {
		int option_index(0);
		int flag_char = getopt_long(argc, argv, "tva:ch", long_options, &option_index);

		if (flag_char == -1) break;

		switch(flag_char) {
		case 't':
			*perThread = true; break;

		case 'v':
			*verbose = true; break;

		case 'a':
			*algorithm = std::string(optarg); break;

		case 'h':
			std::cout << "CSCI442 Simulator by Becca May" << std::endl << std::endl
				 << "Usage: ./simulator [flags] [simulation_file]" << std::endl << std::endl
				 << "	-t, --per_thread			Output additional per-thread statistics for arrival time, service time, etc." << endl
				 << "	-v, --verbose 				Output information about every state-changing event and scheduling decision." << endl
				 << "	-a, --algorithm				The scheduling algorithm to use. One of FCFS, RR, PRIORITY, or CUSTOM." << endl
				 << "	-h, --help 				Display a help message about these flags and exit." << endl;
			exit(0);
		case '?':
			break;
		default:
			exit(-1);
		}
	}
	if (optind + 1 < argc) {
		std::cerr << "Incorrect flags" << std::endl;
		exit(-1);
	}

	if (optind >= argc) {
		std::cerr << "Incorrect flags" << std::endl;
		exit(-1);
	}

	*simFile = std::string(argv[optind]);
}

Scheduler* getScheduler(std::string &algorithm) {
	if (algorithm == "FCFS")
		return new FCFSScheduler();
	else 
		return NULL;
}
