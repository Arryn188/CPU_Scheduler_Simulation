Rebecca May
CSCI 403 Project 2: CPU Scheduling Simulator

The goal of this project is to develop a CPU scheduling simulation that will complete the execution of a group of multi-threaded processes.
It supports several different scheduling algorithms such that the user can specify which one to use via a command line flag.
It also calculates and displays many of the performance criteria by which scheduling algorithms are typically judged.
It has been implemented in C++ and assumes a single CPU and an infinite number of I/O devices.

-------------------------------------------- Files included in this project --------------------------------------------

burst{.h} : Defines the burst object which has a length and a type{CPU/IO}

customscheduler{.h/.cpp} : Defines the custom scheduling algorithm. Details on this algorithm can be found below

decision{.h} : Defines the decision object which holds a time slice and a string for the explaination of the scheduling
    decision as well as a pointer to the current thread

event{.h/.cpp} : Defines the event object. Each event has a type {THREAD_ARRIVED, THREAD_DISPATCH_COMPLETED,
    PROCESS_DISPATCH_COMPLETED, CPU_BURST_COMPLETED, IO_BURST_COMPLETED, THREAD_COMPLETED, THREAD_PREEMPTED,
    DISPATCHER_INVOKED}, a time, a decision, and an associated thread

fcfsscheduler{.h/.cpp} : Defines the first-come first-serve scheduling algorithm that is described in the textbook

main{.cpp} : reads command line flags, starts the entire simulation with the selected options from the given flags and
    prints the selected system statistics based on the selected flags

prints_opts{.h/.cpp} : Prints the system statistics chosen by the user through certain flags. Information can be
    displayed based on transitions of thread states, based on events occuring, by each process, or all system statistics
    after the simulation

priorityscheduler{.h/.cpp} : Defines the priority scheduling algorithm that is described on the textbook by building 4
    queues which threads are fed into based on their priority and then picking from higher priority queues first

process{.h/.cpp} : Defines the process object which contains a process id, a type{SYSTEM, INTERACTIVE, NORMAL, BATCH},
    and a vector of threads

rrscheduler{.h/.cpp} : Defines the round robin scheduling algorithm that is described in the textbook by allowing a time
    quantum of 3 for each process, switching from process to process after each timeslice

scheduler{.h} : Defines an interface for all other scheduling options

simulation{.h/.cpp} : Defines the overall simulation using the next-event method and will parse the simulation file. At
    the end of the simulation it will also print state transitions using print_opts

sys_statistics{.h/.cpp} : contains the declarations for all system statistics desired such as thread counts, response
    time, turnaround time, elapsed time, service time, IO time, dispatch time, idle time, cpu utilization, and cpu
    efficiency that are calculated during each simulation

thread{.h/.cpp} : Defines the thread object that has an id, a queue of associated bursts, current state, previous state,
    arrival time, service time, elapsed time, and IO time. The possible thread states are {NEW, READY, RUNNING, BLOCKED,
    EXIT}

------------------------------------------------------------------------------------------------------------------------


There are no unusual or interesting features added to this program other than those required

I spent approximately 25 hours on this project total


----------------------------------- A short essay on the custom scheduling algorithm -----------------------------------
The custom scheduling algorithm that I have decided to implement in this project is an upgraded Multilevel Feedback
Queue. Initially, higher priority processes are placed in the highest of the 6 queues (1) and lower priority processes are
placed in the lower queues (queue 6 is the lowest priority). My custom algorithm has been implemented in order to optimize
turnaround time as well as response time for CPU bound processes. My algorithm uses preemption to create a more fair
algorithm than the average multilevel feedback queue algorithm. The highest level queued threads recieve the greatest 
amount of time on the processor while lower level queues have a shorter amount of time. This is so that CPU-bound processes
do not dominate the CPU by making shorter processes not able to finish. When originally queued, threads of the SYSTEM type
are given a higher priority but can fall farther down to other queues if they are preempted. This means that processes that
are preemptive are penalized. However, processes may be starved in there are many shorter processes in the queue because
CPU intensive processes in lower queues are only chosen if higher queues are empty. However, this scheduling algorithm is 
a fair algorithm because it balances turnaround time and priority.
------------------------------------------------------------------------------------------------------------------------
