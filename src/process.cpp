#include "process.h"

// Out-of-class Static Initializer
std::string Process::TypeArray[] = {
	"SYSTEM", "INTERACTIVE", "NORMAL", "BATCH"
};

// Paramterized Constructor
Process::Process(int pid, Type type) {
	this->pid = pid;
	this->type = type;
}

// Returns the process pid
int Process::get_pid() {
	return pid;
}

// Returns the process type
Process::Type Process::get_type() {
	return type;
}

// Returns a string containing the process type
std::string Process::get_type_name(Type type) {
	return TypeArray[type];
}

// Adds a thread to the thread vector
void Process::pushThread(Thread *thread) {
	threads.push_back(thread);
}

// Returns a const iterator to the thread vector
const std::vector<Thread*>& Process::getThreads() {
	return threads;
}