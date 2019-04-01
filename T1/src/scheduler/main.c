//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../structs/queue.h"
#include "../structs/process.h"
#include "../utils/utils.h"



int main(int argc, char *argv[]) {
    printf("----- T1 SO FDOM -----\n\n");

    // Definimos el quantum
    int quantum;
    if (argc == 5) {
        sscanf(argv[4], "%i", &quantum);
    } else {
        quantum = 3;
    }


    // if (strcmp(argv[3], "np") == 0)

    // Inicializamos algunas variables
    int PID = 0;
    int time = 0; // Reloj
    int size = 40; // Seteo inicialmente en 40 la cantidad de procesos soportada
    int running = 1; // Estado de la simulacion
    int CPU_use = 0; // Estado de CPU
    int CPU_used = 0; // 1 si fue ocupada en la iteracion
    int finished = 0; // Cantidad de procesos terminados
    int enqueued = 0; // Si hay procesos en la cola
    int n_proccess = 0; // Cantidad de procesos
    int initialized = 0; // Procesos iniciados
    int q_remaining = 0; // Quantum restante para el proceso

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
        n_proccess++;
        PID++;
    }

    // Cerrar archivo
    fclose(fp);

    // Ordeno los procesos segun tiempo de llegada para hacer un manejo eficiente de ciclos despues
    qsort(processes, n_proccess, sizeof(Process *), time_compare);
    printf("\nProcesos ordenados por start time!\n");
    print_processes(processes, n_proccess);

    printf("\nn_proccess = %i\n", n_proccess);

    // Liberamos memoria de variables usadas
    free(name);

    // Inicializamos una cola de bursts
    Queue* queue = queue_init();

    // Iniciamos la simulacion
    char actual_process[257];

    while(running) {
        CPU_use = 0;
        printf("\n[T: %i] inicio de iteracion. \n", time);

        for (int i = 0; i < n_proccess; i++) {
            // Revisamos si empieza algun proceso
            if (processes[i] -> start_time  == time) {
                queue_append(queue, processes[i]);
                enqueued++;
                // Ordeno los procesos segun prioridad
                printf("[T: %i] Proceso %i READY, agregado a cola, burst inicial: %i\n", time, processes[i] -> PID,
                        processes[i] -> bursts[0]);
                initialized++;
            }

            // Para no iterar extra
            if (processes[i] -> start_time  > time) {
                break;
            }
        }
        // Proceso la cola
        Node* actual = queue -> start;
        int work_status; // 1 si termino burst, 2 si termino el proceso
        int entered; // 1 si ya maneje ese proceso
        Node* deleted;
        for (int i = 0; i < enqueued; i++) {
            entered = 0;
            work_status = -1;

            if (actual -> process -> status == READY) {
                if (!CPU_use && CPU_used == 0) {
                    set_state(actual -> process, RUNNING);
                    if (actual -> process -> response_time == -1) {
                        set_response_time(actual -> process, time);
                    }
                    CPU_use = 1;
                    CPU_used = 1;
                    entered = 1;
                    actual -> process -> runs += 1;
                    strcpy(actual_process, actual -> process -> name);
                } else {
                    increment_waiting_time(actual -> process);
                }
            }

            if (actual -> process -> status == RUNNING) {
                // Restamos una unidad de tiempo al burst
                work_status = work(actual -> process);
                CPU_used = 1;
                entered = 1;

                // Termina el burst
                if (work_status == 1) {
                    set_state(actual -> process, WAITING);
                    CPU_used = 0;
                }
                // Termina el proceso
                if (work_status == 2) {
                    set_state(actual -> process, FINISHED);
                    CPU_used = 0;
                    deleted = actual;
                    enqueued -= 1;
                    finished++;
                    strcpy(actual_process, "");
                    actual -> process -> finish_time = time;
                    printf("Finish time: %i\n", actual -> process -> finish_time);
                }
            }

            if (actual -> process -> status == WAITING && !entered) {
                // Restamos una unidad de tiempo al burst
                work_status = work(actual -> process);
                if (work_status == 1) {
                    set_state(actual -> process, READY);
                }
                entered = 1;
                increment_waiting_time(actual -> process);
            }

            if (q_remaining == quantum - 1) {
                q_remaining = quantum;
                // Process p mode
            }

            actual = actual -> next;

            // Elimino el nodo
            if (work_status == 2) {
                queue_remove(queue, deleted -> process -> PID);
            }
        }

        // Revisamos si termino la simulacion
        if (finished == n_proccess) {
            running = 0;
        } else {
            print_queue_status(queue, actual_process);
        }

        time++;
    }

    qsort(processes, n_proccess, sizeof(Process *), PID_compare);
    write_statistics(processes, n_proccess);

    // Liberamos memoria
    free_processes(processes, n_proccess);
    free(queue);

    return 0;
}
