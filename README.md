Project 3  
=========  

1. Ryan Hunt  

2. 	burst.h 					-- Defines a burst object, which holds a length and a type (CPU/IO).  

	customscheduler{.h, .cpp} 	-- Defines a custom scheduling algorithm which is described in more detail below.  

	decision.h 					-- Defines a decision object, which holds a pointer to a thread, an optional preemptive time slice length, and an optional decision description.  

	event{.h, .cpp} 			-- Defines an event object, which holds an event type, a pointer to the associated thread, a pointer to an optional decision object, and a time of occurrence.  

	fcfsscheduler{.h, .cpp} 	-- Defines an implementation of an FCFS scheduling algorithm which draws upon the implementation described in the textbook.  

	logger{.h, .cpp} 			-- Defines a logging class which can print state transitions, process details, and overall statistics.  

	main.cpp 					-- Initializes the simulation, reads command line options from the user, and uses the logger to print overall simulation statistics.  

	priorityscheduler{.h, .cpp} -- Defines an implementation of a Priority scheduling algorithm which draws upon the implementation described in the textbook.  

	process{.h, .cpp} 			-- Defines a process object, which holds a PID, a process priority, and a vector of child threads.  

	rrscheduler{.h, .cpp} 		-- Defines an implementation of a Round Robin scheduling algorithm which draws upon the implementation described in the textbook.  

	scheduler.h 				-- Defines an interface for all other schedulers to implement, such that schedulers can easily be interchanged.  

	simulation{.h, .cpp} 		-- Defines the class responsible for managing the simulation using the next-event simulation model. Also reads the simulation file and prints state transitions.  

	systemstats.h 				-- Defines a struct that facilitates the printing of simulation statistics for the logger class.  

	thread{.h, .cpp} 			-- Defines a thread object, which holds an ID, a pointer to a parent process, and a queue of CPU/IO bursts. Thread objects are responsible for keeping track of their own performance statistics.  


3. The user can pass an additional flag, (--color or -c) to receive a colorized output of the simulation. This requires a terminal with color support.  

4. 15 hours  

5.	The custom CPU scheduling algorithm implemented in this project models the Multilevel Feedback Queue Algorithm described in the textbook but offers several improvements over the original version. The custom algorithm natively handles and respects processes with different priorities with an initial seeding algorithm. Higher priority processes are initially placed in higher priority queues, while lower priority processes are initially placed in lower priority queues. This offers a better performance metric for higher priority processes when compared directly with a Multilevel Feedback Queue Algorithm. This algorithm attempts to optimize response and turnaround time for shorter CPU-bound processes and for high priority processes. As a result, the custom algorithm offers the best performance for the short threads out of any of the algorithms implemented in this project.  

	The custom scheduling algorithm uses preemption to improve the "fairness" of scheduling on the CPU. Threads in the highest level queue receive the longest time slice of any queue, while lower queues receive a substantially shorter time slice. This helps
to keep CPU-bound threads from dominating the CPU and keeping shorter threads from finishing. This represents a fair algorithm because longer jobs should be expected to wait longer than shorter ones. Using a metric such as normalized turnaround time, this point becomes very clear. The amount of time a process waits is not independent from the length of the process. A wait of 50 time units is very different in the context of a long process than it is in the context of a short process.	

	The implemented custom scheduing algorithm uses six queues of decreasing priority, with SYSTEM threads starting in queue #1, and having the potential to fall all the way to queue #6. Processes of other priorities start in lower queues, and may also fall 
to lower priority queues if they are preempted. Thus, processes are penalized for preemption, because it signifies the process' desire for an unfair share of the CPU. As a result, starvation is technically possible with an unlimited stream of shorter processes. CPU-intensive processes in low priority queues will only be chosen after higher priority queues have been emptied.  
  
	Overall, this algorithm is quite fair. It strikes a good balance between priority and normalized turnaround time, the two major metrics of a strong scheduling algorithm.  
