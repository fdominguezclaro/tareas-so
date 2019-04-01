//
// Created by Felipe Dominguez Claro on 2019-03-29.
//

#include "../structs/process.h"
#include "../structs/queue.h"


int time_compare(const void *pointer1, const void *pointer2);

int PID_compare(const void *pointer1, const void *pointer2);

void print_processes(Process **processes, int n);

void write_statistics(Process** processes, int n);

void print_queue_status(Queue* queue, char* actual_process);

void set_statistics(Process* process);

void free_processes(Process** processes, int n);