//
// Created by Felipe Dominguez Claro on 2019-04-10.
//

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "map.h"
#include "reduce.h"

char** create_array(int BUFFER_SIZE, int WORD_SIZE) {
    char** array = malloc(sizeof(char*) * BUFFER_SIZE);

    for(int k = 0; k < BUFFER_SIZE; k++) {
        array[k] = malloc(WORD_SIZE * sizeof(char));
    }

    return array;
}

Args* args_init(char** array, int array_length) {
    Args *args = malloc(sizeof(Args));
    args -> array = array;
    args -> array_length = array_length;

    return args;
}

void args_destroy(Args* args) {
    free(args);
}

//void print_array(char** array, int array_length) {
//    for(int i = 0; i < array_length; i++) {
//        printf("%s\n", array[i);
//    }
//}

pthread_t init_mapper_thread(char** array, int array_length) {
    pthread_t mapper_thread;
    puts("--- Creando thread ---");
    Args* args = args_init(array, array_length);
    pthread_create(&mapper_thread, NULL, mapper, (void *) args);
    args_destroy(args);
}

void create_process(int* array, int array_length) {

}