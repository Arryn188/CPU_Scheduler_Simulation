#include "logger.h"

void Logger::print_state_transition(const Event* event, Thread::State before_state, Thread::State after_state) {
}


void Logger::print_dispatch_invoked_message(const Event* event, Thread *thread, std::string message) {
}

void Logger::print_process_details(Process* process) const {

}

void Logger::print_statistics(SystemStats stats) const {
}
