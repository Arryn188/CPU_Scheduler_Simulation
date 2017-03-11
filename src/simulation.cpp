#include "simulation.h"

// Constructor
Simulation::Simulation(Scheduler *scheduler, Print_opts *print_opts) {
	this->scheduler = scheduler;
	this->print_opts = print_opts;
}

void Simulation::run(std::string &file) {
	// Populate the event queue
	parseFile(file);

	// Run event loop while event queue isn't empty
	while (!events.empty()) {
		// Get top element of queue
		const Event* event = events.top();
		events.pop();

		//for deliverable 1 only
		/*
        std::cout << "At time " <<event->get_time() << ":" << std::endl;
        std::cout << event->get_type_name(event->get_type())<< std::endl;
        std::cout << "thread " << event->thread->get_id() + 1 << " in process " << event->thread->process->get_pid()<< " "<<event->thread->process->get_type_name(event->thread->process->get_type()) <<std::endl;
        std::cout << std::endl;
		*/

        updateStartandEnd(event);

        switch(event -> get_type()){
            case Event:: THREAD_ARRIVED: {
                handle_thread_arrived(event);
                break;
            }

            case Event:: DISPATCHER_INVOKED:{
                handle_dispatcher_invoked(event);
                break;
            }

            case Event:: PROCESS_DISPATCH_COMPLETED:{
                handle_dispatch_completed(event):
                break;
            }

            case Event:: THREAD_PREEMPTED:{
                handle_thread_preempted(event);
                break;
            }

            case Event:: CPU_BURST_COMPLETED:{
                handle_cpu_burst_completed(event);
                break;
            }

            case Event:: IO_BURST_COMPLETED:{
                handle_io_burst_completed(event);
                break;
            }

            case Event:: THREAD_DISPATCH_COMPLETED:{
                handle_dispatch_completed(event);
                break;
            }

            case Event:: THREAD_COMPLETED:{
                handle_thread_completed(event);
            }


        }

		delete event;
	}

    printStatistics();
}

void Simulation::handle_thread_arrived(const Event *event) {
    
}

void Simulation::handle_dispatcher_invoked(const Event *event) {

}

void Simulation::handle_dispatch_completed(const Event *event) {

}

void Simulation::handle_thread_preempted(const Event *event) {

}

void Simulation::handle_cpu_burst_completed(const Event *event) {

}

void Simulation::handle_io_burst_completed(const Event *event) {

}

void Simulation::handle_thread_completed(const Event *event) {

}

void Simulation::parseFile(std::string &file) {
	std::ifstream simFile(file);

	if (!simFile) {
		std::cerr << "Cannot open: " << file << std::endl;
		exit(-2);
	}

	int num_processes = -1;
    int pid = 0;
    int type = 0;
    int num_threads = 0;
    int arrival_time = 0;
    int num_bursts = 0;
    int burst_length = 0;

	simFile >> num_processes >> thread_switch_overhead >> process_switch_overhead;

	for (int i = 0; i < num_processes; i++) {
		simFile >> pid >> type >> num_threads;

		Process* process = new Process(pid, static_cast<Process::Type>(type));

		for (int j = 0; j < num_threads; j++) {
			simFile >> arrival_time >> num_bursts;

			Thread* thread = new Thread(j, process, arrival_time);

			for (int k = 0; k < num_bursts; k++) {

				for (int l = 0; l < 2 && k + l < num_bursts; l++) {

					simFile >> burst_length;
					Burst *burst = new Burst(static_cast<Burst::Type>(l), burst_length);

					thread->pushBurst(burst);
				}
			}

			process->pushThread(thread);

			Event* event = new Event(Event::Type::THREAD_ARRIVED, arrival_time, thread, NULL);

			events.push(event);
		}

		pidMap[pid] = process;
	}
}

void Simulation::updateStartandEnd(const Event *event) {

}

void Simulation::printStatistics() {

}