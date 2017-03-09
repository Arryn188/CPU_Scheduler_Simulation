#include "simulation.h"

// Constructor
Simulation::Simulation(Scheduler *scheduler, Logger *logger) {
	this->scheduler = scheduler;
	this->logger = logger;
}

void Simulation::run(std::string &file) {
	// Populate the event queue
	parseFile(file);

	// Run event loop while event queue isn't empty
	while (!events.empty()) {
		// Get top element of queue
		const Event* event = events.top();
		events.pop();


        std::cout << "At time " <<event->get_time() << ":" << std::endl;
        std::cout << event->get_type_name(event->get_type())<< std::endl;
        std::cout << "thread " << event->thread->get_id() + 1 << " in process " << event->thread->process->get_pid()<< " "<<event->thread->process->get_type_name(event->thread->process->get_type()) <<std::endl;
        std::cout << std::endl;

		delete event;
	}
}

// handle_thread_arrived handles the THREAD_ARRIVED event in the event queue
void Simulation::handle_thread_arrived(const Event *event) {

}

// handle_dispatcher_invoked handles the DISPATCHER_INVOKED event in the event queue
void Simulation::handle_dispatcher_invoked(const Event *event) {

}

// handle_dispath_completed handles the THREAD_DISPATCH_COMPLETED and PROCESS_DISPATCH_COMPLETED events in the event queue
void Simulation::handle_dispatch_completed(const Event *event) {

}

// handle_thread_preempted handles the THREAD_PREEMPTED event in the event queue
void Simulation::handle_thread_preempted(const Event *event) {

}

// handle_burst_completed handles the CPU_BURST_COMPLETED event in the event queue
void Simulation::handle_cpu_burst_completed(const Event *event) {

}

// handle_io_burst_completed handles the IO_BURST_COMPLETED event in the event queue
void Simulation::handle_io_burst_completed(const Event *event) {

}

// handle_thread_completed handles the THREAD_COMPLETED event in the event queue
void Simulation::handle_thread_completed(const Event *event) {

}

void Simulation::parseFile(std::string &file) {
	std::ifstream simFile(file);

	// ifstream error
	if (!simFile) {
		std::cerr << "Cannot open: " << file << std::endl;
		exit(-2);
	}

	// Temporary variables
	int num_processes(-1), pid(0), type(0), num_threads(0), arrival_time(0), num_bursts(0), burst_length(0);

	// First line of simulation file
	simFile >> num_processes >> thread_switch_overhead >> process_switch_overhead;

	// Iterate through processes
	for (int i = 0; i < num_processes; i++) {
		simFile >> pid >> type >> num_threads;

		// Create a process object
		Process* process = new Process(pid, static_cast<Process::Type>(type));

		// Iterate through threads
		for (int j = 0; j < num_threads; j++) {
			simFile >> arrival_time >> num_bursts;

			// Create a thread, passing the constructor the parent process
			Thread* thread = new Thread(j, process, arrival_time);

			// Iterate through bursts
			for (int k = 0; k < num_bursts; k++) {

				// Iterate through alternating CPU and IO bursts
				for (int l = 0; l < 2 && k + l < num_bursts; l++) {
					// Create a CPU burst object
					simFile >> burst_length;
					Burst *burst = new Burst(static_cast<Burst::Type>(l), burst_length);

					// Add the burst to the thread's queue
					thread->pushBurst(burst);
				}
			}

			// Add the thread to the parent's thread vector
			process->pushThread(thread);

			// Create an event, passing the constructor the thread
			Event* event = new Event(Event::Type::THREAD_ARRIVED, arrival_time, thread, NULL);

			// Add the event to the event queue
			events.push(event);
		}

		// Add the process to the simulation process map
		pidMap[pid] = process;
	}
}

// UpdateStartandEnd iteratively updates the start and end times of the simulation
void Simulation::updateStartandEnd(const Event *event) {

}

// printStatistics prints required statistics from the simulation
void Simulation::printStatistics() {

}