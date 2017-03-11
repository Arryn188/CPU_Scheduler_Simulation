//
// Created by remay on 3/9/17.
//
#ifndef THREAD_H
#define THREAD_H

#include "burst.h"
#include "process.h"
#include <queue>
#include <cassert>

using namespace std;

class Process;

class Thread {
public:
	enum State { NEW, READY, RUNNING, BLOCKED, EXIT };
	Thread(int id, Process* parent, unsigned int arrival_time);
	int get_id();
	unsigned int get_arrival_time();
	unsigned int get_service_time();
	unsigned int get_io_time();
	unsigned int get_end_time();
	unsigned int get_response_time();
	unsigned int get_turnaround_time();
	static std::string get_state_name(State state);
	void set_ready(unsigned int time);
	unsigned int set_running(unsigned int time);
	unsigned int set_blocked(unsigned int time);
	void set_finished(unsigned int time);
	void pushBurst(Burst* burst);
	Process* process;

private:
	int id;
	queue<Burst*> bursts;
	State curr_state = NEW;
	State prev_state = NEW;
	int arr_time = -1;
	int s_time = -1;
	int e_time = -1;
	unsigned int service_time = 0;
	unsigned int io_time = 0;
    int last_state_change = -1;
    static string StateArray[5];
    void do_update(unsigned int time);
};

#endif
