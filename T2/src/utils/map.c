//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "../structs/linked_list.h"
#include "utils.h"


extern volatile int running_threads;
extern pthread_mutex_t running_mutex;
extern LinkedList**  volatile ll_list;
extern volatile int ll_count;
extern int BUFFER_SIZE;



void* mapper(void* args) {
    Args* data = args;
    puts("I'm a thread, creating hashtable...");

    LinkedList* ll = ll_init();

    char* word = malloc(sizeof(char) * 60);

    // Creo una ll con cada key y un contador
    for(int i = 0; i < data -> array_length; i++) {
        strcpy(word, data -> array[i]);
        //printf("%s\n", data -> array[i]);
        ll_append(ll, word, 1);
    }

    args_destroy(data);
    free(word);

    printf("threads: %i, arrays: %i \n", running_threads, ll_count);

    // Elimino el thread del contador usando lock y guardo su hastable
    pthread_mutex_lock(&running_mutex);
    ll_list[ll_count] = ll;
    ll_count++;
    running_threads--;
    pthread_mutex_unlock(&running_mutex);

    puts("I'm done!");
    pthread_detach(pthread_self());
    pthread_exit (NULL);
}
