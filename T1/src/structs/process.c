//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "process.h"


Process* process_init(int PID, int priority, int start_time, int length, int* bursts, char* name) {
    Process* process = malloc(sizeof(Process));

    process -> PID = PID;
    process -> priority = priority;
    process -> start_time = start_time; // Comienzo de ejecucion
    process -> length = length; // Cantidad de bursts
    process -> bursts = bursts; // Lista de bursts
    process -> finish_time = -1; // Tiempo en que termino
    process -> actual_burst = 0; // Burst actual del proceso
    process -> status = READY;
    process -> runs = 0; // Cantidad de veces que fue llamado por la CPU
    process -> interruptions = 0; // Cantidad de veces que fue interrumpido
    process -> turnaround_time = -1;
    process -> response_time = -1;
    process -> waiting_time = 0;
    strcpy(process -> name, name);

    return process;
}

void process_destroy(Process* process) {
    free(process);
}

int work(Process* process) {
    process -> bursts[process -> actual_burst] -= 1;

    if (process -> actual_burst == 2 * process -> length - 2 && process -> bursts[process -> actual_burst] == 0) {
        return 2;
    }

    if (process -> bursts[process -> actual_burst] == 0) {
        process -> actual_burst ++;
        return 1;
    }

    return 0;
}

void set_state(Process* process, states state) {
    printf("Proceso %s cambio de %i a %i\n", process -> name, process -> status, state);
    process -> status = state;
}

void set_response_time(Process* process, int time) {
    process -> response_time = time - process -> start_time;
}

void increment_waiting_time(Process* process) {
    process -> waiting_time += 1;
}

void interrupt(Process* process) {
    process -> interruptions += 1;
}