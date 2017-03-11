//
// Created by remay on 3/9/17.
//
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "thread.h"
#include <vector>


using namespace std;

class Thread;

class Process {
public:

	enum Type { SYSTEM, INTERACTIVE, NORMAL, BATCH };
	Process(int pid, Type type);
	int get_pid();
	Type get_type();
	static string get_type_name(Type type);
	void pushThread(Thread* thread);
	const vector<Thread*>& getThreads();

private:
	int pid;
	Type type;
	vector<Thread*> threads;
	static string TypeArray[4];
};

#endif
