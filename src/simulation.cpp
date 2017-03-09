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

		// Find the starting and ending points of the simulation iteratively
		updateStartandEnd(event);

		// Invoke appropriate method for each event
		switch(event->get_type()) {
			case Event::THREAD_ARRIVED:
				handle_thread_arrived(event); break;
			case Event::DISPATCHER_INVOKED:
				handle_dispatcher_invoked(event); break;
			case Event::THREAD_DISPATCH_COMPLETED:
			case Event::PROCESS_DISPATCH_COMPLETED:
				handle_dispatch_completed(event); break;
			case Event::THREAD_PREEMPTED:
				handle_thread_preempted(event); break;
			case Event::CPU_BURST_COMPLETED:
				handle_cpu_burst_completed(event); break;
			case Event::IO_BURST_COMPLETED:
				handle_io_burst_completed(event); break;
			case Event::THREAD_COMPLETED:
				handle_thread_completed(event); break;
		}

		// Free event
		delete event;
	}

	// Print simulation statistics
	printStatistics();
}

// handle_thread_arrived handles the THREAD_ARRIVED event in the event queue
void Simulation::handle_thread_arrived(const Event *event) {
	// Set the new thread to READY state
	event->thread->set_ready(event->get_time());

	// Enqueue the new thread for future execution
	scheduler->enqueue(event, event->thread);

	// If the processor is idle, invoke the scheduler right now
	if (!currentThread)
		events.push(new Event(Event::Type::DISPATCHER_INVOKED, event->get_time(), NULL, NULL));

	// Print a state transition, if so requested by the user
	logger->print_state_transition(event, Thread::State::NEW, Thread::State::READY);
}

// handle_dispatcher_invoked handles the DISPATCHER_INVOKED event in the event queue
void Simulation::handle_dispatcher_invoked(const Event *event) {
	// Get next scheduling decision from the scheduler
	Decision *decision = scheduler->get_next_thread(event);

	// Check we are done running (for now...events may still remain in the event queue which produce more threads)
	if (!decision->thread)
		return;

	// Set the chosen thread as the thread currently running on the processor
	currentThread = decision->thread;

	// Dispath appropriate event depending on if a process or thread switch has occured
	if (previousThread && currentThread && previousThread->process == currentThread->process) {
		// Push a THREAD_DISPATCH_COMPLETED event onto the queue, passing the associated thread and scheduling decision
		events.push(new Event(Event::Type::THREAD_DISPATCH_COMPLETED, event->get_time() + thread_switch_overhead, decision->thread, decision));

		// Update dispatch overhead counter
		dispatch_time += thread_switch_overhead;
	} else {
		// Push a PROCESS_DISPATCH_COMPLETED event onto the queue, passing the associated thread and scheduling deicison
		events.push(new Event(Event::Type::PROCESS_DISPATCH_COMPLETED, event->get_time() + process_switch_overhead, decision->thread, decision));

		// Update dispatch overhead counter
		dispatch_time += process_switch_overhead;
	}

	// Print a dispatcher invoked message, if so requested by the user
	logger->print_dispatch_invoked_message(event, decision->thread, decision->get_explanation());
}

// handle_dispath_completed handles the THREAD_DISPATCH_COMPLETED and PROCESS_DISPATCH_COMPLETED events in the event queue
void Simulation::handle_dispatch_completed(const Event *event) {
	// Set the thread to running, store the thread's expected burst time
	int burst_length = event->thread->set_running(event->get_time());

	// Get the time slice provided by the scheduler's decision object
	int slice_length = event->get_decision()->get_slice();

	// Add a preemption event a length of one time slice in the future if preemption occurs before burst completes
	if (slice_length != -1 && slice_length < burst_length)
		events.push(new Event(Event::Type::THREAD_PREEMPTED, event->get_time() + slice_length, event->thread, NULL));
	// Add a cpu burst completion event in the future if the burst completes before preemption
	else
		events.push(new Event(Event::Type::CPU_BURST_COMPLETED, event->get_time() + burst_length, event->thread, NULL));

	// Print a state transition, if so requested by the useer
	logger->print_state_transition(event, Thread::State::READY, Thread::State::RUNNING);
}

// handle_thread_preempted handles the THREAD_PREEMPTED event in the event queue
void Simulation::handle_thread_preempted(const Event *event) {
	// Set the thread to ready
	event->thread->set_ready(event->get_time());

	// Put the thread back in the scheduler's ready queue
	scheduler->enqueue(event, event->thread);

	// Update the previous and currently running threads
	previousThread = currentThread;
	currentThread = NULL;

	// If the processor is idle, invoke the scheduler right now
	if (!currentThread)
		events.push(new Event(Event::Type::DISPATCHER_INVOKED, event->get_time(), NULL, NULL));

	// Print a state transition, if so requested by the user
	logger->print_state_transition(event, Thread::State::RUNNING, Thread::State::READY);
}

// handle_burst_completed handles the CPU_BURST_COMPLETED event in the event queue
void Simulation::handle_cpu_burst_completed(const Event *event) {
	// Set the thread to blocked, store the thread's expected block time
	int burst_length = event->thread->set_blocked(event->get_time());

	// If burst_length is -1, we know that the thread is done
	if (burst_length == -1) {
		events.push(new Event(Event::Type::THREAD_COMPLETED, event->get_time(), event->thread, NULL));
		return;
	}

	// Update the previous and currently running threads
	previousThread = currentThread;
	currentThread = NULL;

	// Add an IO burst completed event in the future based on the response from the thread object
	events.push(new Event(Event::Type::IO_BURST_COMPLETED, event->get_time() + burst_length, event->thread, NULL));

	// If the processor is idle, invoke the scheduler right now
	if (!currentThread)
		events.push(new Event(Event::Type::DISPATCHER_INVOKED, event->get_time(), NULL, NULL));

	// Print a state transition, if so requested by the user
	logger->print_state_transition(event, Thread::State::RUNNING, Thread::State::BLOCKED);
}

// handle_io_burst_completed handles the IO_BURST_COMPLETED event in the event queue
void Simulation::handle_io_burst_completed(const Event *event) {
	// Set the thread to ready
	event->thread->set_ready(event->get_time());

	// Put the thread back in the scheduler's ready queue
	scheduler->enqueue(event, event->thread);

	// If the processor is idle, invoke the scheduler right now
	if (!currentThread)
		events.push(new Event(Event::Type::DISPATCHER_INVOKED, event->get_time(), NULL, NULL));

	// Print a state transition, if so requested by the user
	logger->print_state_transition(event, Thread::State::BLOCKED, Thread::State::READY);
}

// handle_thread_completed handles the THREAD_COMPLETED event in the event queue
void Simulation::handle_thread_completed(const Event *event) {
	// SEt the thread to finished
	event->thread->set_finished(event->get_time());

	// Update the previous and currently running threads
	previousThread = currentThread;
	currentThread = NULL;

	// If the processor is idle, invoke the scheduler right now
	if (!currentThread)
		events.push(new Event(Event::Type::DISPATCHER_INVOKED, event->get_time(), NULL, NULL));

	// Print a state transition, if so requested by the user
	logger->print_state_transition(event, Thread::State::RUNNING, Thread::State::EXIT);
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
	// Update start time of simulation
	if (event->get_time() < start_time)
		start_time = event->get_time();

	// Update end time of simulation
	if (event->get_time() > end_time)
		end_time = event->get_time();
}

// printStatistics prints required statistics from the simulation
void Simulation::printStatistics() {
	// Instantiate a SystemStats struct to send to the logger instance
	SystemStats systemstats = {0};

	// Iterate through processes and print associated statistics
	for (std::map<int, Process*>::iterator iter = pidMap.begin(); iter != pidMap.end(); iter++) {
		// Print per-thread details if user requests them
		logger->print_process_details(iter->second);

		/* Fill information into the SystemStats struct */

		// Get process type, and const reference to thread vector
		Process::Type type = iter->second->get_type();
		const std::vector<Thread*> threads = iter->second->getThreads();

		// Iterate through threads of a process
		for (int i = 0; i < (int) threads.size(); i++) {
			// Update thread counts
			int n = ++systemstats.count[type];

			// Dynamically adjust response and turnaround averages
			systemstats.response[type] = systemstats.response[type] * (n - 1) / (n) + threads[i]->get_response_time() * 1.0 / n;
			systemstats.turnaround[type] = systemstats.turnaround[type] * (n - 1) / (n) + threads[i]->get_turnaround_time() * 1.0 / n;

			// Update service and io time counters
			systemstats.service += threads[i]->get_service_time();
			systemstats.io += threads[i]->get_io_time();
		}


	}

	// Update overall SystemStats metrics
	systemstats.elapsed = end_time - start_time;
	systemstats.dispatch = dispatch_time;
	systemstats.idle = systemstats.elapsed  - systemstats.dispatch - systemstats.service;

	// Calculate CPU utilization and efficiency
	systemstats.utilization = (100.0 * (systemstats.dispatch + systemstats.service)) / systemstats.elapsed;
	systemstats.efficiency = (100.0 * systemstats.service) / systemstats.elapsed;

	// Print overall system statistics
	logger->print_statistics(systemstats);
}