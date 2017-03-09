#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "thread.h"
#include <vector>

#define THREAD_TYPES 4

// Forward Declaration for Thread Class
class Thread;

class Process {
public:
	// Process Type Enumeration (including priority)
	enum Type {
		SYSTEM, INTERACTIVE, NORMAL, BATCH
	};

	// Constructor
	Process(int pid, Type type);

	// Returns pid
	int get_pid();

	// Returns type
	Type get_type();

	// Returns type as a string
	static std::string get_type_name(Type type);

	// Push a thread into the thread vector
	void pushThread(Thread* thread);

	// Returns a const iterator to the thread vector
	const std::vector<Thread*>& getThreads();

private:
	// PID
	int pid;

	// Process Type and Priority
	Type type;

	// Associated Threads
	std::vector<Thread*> threads;

	// Corresponding Name Array
	static std::string TypeArray[THREAD_TYPES];
};

#endif
