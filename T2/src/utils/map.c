//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>

#include "map.h"


extern volatile int running_threads;
extern pthread_mutex_t running_mutex;


void* mapper(void* args) {

    Args* data = args;
    puts("I'm a thread, and my words are:");
    // print_array(data -> array, data -> array_length);
    args_destroy(args);
    // Elimino el thread de la cuenta usando lock
    pthread_mutex_lock(&running_mutex);
    running_threads--;
    pthread_mutex_unlock(&running_mutex);
    puts("I'm done!");
    pthread_exit (NULL);
}

char** map(char** array, int array_length) {
    // Map array
    return array;
}