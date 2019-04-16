//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "../structs/hashtable.h"
#include "../structs/linked_list.h"


extern volatile int running_threads;
extern pthread_mutex_t running_mutex;
// extern hashtable **  volatile hastables_list;
// extern volatile int hashes_count;
extern LinkedList**  volatile ll_list;
extern volatile int ll_count;


void* mapper(void* args) {
    // hashtable* hash = createtable(500000);
    // struct node *node = NULL;

    Args* data = args;
    puts("I'm a thread, creating hashtable...");
    // print_array(data -> array, data -> array_length);

    LinkedList* ll = ll_init();

    char word[45];
    // Creo un hastable con la frecuencia de cada key
    // Creo una ll con cada key y un contador
    for(int i = 0; i < data -> array_length; i++) {
        strcpy(word, data -> array[i]);
        ll_append(ll, word, 1);

        //node = add(hash, data -> array[i], 0);
        //node -> count++;
    }

    free(data);

    // Elimino el thread del contador usando lock y guardo su hastable
    pthread_mutex_lock(&running_mutex);
    ll_list[ll_count] = ll;
    // hastables_list[hashes_count] = hash;
    ll_count++;
    // hashes_count++;
    printf("threads: %i, hashes: %i \n", running_threads, ll_count);
    running_threads--;
    pthread_mutex_unlock(&running_mutex);

    puts("I'm done!");
    pthread_exit (NULL);
}
