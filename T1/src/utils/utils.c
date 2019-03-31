//
// Created by Felipe Dominguez Claro on 2019-03-29.
//

#include <stdio.h>

#include "utils.h"
#include "../structs/process.h"

// Ordena la lista de procesos segun el orden de comienzo
int time_compare(const void *pointer1, const void *pointer2) {
    const Process *p1 = *(Process *const *) pointer1;
    const Process *p2 = *(Process *const *) pointer2;

    return p1 -> start_time - p2 -> start_time;
}

// Imprime la lista de procesos
void print_processes(Process **processes, int n) {
    for (int i=0; i<n; i++) {
        printf("Proceso %i, start:%i, priority:%i\n", processes[i] -> PID, processes[i] -> start_time,
                processes[i] -> priority);
    }
}