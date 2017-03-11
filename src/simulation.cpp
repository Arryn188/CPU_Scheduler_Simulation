//
// Created by remay on 3/9/17.
//
#include "simulation.h"

using namespace std;

Simulation::Simulation(Scheduler *scheduler, Print_opts *print_opts) {
	this->scheduler = scheduler;
	this->print_opts = print_opts;
}

void Simulation::run(std::string &file) {
    fileIn(file);

	while (!events.empty()) {
		const Event* event = events.top();
		events.pop();

		//for deliverable 1 only
		/*
        std::cout << "At time " <<event->get_time() << ":" << std::endl;
        std::cout << event->get_type_name(event->get_type())<< std::endl;
        std::cout << "thread " << event->thread->get_id() + 1 << " in process " << event->thread->process->get_pid()<< " "<<event->thread->process->get_type_name(event->thread->process->get_type()) <<std::endl;
        std::cout << std::endl;
		*/

        updateTimes(event);

        switch(event -> get_type()){
            case Event::THREAD_ARRIVED: {
                thread_arrived_update(event);
                break;
            }

            case Event::DISPATCHER_INVOKED: {
                dispatcher_invoked_update(event);
                break;
            }

            case Event::THREAD_DISPATCH_COMPLETED:

            case Event::PROCESS_DISPATCH_COMPLETED: {
                dispatch_complete_update(event);
                break;
            }

            case Event::THREAD_PREEMPTED: {
                thread_preempted_update(event);
                break;
            }

            case Event::CPU_BURST_COMPLETED: {
                cpu_burst_complete_update(event);
                break;
            }

            case Event::IO_BURST_COMPLETED: {
                io_burst_complete_update(event);
                break;
            }

            case Event::THREAD_COMPLETED: {
                thread_complete_update(event);
                break;
            }
        }

		delete event;
	}

    printStats();
}

void Simulation::thread_arrived_update(const Event *event) {
    event -> thread -> set_ready(event->get_time());
    scheduler->enqueue(event, event->thread);

    if(!currentThread){
        events.push(new Event(Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL));
    }
    print_opts->transitions(event, Thread::State::NEW, Thread::State::READY);
}

void Simulation::dispatcher_invoked_update(const Event *event) {
    Decision *decision = scheduler->get_next_thread(event);
    if(!decision->thread){
        return;
    }

    currentThread = decision -> thread;

    if(currentThread && previousThread && previousThread->process == currentThread->process){
        events.push(new Event(Event::Type::THREAD_DISPATCH_COMPLETED, event->get_time() + thread_switch_overhead, decision->thread, decision));
        dispatch_time += thread_switch_overhead;
    }
    else{
        events.push(new Event(Event::Type::PROCESS_DISPATCH_COMPLETED, event->get_time() + process_switch_overhead, decision ->thread, decision));
        dispatch_time += process_switch_overhead;
    }

    print_opts->dispatcher(event, decision->thread, decision->get_explanation());
}

void Simulation::dispatch_complete_update(const Event *event) {
    int burst_length = event -> thread -> set_running(event ->get_time());
    int sl_length = event -> get_decision() -> get_slice();

    if (sl_length != -1 && sl_length < burst_length){
        events.push(new Event(Event::Type::THREAD_PREEMPTED, event -> get_time() + sl_length, event -> thread, NULL));
    }
    else{
        events.push(new Event(Event::Type::CPU_BURST_COMPLETED, event -> get_time() + burst_length, event -> thread, NULL));
    }

    print_opts->transitions(event, Thread::State::READY, Thread::State::RUNNING);
}

void Simulation::thread_preempted_update(const Event *event) {
    event->thread->set_ready(event->get_time());
    scheduler->enqueue(event, event->thread);
    previousThread = currentThread;
    currentThread = NULL;

    if (!currentThread) {
        events.push(new Event(Event::Type::DISPATCHER_INVOKED, event->get_time(), NULL, NULL));
    }

    print_opts->transitions(event, Thread::State::RUNNING, Thread::State::READY);
}

void Simulation::cpu_burst_complete_update(const Event *event) {
    int burst_length = event -> thread -> set_blocked(event -> get_time());

    if (burst_length == -1){
        events.push(new Event(Event::Type::THREAD_COMPLETED, event -> get_time(), event -> thread, NULL));
        return;
    }

    previousThread = currentThread;
    currentThread = NULL;

    events.push(new Event(Event::Type::IO_BURST_COMPLETED, event -> get_time() + burst_length, event -> thread, NULL));

    if (!currentThread){
        events.push(new Event(Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL));
    }

    print_opts->transitions(event, Thread::State::RUNNING, Thread::State::BLOCKED);
}

void Simulation::io_burst_complete_update(const Event *event) {
    event -> thread -> set_ready(event -> get_time());
    scheduler -> enqueue(event, event -> thread);

    if(!currentThread){
        events.push(new Event(Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL));
    }

    print_opts->transitions(event, Thread::State::BLOCKED, Thread::State::READY);
}

void Simulation::thread_complete_update(const Event *event) {
    event -> thread -> set_finished(event -> get_time());

    previousThread = currentThread;
    currentThread = NULL;

    if(!currentThread){
        events.push(new Event(Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL));
    }

    print_opts->transitions(event, Thread::State::RUNNING, Thread::State::EXIT);
}

void Simulation::fileIn(std::string &file) {
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

void Simulation::updateTimes(const Event *event) {
    if(event -> get_time() > end_time){
        end_time = event->get_time();
    }

    if(event ->get_time() < start_time){
        start_time = event -> get_time();
    }
}

void Simulation::printStats() {
    Sys_statistics sys_stats = {0};

    for(map <int, Process*>::iterator iterate = pidMap.begin(); iterate != pidMap.end(); iterate++){
        print_opts->processes(iterate->second);
        Process::Type type = iterate -> second -> get_type();
        const std::vector<Thread*> threads = iterate -> second -> getThreads();

        for(int i = 0; i < (int) threads.size(); i++){
            int n = ++sys_stats.count_threads[type];

            sys_stats.response_time[type] = sys_stats.response_time[type] * (n -1) / (n) + threads[i] -> get_response_time() * 1.0 / n;
            sys_stats.tat_time[type] = sys_stats.tat_time[type] * (n - 1) / (n) + threads[i] -> get_turnaround_time() * 1.0 / n;
            sys_stats.service_time += threads[i] -> get_service_time();
            sys_stats.io_time += threads[i] -> get_io_time();
        }
    }

    sys_stats.elapsed_time = end_time - start_time;
    sys_stats.dispatch_time = dispatch_time;
    sys_stats.idle_time = sys_stats.elapsed_time - sys_stats.dispatch_time - sys_stats.service_time;
    sys_stats.cpu_util = (100.0 *(sys_stats.dispatch_time + sys_stats.service_time)) /sys_stats.elapsed_time;
    sys_stats.cpu_efficiency = (100.0 * sys_stats.service_time) / sys_stats.elapsed_time;

    print_opts->stats(sys_stats);
}