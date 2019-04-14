//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>

#include "map.h"
#include "../structs/hashtable.h"


extern volatile int running_threads;
extern pthread_mutex_t running_mutex;
extern hashtable **  volatile hastables_list;
extern volatile int hashes_index;


void* mapper(void* args) {
    hashtable* hash = createtable(50000);
    struct node *node = NULL;

    Args* data = args;
    puts("I'm a thread, creating hashtable...:");
    // print_array(data -> array, data -> array_length);

    // Creo un hastable con la frecuencia de cada key
    for(int i = 0; i < data -> array_length; i++) {
        node = add(hash, data -> array[i], 1);
        node -> frequency++;
    }

    // Elimino el thread del contador usando lock y guardo su hastable
    pthread_mutex_lock(&running_mutex);
    hastables_list[hashes_index] = hash;
    hashes_index++;
    printf("threads: %i, hashes: %i \n", running_threads, hashes_index);
    running_threads--;
    pthread_mutex_unlock(&running_mutex);

    puts("I'm done!");
    pthread_exit (NULL);
}

char** map(char** array, int array_length) {
    // Map array
    return array;
}