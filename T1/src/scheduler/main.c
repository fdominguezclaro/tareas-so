//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "process.h"

int main(int argc, char *argv[]) {
    printf("Tarea 1 FDOM\n\n");

    // Definimos el quantum
    int quantum;
    if (argc == 4) {
        sscanf(argv[3], "%i", &quantum);
    } else {
        quantum = 3;
    }

    // Inicializamos algunas variables
    Process** processes;
    int current_pid = 0;
    int size = 40; // Seteo inicialmente en 40 la cantidad de procesos soportada
    processes = malloc(sizeof(Process*) * size);

    // Leemos el archivo
    char ch;
    FILE *fp;
    fp = fopen(argv[1], "r"); // read mode

    if (fp == NULL) {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }

    printf("%s abierto exitosamente\n", argv[1]);

    char* name = malloc(sizeof(char) * 257);
    int priority;
    int start_time;
    int length;

    // El while fue una moodificacion obtenida de: https://overiq.com/c-programming-101/fscanf-function-in-c/
    while( fscanf(fp, "%s %i %i %i", name, &priority, &start_time, &length) == 4 ) {
        //fscanf(fp, "%*[^\n]");
        // printf("%s nombre\n", name);

        int* events = malloc(sizeof(int) * (length * 2 - 1));
        // Leo las rafagas del proceso
        for (int i = 0; i < (2 * length - 1); i++) {
            fscanf(fp, "%i", &events[i]);
            // printf("%i\n", events[i]);
        }
    }

    // Cerrar archivo
    fclose(fp);

    return 0;
}
