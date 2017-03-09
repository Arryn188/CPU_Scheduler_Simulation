#include "logger.h"

// If verbose is true, prints a state-transition message
void Logger::print_state_transition(const Event* event, Thread::State before_state, Thread::State after_state) {
	if (verbose) {
		std::cout << (color ? YEL : "") << "At time " << event->get_time() << ":" << (color ? RST : "") << std::endl
			 << "\t" << Event::get_type_name(event->get_type()) << std::endl
			 << "\t" << "Thread " << (color ? GRN : "") << event->thread->get_id() << (color ? RST : "") << " in process " << (color ? GRN : "") << event->thread->process->get_pid() << (color ? RST : "")
			 << (color ? YEL : "") << " [" << Process::get_type_name(event->thread->process->get_type()) << "]" << (color ? RST : "") << std::endl
			 << "\t" << "Transitioned from " << (color ? YEL : "") << Thread::get_state_name(before_state) << (color ? RST : "") << " to " << (color ? YEL : "")
			 << Thread::get_state_name(after_state) << (color ? RST : "") << std::endl << std::endl;
	}
}

// If verbose is true, prints a dispatcher invoked custom message
void Logger::print_dispatch_invoked_message(const Event* event, Thread *thread, std::string message) {
	if (verbose) {
		std::cout << (color ? YEL : "") << "At time " << event->get_time() << ":" << (color ? RST : "") << std::endl
			 << "\t" << Event::get_type_name(event->get_type()) << std::endl
			 << "\t" << "Thread " << (color ? GRN : "") << thread->get_id() << (color ? RST : "") << " in process " << (color ? GRN : "") << thread->process->get_pid() << (color ? RST : "")
			 << (color ? YEL : "") << " [" << Process::get_type_name(thread->process->get_type()) << "]" << (color ? RST : "") << std::endl << "\t" << message << std::endl << std::endl;
	}
}

// if per_thread is true, prints information about a process and its threads
void Logger::print_process_details(Process* process) const {
	if (per_thread) {
		std::cout << (color ? YEL : "") << "Process " << process->get_pid() << " [" << Process::get_type_name(process->get_type()) << "]:" << (color ? RST : "") << std::endl;

		// Get a const reference to the process' child threads
		const std::vector<Thread*> threads = process->getThreads();
		for (int i = 0; i < (int) threads.size(); i++) {
			std::cout << std::left << "\tThread " << (color ? GRN : "") << threads[i]->get_id() << (color ? RST : "") << ": " << "  ARR: " << (color ? GRN : "") << std::setw(6) << threads[i]->get_arrival_time() << 
			(color ? RST : "") << "CPU: " << (color ? GRN : "") << std::setw(6) << threads[i]->get_service_time() << (color ? RST : "") << "I/O: " << (color ? GRN : "") << std::setw(6) << threads[i]->get_io_time() << 
			(color ? RST : "") << "TRT: " << (color ? GRN : "") << std::setw(6) << threads[i]->get_turnaround_time() << (color ? RST : "") << "END: " << (color ? GRN : "") << std::setw(6) << threads[i]->get_end_time() << 
			(color ? RST : "") << std::endl;
		}

		std::cout << std::endl;
	}
}

// Prints overall simulation statistics
void Logger::print_statistics(SystemStats stats) const {
	std::cout << (color ? YEL : "") << "SIMULATION COMPLETED!" << (color ? RST : "") << std::endl << std::endl;

	// Iterate through each type of thread (SYSTEM, INTERACTIVE, NORMAL, BATCH) and print statistics
	for (int i = 0; i < 4; i++) {
		std::cout << (color ? YEL : "") << Process::get_type_name( (Process::Type) i) << " THREADS:" << (color ? RST : "") << std::endl
				  << "\t" << "Total count: " << "\t\t" << (color ? GRN : "") << std::right << std::setw(6) << stats.count[i] << (color ? RST : "") << std::endl
				  << "\t" << "Avg response time:" << "\t" << (color ? GRN : "") << std::right << std::setw(6) << std::fixed << std::setprecision(2) << stats.response[i] << (color ? RST : "") << std::endl
				  << "\t" << "Avg turnaround time:" << "\t" << (color ? GRN : "") << std::right << std::setw(6) << std::fixed << std::setprecision(2) << stats.turnaround[i] << (color ? RST : "") << std::endl << std::endl;
	}

	std::cout << (color ? YEL : "") << "Total elapsed time:" << (color ? RST : "") << "\t\t" << (color ? GRN : "") << std::right << std::setw(6) << stats.elapsed << (color ? RST : "") << std::endl
			  << (color ? YEL : "") << "Total service time:" << (color ? RST : "") << "\t\t" << (color ? GRN : "") << std::right << std::setw(6) << stats.service << (color ? RST : "") << std::endl
			  << (color ? YEL : "") << "Total I/O time:" << (color ? RST : "") << "\t\t\t" << (color ? GRN : "") << std::right << std::setw(6) << stats.io << (color ? RST : "") << std::endl
			  << (color ? YEL : "") << "Total dispatch time:" << (color ? RST : "") << "\t\t" << (color ? GRN : "") << std::right << std::setw(6) << stats.dispatch << (color ? RST : "") << std::endl
			  << (color ? YEL : "") << "Total idle time:" << (color ? RST : "") << "\t\t" << (color ? GRN : "") << std::right << std::setw(6) << stats.idle << (color ? RST : "") << std::endl << std::endl

			  << (color ? YEL : "") << "CPU utilization:" << (color ? RST : "") << "\t\t" << (color ? GRN : "") << std::right << std::setw(5) << stats.utilization << "%" << (color ? RST : "") << std::endl
			  << (color ? YEL : "") << "CPU efficiency:" << (color ? RST : "") << "\t\t\t" << (color ? GRN : "") << std::right << std::setw(5) << stats.efficiency << "%" << (color ? RST : "") << std::endl;
}
