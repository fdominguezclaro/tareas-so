//
// Created by Felipe Dominguez Claro on 2019-04-10.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>


#include "utils.h"
#include "map.h"
#include "reduce.h"

#include "../structs/linked_list.h"


extern volatile int running;
extern pthread_mutex_t running_mutex;
extern int BUFFER_SIZE;


char** create_array(int BUFFER_SIZE, int WORD_SIZE) {
    char** array = malloc(sizeof(char*) * BUFFER_SIZE);

    for(int k = 0; k < BUFFER_SIZE; k++) {
        array[k] = malloc(WORD_SIZE * sizeof(char));
    }

    return array;
}

void array_destroy(char** array, int BUFFER_SIZE) {
    for(int k = 0; k < BUFFER_SIZE; k++) {
        free(array[k]);
    }

    free(array);
}

void* args_init(char** array, int array_length, void** shm_data) {
    Args *args = malloc(sizeof(Args));
    args -> array = array;
    args -> array_length = array_length;

    return args;
}

void* reducer_args_init(LinkedList ** array, int ll_count) {
    ReducerArgs *args = malloc(sizeof(ReducerArgs));
    args -> array = array;
    args -> ll_count = ll_count;

    return args;
}

void args_destroy(Args* args) {
    array_destroy(args -> array, BUFFER_SIZE);
    free(args);
}

void reducer_args_destroy(ReducerArgs* args) {
    // free(args -> array);
    free(args);
}

void print_array(char** array, int array_length) {
    for(int i = 0; i < array_length; i++) {
        printf("%s\n", array[i]);
    }
}

pthread_t init_mapper_thread(char** array, int array_length) {
    pthread_t thread;
    puts("\n--- Creando mapper thread ---");
    void* args = args_init(array, array_length, (void **) NULL);
    // Sumo el thread al ounter
    pthread_mutex_lock(&running_mutex);
    running++;
    pthread_mutex_unlock(&running_mutex);
    pthread_create(&thread, NULL, mapper, args);
    return thread;
}

pthread_t init_reducer_thread(LinkedList ** ll_list, int ll_count) {
    pthread_t thread;
    void* args = reducer_args_init(ll_list, ll_count);
    puts("\n--- Creando reducer thread ---");
    pthread_create(&thread, NULL, reducer, args);
    return thread;
}

void create_process(int* array, int array_length) {

}

void write_output(LinkedList* ll_list, char* name, int type) {
    puts("writing output!");
    FILE* file;
    file = fopen (name, "w");

    if (type == 1) {
        fprintf(file, "repeticiones,palabra\n");
    } else if (type == 2) {
        fprintf(file, "repeticiones,palabras\n");
    } else {
        fprintf(file, "cantidad,palabras\n");
    }

    LinkedList* ll_counts = ll_init();
    int counter = 0;

    Node* node = ll_list -> head;
    int actual = node -> count;
    // puts("DEBUGGER");
    char lenght[12];
    int size;

    while (node) {
        if (type == 1) {
            fprintf(file, "%i,%s\n", node -> count, node -> key);
        } else if (type == 2) {
            if (actual == node -> count) {
                if (counter == 0) {
                    fprintf(file, "%i,[%s", node -> count, node -> key);
                } else {
                    fprintf(file, ";%s", node -> key);
                }
                counter++;
            } else {
                fprintf(file, "]\n");
                fprintf(file, "%i,[%s", node -> count, node -> key);
                actual = node -> count;
                counter = 1;
            }

        } else if (type == 3) {
            size = strlen(node -> key);
            sprintf(lenght, "%d", size);
            ll_append(ll_counts, lenght, node -> count);
        }

        node = node -> next;
    }

    if (type == 2) {
        fprintf(file, "]\n");
    } else if (type == 3) {
        ll_quicksort(ll_counts, 0);
        node = ll_counts -> head;
        while (node) {
            fprintf(file, "%i,%s\n", node -> count, node -> key);
            node = node -> next;
        }
    }
    ll_destroy(ll_counts);

    fclose (file);
}

LinkedList** shm_to_ll(LinkedList** ll_list, void ** shared_data, int* shm_ids, int running) {
    // Recorro cada lista y copio la informacion
    LinkedList* shm_chunk = ll_init();
    int* word_size;
    char* string;
    int* count;
    for (int i = 0; i < running; i++) {
        // printf("%p\n", index);
        void* index = shared_data[i];
        int* size = (int*) index;
        puts("AQUI FALLO...");
        index = index + sizeof(int);
        // printf("%i", *size);
        puts("Converting inside fuction");
        for (int j = 0; j < *size; j++) {
            word_size = (int*) index;
            string = malloc(sizeof(char) * (*word_size + 1));
            strcpy(string, (char*) (index + sizeof(int)));
            count = (int*) (index + sizeof(int) + sizeof(char) * (*word_size + 1));
            index = (index + sizeof(int) + sizeof(char) * (*word_size + 1) + sizeof(int));
            ll_append(shm_chunk, string, *count);
        }
        ll_list[i] = shm_chunk;

        shmctl(shm_ids[i], IPC_RMID, NULL);
    }

    return ll_list;
}