//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <string.h>
#include <stdlib.h>

#include "process.h"


Process* process_init(int PID, int priority, int start_time, int length, int* bursts, char* name) {
    Process* process = malloc(sizeof(Process));

    process -> PID = PID;
    process -> priority = priority;
    process -> start_time = start_time;
    process -> finish_time = -1;
    process -> length = length;
    process -> bursts = bursts;
    process -> actual_burst = 0;
    process -> status = NOINIT;
    strcpy(process -> name, name);

    return process;
}

void process_destroy(Process* process) {
    free(process);
}

void ready_process(Process* process) {
    process -> status = READY;
}

void run_process(Process* process) {
    process -> status = RUNNING;
}

void pause_process(Process* process) {
    process -> status = WAITING;
}

void stop_process(Process* process) {
    process -> status = FINISHED;
}