#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>


#include "../utils/utils.h"
#include "../utils/map.h"
#include "../utils/reduce.h"


// Seteamos algunas variables globales
int BUFFER_SIZE = 4000;
// assumes no word exceeds length of 45
int WORD_SIZE = 45;

volatile int running;
volatile int version = 0;
int type;

// Para threads
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
LinkedList**  volatile ll_list;
volatile int ll_count;

// Para procesos
void *shared_memory;
int* shm_ids;
int mem_id = 1;
void** shm_data;
void* args;
pid_t pid, wpid;
int status = 0;


// Obtenido de https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
static volatile sig_atomic_t keep_running = 1;
static void sig_handler(int _) {
    (void)_;
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    clock_t t;
    t = clock();
    signal(SIGINT, sig_handler);
    puts("----- T2 SO FDOM -----\n");

    // Manejamos los parametros del programa
    if (argc == 5) {
        sscanf(argv[4], "%i", &type);
    } else if (argc == 4) {
        type = 1;
    } else if (argc < 4 || argc > 4) {
        puts("Cantidad de argumentos incorrectos!");
        exit(0);
    }

    // Tipo de la simulacion
    if (strcmp(argv[3], "threads") == 0) {
        puts("Version thread");
        version = 0;
        // Llevo la cuenta de los threads creados
        running = 0;
        // Lista de listas ligadas
        ll_list = malloc(sizeof(LinkedList *) * 1);

    } else if (strcmp(argv[3], "fork") == 0) {
        puts("Version fork");
        version = 1;

        shm_data = malloc(sizeof(void *) * 1);
        shm_ids = malloc(sizeof(void *) * 1);
    } else {
        puts("Tipo de simulacion incorrecto! Debe ser threads o fork");
        exit(0);
    }

    int chunk_count = 0;

    while (keep_running) {
        puts("Still running...");

        // Leemos el archivo
        FILE* f;
        f = fopen(argv[1], "r");

        if (f == NULL) {
            perror("Error while opening the file");
            exit(EXIT_FAILURE);
        }

        printf("%s opened succesfully\n\n", argv[1]);

        char* word = malloc(sizeof(char) * WORD_SIZE);
        char** array = create_array(BUFFER_SIZE, WORD_SIZE);

        while((chunk_count != BUFFER_SIZE)  && (keep_running) && (fscanf(f, "%s", word) == 1)) {
            // Lowercase
            for(int i = 0; word[i]; i++){
                word[i] = tolower(word[i]);
            }

            strcpy(array[chunk_count], word);

            chunk_count++;

            // Si no queda espacio, hago map sobre el chunk y empiezo otro
            if (chunk_count == BUFFER_SIZE - 1) {
                if (!version) {
                    ll_list = (LinkedList**)realloc(ll_list, sizeof(LinkedList*) * chunk_count);
                    init_mapper_thread(array, chunk_count);
                    array = create_array(BUFFER_SIZE, WORD_SIZE);
                } else {
                    shm_data = (void **)realloc(shm_data, sizeof(void *) * 1);
                    shm_ids = (int *)realloc(shm_data, sizeof(void *) * 1);
                    // Creo la memoria compartida
                    key_t key = ftok("keys/v1", mem_id);
                    mem_id++;
                    int shmid = shmget(key, sizeof(int) + sizeof(int) * BUFFER_SIZE + sizeof(char) * WORD_SIZE * BUFFER_SIZE + sizeof(int) * BUFFER_SIZE, IPC_CREAT | SHM_W | SHM_R);
                    shared_memory = shmat(shmid, NULL, 0);
                    shm_data[running] = shared_memory;
                    shm_ids[running] = shmid;
                    running++;

                    if ((pid = fork()) == 0) {
                        args = args_init(array, chunk_count, shared_memory);
                        mapper(args);
                        free(word);
                        free(shm_data);
                        fclose(f);
                        array = create_array(BUFFER_SIZE, WORD_SIZE);
                        exit(0);
                    }
                    array = create_array(BUFFER_SIZE, WORD_SIZE);
                }

                chunk_count = 0;
            }
        }

        free(word);
        fclose(f);

        LinkedList* words = NULL;
        if (!version) {
            // Proceso el ultimo chunk
            puts("Main creating the last thread");
            init_mapper_thread(array, chunk_count);
            // Espero a que terminen todos los threads
            while (running > 0);

            puts("\n --- All mapper threads finished! --- \n");

            // Creo un reducer_thread

            // DEBUGGER: Imprimo lista
            // Node* node = ll_list[0] -> head;
            // while (node) {
            //     printf("%i,%s", node -> count, node -> key);
            //     node = node -> next;
            // }

            pthread_t thread = init_reducer_thread(ll_list, ll_count);
            pthread_join(thread, (void*) &words);

            puts("-- Reducer thread finished! ---\n");

        } else {
            // Proceso el ultimo chunk
            key_t key = ftok("keys/v1", mem_id);
            mem_id++;
            int shmid = shmget(key, sizeof(int) + sizeof(int) * BUFFER_SIZE + sizeof(char) * WORD_SIZE * BUFFER_SIZE + sizeof(int) * BUFFER_SIZE, IPC_CREAT | SHM_W | SHM_R);
            shared_memory = shmat(shmid, NULL, 0);
            shm_data[running] = shared_memory;
            shm_ids[running] = shmid;

            if (running == 0) running++;

            if ((pid = fork()) == 0) {
                puts("Child process reading chunk");
                args = args_init(array, chunk_count, shared_memory);
                mapper(args);
                free(word);
                free(shm_data);
                fclose(f);
                exit(0);
            }

            // Espero que terminen los procesos
            while ((wpid = wait(&status)) > 0);
            puts("\n --- All mapper processes finished! --- \n");

            printf("Converting, %i threads run\n", running);

            // Convertimos las shared memories en una lista ligada para pasarsela al reduce
            LinkedList** ll_list = malloc(sizeof(LinkedList *) * running);
            ll_list = shm_to_ll(ll_list, shm_data, shm_ids, running);

            // DEBUGGER: Imprimo lista
            // Node* node = ll_list[0] -> head;
            // while (node) {
            //     printf("%i,%s", node -> count, node -> key);
            //     node = node -> next;
            // }

            puts("Sending to reducer");
            if ((pid = fork()) == 0) {
                puts("Child process reducing!");
                void* args = reducer_args_init(ll_list, ll_count);
                words = (LinkedList *)reducer(args);

                ll_destroy(words);
                free(ll_list);
                exit(0);

            }

            // Ahora leo las memorias compartidas y paso los datos a una lista ligada
            // No implementado dado que se cae antes de llegar... Pero el proceso es muy similar lo que hace shm_to_ll

            // Espero que termine el proceso reducer
            while ((wpid = wait(&status)) > 0);
            puts("--- Reducer process finished! ---");

            // Libero las memorias compartidas
            for (int i = 0; i < running; i++) {
                shmctl(shm_ids[i], IPC_RMID, NULL);
            }

        }

        puts("All childs finished\n");
        write_output(words, argv[2], type);
        ll_destroy(words);
        free(ll_list);

        if (!version) {
        } else {
        }

        t = clock() - t;
        double time = ((double)t)/CLOCKS_PER_SEC;

        // Imprimimos el tiempo
        printf("%f,", time);

        return 0;

    }

    puts("Stopped by signal SIGINT");
    return EXIT_SUCCESS;
}