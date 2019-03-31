//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <stdlib.h>
#include <stdio.h>

#include "../structs/queue.h"
#include "../structs/process.h"
#include "../utils/utils.h"



int main(int argc, char *argv[]) {
    printf("T1 SO FDOM\n\n");

    // Definimos el quantum
    int quantum;
    if (argc == 4) {
        sscanf(argv[3], "%i", &quantum);
    } else {
        quantum = 3;
    }

    // Inicializamos algunas variables
    int PID = 0;
    int size = 40; // Seteo inicialmente en 40 la cantidad de procesos soportada
    int time = 0;
    int running = 1;
    int finished = 0;
    int n_proccess = 0;
    int initialized = 0;

    Process** processes = malloc(sizeof(Process*) * size);

    // Leemos el archivo
    FILE* fp;
    fp = fopen(argv[1], "r"); // read mode

    if (fp == NULL) {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }

    printf("%s opened succesfully\n\n", argv[1]);

    // Creamos los procesos
    char* name = malloc(sizeof(char) * 257);
    int priority;
    int start_time;
    int length;

    // El while fue una modificacion obtenida de: https://overiq.com/c-programming-101/fscanf-function-in-c/
    while( fscanf(fp, "%s %i %i %i", name, &priority, &start_time, &length) == 4 ) {
        // Creo un array con las rafagas
        int* bursts = malloc(sizeof(int) * (length * 2 - 1));
        // Leo las rafagas del proceso
        for (int i = 0; i < (2 * length - 1); i++) {
            fscanf(fp, "%i", &bursts[i]);
        }

        Process* process = process_init(PID, priority, start_time, length, bursts, name);
        processes[PID] = process;
        printf("Proceso %i:%s agregado, prioridad: %i\n", PID, name, priority);
        ++n_proccess;
        ++PID;
    }

    // Ordeno los procesos segun tiempo de llegada para hacer un manejo eficiente de ciclos despues
    qsort(processes, n_proccess, sizeof(Process *), time_compare);
    print_processes(processes, n_proccess);

    printf("\nn_proccess = %i\n", n_proccess);


    // Liberamos memoria de variables usadas
    free(name);

    // Inicializamos una cola de bursts
    Queue* queue = queue_init();

    // Iniciamos la simulacion
    while(running) {
        printf("\n[T: %i] inicio de iteracion. \n", time);

        for (int i = 0; i < n_proccess; i++) {
            // Revisamos si empieza
            if (processes[i] -> start_time  == time) {
                queue_append(queue, processes[i]);
                ready_process(processes[i]);
                printf("[T: %i] Proceso %i READY\n", time, processes[i] -> PID);
                printf("[T: %i] Proceso %i agregado a cola, burst inicial: %i\n", time, processes[i] -> PID,
                        processes[i] -> bursts[0]);
                ++initialized;
            }
        }

        // Revisamos si termino la simulacion
        if (initialized == n_proccess) {
            running = 0;
        }

        ++time;
    }

    // Cerrar archivo
    fclose(fp);

    return 0;
}
