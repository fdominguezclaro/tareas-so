//
// Created by Felipe Dominguez Claro on 2019-03-29.
//

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "../structs/process.h"
#include "../structs/queue.h"


// Ordena la lista de procesos segun el orden de comienzo
int time_compare(const void *pointer1, const void *pointer2) {
    const Process *p1 = *(Process *const *) pointer1;
    const Process *p2 = *(Process *const *) pointer2;

    return p1 -> start_time - p2 -> start_time;
}

// Ordena la lista de procesos segun el orden de comienzo
int PID_compare(const void *pointer1, const void *pointer2) {
    const Process *p1 = *(Process *const *) pointer1;
    const Process *p2 = *(Process *const *) pointer2;

    return p1 -> PID - p2 -> PID;
}

// Imprime la lista de procesos
void print_processes(Process** processes, int n) {
    for (int i=0; i<n; i++) {
        printf("Proceso %i:%s, start:%i, priority:%i\n", processes[i] -> PID, processes[i] -> name,
                processes[i] -> start_time, processes[i] -> priority);
    }
}

void write_statistics(Process** processes, int n) {
    FILE* file;
    file = fopen ("out.csv","w");
    for (int i=0; i<n; i++) {
        set_statistics(processes[i]);
        fprintf(file, "%s,%i,%i,%i,%i,%i\n", processes[i] -> name, processes[i] -> runs, processes[i] -> interruptions,
                processes[i] -> turnaround_time, processes[i] -> response_time, processes[i] -> waiting_time);
    }
    fclose (file);
}

void print_queue_status(Queue* queue, char* actual_process) {
    printf("Hay %i procesos, actual: %s\n", queue -> count, actual_process);
    // Imprime la lista de procesos en cola
    Node* actual = queue -> start;
    for (int i = 0; i < queue -> count; i++) {
        // printf("DEBUGGER\n");
        printf("%s: estado: %i, burst actual: %i, remaining: %i, priority:%i, waiting: %i\n", actual -> process -> name,
                actual -> process -> status, actual -> process -> actual_burst,
                actual -> process -> bursts[actual -> process -> actual_burst], actual -> process -> priority,
                actual -> process -> waiting_time);
        actual = actual -> next;
    }
}

void set_statistics(Process* process) {
    process -> turnaround_time = process -> finish_time - process -> start_time + 1;
}

void free_processes(Process** processes, int n) {
    for (int i = 0; i < n; i++) {
        free(processes[i] -> bursts);
        free(processes[i]);
    }
    free(processes);
}