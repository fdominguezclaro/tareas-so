//
// Created by Felipe Dominguez Claro on 2019-04-10.
//

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "map.h"
#include "reduce.h"

#include "../structs/linked_list.h"


extern volatile int running_threads;
extern pthread_mutex_t running_mutex;

char** create_array(int BUFFER_SIZE, int WORD_SIZE) {
    char** array = malloc(sizeof(char*) * BUFFER_SIZE);

    for(int k = 0; k < BUFFER_SIZE; k++) {
        array[k] = malloc(WORD_SIZE * sizeof(char));
    }

    return array;
}

void* args_init(char** array, int array_length) {
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
    free(args -> array);
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
    void* args = args_init(array, array_length);
    // Sumo el thread al ounter
    pthread_mutex_lock(&running_mutex);
    running_threads++;
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
    FILE* file;
    file = fopen (name, "w");

    if (type <= 2) {
        fprintf(file, "repeticiones,palabras\n");
    } else {
        fprintf(file, "cantidad,palabras\n");
    }

    LinkedList* sames = ll_init();
    int counter = 0;

    Node* node = ll_list -> head;
    int actual = node -> count;

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

        } else {
        }

        node = node -> next;
    }

    if (type == 2) {
        fprintf(file, "]\n");
    }

    fclose (file);
}

