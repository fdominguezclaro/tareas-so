//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "../structs/linked_list.h"
#include "utils.h"


extern volatile int running;
extern pthread_mutex_t running_mutex;
extern LinkedList**  volatile ll_list;
extern volatile int ll_count;
extern int BUFFER_SIZE;
extern volatile int version;
extern void *shared_memory;


void* mapper(void* args) {
    Args* data = args;
    puts("I'm a worker mapping words...");

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

    printf("workers: %i, arrays: %i \n", running, ll_count);

    if (!version) {
        // Elimino el thread del contador usando lock y guardo su ll
        pthread_mutex_lock(&running_mutex);
        ll_list[ll_count] = ll;
        ll_count++;
        running--;
        pthread_mutex_unlock(&running_mutex);

        puts("I'm done!");
        pthread_detach(pthread_self());
        pthread_exit (NULL);
    } else {
        Node* node = ll -> head;
        void* index = shared_memory;
        int* size = (int*) index;
        *size = ll -> size;
        index = index + sizeof(int);
        int* word_size;
        char* string;
        int* count;
        while (node) {
            word_size = (int*) index;
            *word_size = strlen(node -> key);
            string = (char*) (index + sizeof(int));
            strcpy(string, node -> key);
            count = (int*) (index + sizeof(int) + sizeof(char) * (*word_size + 1));
            *count = node -> count;
            index = (index + sizeof(int) + sizeof(char) * (*word_size + 1) + sizeof(int));

            node = node -> next;
        }
        ll_destroy(ll);
    }

    return(NULL);
}
