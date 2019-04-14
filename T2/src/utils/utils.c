//
// Created by Felipe Dominguez Claro on 2019-04-10.
//

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "map.h"
#include "reduce.h"

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

void args_destroy(Args* args) {
    free(args);
}

void print_array(char** array, int array_length) {
    for(int i = 0; i < array_length; i++) {
        printf("%s\n", array[i]);
    }
}

pthread_t init_mapper_thread(char** array, int array_length) {
    pthread_t mapper_thread;
    puts("--- Creando thread ---");
    void* args = args_init(array, array_length);
    // Sumo el thread al ounter
    pthread_mutex_lock(&running_mutex);
    running_threads++;
    pthread_mutex_unlock(&running_mutex);
    pthread_create(&mapper_thread, NULL, mapper, args);
    return mapper_thread;
}

void create_process(int* array, int array_length) {

}

