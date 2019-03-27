//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <string.h>
#include <stdlib.h>

#include "process.h"

static enum states{READY, RUNNING, WAITING, FINISHED};

Process* process_init(int PID, int priority, int start_time, int length, char* name) {
    Process* process = malloc(sizeof(Process));

    process -> PID = PID;
    process -> priority = priority;
    process -> start_time = start_time;
    process -> finish_time = -1;
    process -> length = length;
    strcpy(process -> name, name);
    strcpy(process -> status, "NOINIT");

    return process;
}

void process_destroy(Process* process) {
    free(process);
}