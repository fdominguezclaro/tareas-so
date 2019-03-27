//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "process.h"

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
    int current_pid = 0;
    int size = 40; // Seteo inicialmente en 40 la cantidad de procesos soportada
    Process** processes = malloc(sizeof(Process*) * size);

    // Leemos el archivo
    char ch;
    FILE *fp;
    fp = fopen(argv[1], "r"); // read mode

    if (fp == NULL) {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }

    printf("%s opened succesfully\n", argv[1]);

    // Creamos los procesos
    char* name = malloc(sizeof(char) * 257);
    int priority;
    int start_time;
    int length;
    int actual_PID = 0;

    // El while fue una modificacion obtenida de: https://overiq.com/c-programming-101/fscanf-function-in-c/
    while( fscanf(fp, "%s %i %i %i", name, &priority, &start_time, &length) == 4 ) {
        // Creo un array con las rafagas
        int* bursts = malloc(sizeof(int) * (length * 2 - 1));
        // Leo las rafagas del proceso
        for (int i = 0; i < (2 * length - 1); i++) {
            fscanf(fp, "%i", &bursts[i]);
            printf("%i\n", bursts[i]);
        }

        Process* process = process_init(actual_PID, priority, start_time, length, name);
        processes[actual_PID] = process;
        printf("Proceso %i:%s agregado, prioridad: %i\n", actual_PID, name, priority);
        actual_PID += 1;
    }

    printf("%i\n", processes[0] -> PID);

    // Cerrar archivo
    fclose(fp);

    return 0;
}
