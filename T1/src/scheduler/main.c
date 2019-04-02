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
    } else if (argc == 4) {
        quantum = 3;
    } else {
        printf("Cantidad de argumentos incorrectos!\n");
        exit(0);
    }

    types type = 0; // Tipo de la simulacion
    if (strcmp(argv[3], "np") == 0) {
        type = 0;
    } else if (strcmp(argv[3], "p") == 0) {
        type = 1;
    } else if (strcmp(argv[3], "fdom") == 0) {
        type = 2;
    } else {
        printf("Tipo de simulacion incorrecto!\n");
        exit(0);
    }

    printf("Tipo %s, quantum = %i\n", argv[3], quantum);

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
    int q_remaining = quantum; // Quantum restante para el proceso

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
        CPU_used = 0;
        printf("\n[T: %i] inicio de iteracion, Q = %i \n", time, q_remaining);

        for (int i = 0; i < n_proccess; i++) {
            // Revisamos si empieza algun proceso
            if (processes[i] -> start_time  == time) {
                queue_append(queue, processes[i]);
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
        enqueued = queue -> count;

        for (int i = 0; i < enqueued; i++) {
            entered = 0;
            work_status = -1;

            if (actual -> process -> status == READY) {
                if (!CPU_use && !CPU_used) {
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
                    if (type != 0) {
                        q_remaining -= 1;
                    }
                    if (!q_remaining) {
                        printf("%s: Proceso interruptido\n", actual -> process -> name);
                        set_state(actual -> process, READY);
                        interrupt(actual -> process);
                        q_remaining = quantum;
                        CPU_use = 0;
                        strcpy(actual_process, "");
                    }

                    // Restamos una unidad de tiempo al burst
                    work_status = work(actual -> process);
                    CPU_used = 1;
                    entered = 1;

                    // Termina el burst
                    if (work_status == 1) {
                        set_state(actual -> process, WAITING);
                        CPU_use = 0;
                        strcpy(actual_process, "");
                        q_remaining = quantum;
                    }
                    // Termina el proceso
                    if (work_status == 2) {
                        set_state(actual -> process, FINISHED);
                        CPU_use = 0;
                        deleted = actual;
                        finished++;
                        strcpy(actual_process, "");
                        actual -> process -> finish_time = time;
                        printf("%s: Finish time: %i\n", actual -> process -> name, actual -> process -> finish_time);
                        q_remaining = quantum;
                    }
            }

            else if (actual -> process -> status == WAITING && !entered) {
                // Restamos una unidad de tiempo al burst
                work_status = work(actual -> process);
                if (work_status == 1) {
                    set_state(actual -> process, READY);
                }
                entered = 1;
                increment_waiting_time(actual -> process);
            }

            // Elimino el nodo
            if (work_status == 2) {
                // printf("%s\n", actual -> process -> name);
                queue_remove(queue, deleted -> process -> PID);
            }

            if (i < queue -> count && enqueued > 1) {
                actual = actual -> next;
            }
        }

        // Revisamos si termino la simulacion
        if (finished == n_proccess) {
            running = 0;
            // printf("Quedan %i en cola\n", queue -> count);
        } else {
            print_queue_status(queue, actual_process);
        }

        time++;
    }

    qsort(processes, n_proccess, sizeof(Process *), PID_compare);
    write_statistics(processes, n_proccess, argv[2]);

    // Liberamos memoria
    free_processes(processes, n_proccess);
    free(queue);

    return 0;
}
