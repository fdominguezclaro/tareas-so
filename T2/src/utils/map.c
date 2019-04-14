//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>

#include "map.h"


void* mapper(void* args) {
    Args* data = args;
    puts("I'm a thread, and my words are:");
    print_array(data -> array, data -> array_length);
    args_destroy(args);
    pthread_exit (NULL);
}
