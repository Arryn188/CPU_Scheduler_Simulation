//
// Created by remay on 3/11/17.
//

#ifndef SCHED_SYS_STATISTICS_H
#define SCHED_SYS_STATISTICS_H

struct Sys_statistics{
    int count_threads[4];
    double response_time[4];
    double tat_time[4];
    int elapsed_time;
    int service_time;
    int io_time;
    int dispatch_time;
    int idle_time;
    double cpu_util;
    double cpu_efficiency;
};

#endif //SCHED_SYS_STATISTICS_H
