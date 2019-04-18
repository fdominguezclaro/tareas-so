//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "reduce.h"
#include "utils.h"
#include "../structs/linked_list.h"


extern LinkedList** volatile ll_list;
extern volatile int ll_count;


void* reducer(void* args) {
    LinkedList* joined_ll = ll_init();

    ReducerArgs* data = args;
    puts("I'm a reducer thread, merging all ll's!");

    char* word = malloc(sizeof(char) * 45);
    // transfer data to joined_ll and free ll
    for(int k = 0; k < ll_count; k++) {

        Node* node = ll_list[k] -> head;
        while (node) {
            strcpy(word, node -> key);
            ll_append(joined_ll, word, node -> count);

            node = node -> next;
        }

        // puts("DEBUGGER");

        ll_destroy(ll_list[k]);
    }

    free(word);
    reducer_args_destroy(data);

    puts("Sorting words!");

    ll_quicksort(joined_ll, 1);
    ll_quicksort(joined_ll, 0);

    puts("I'm done!");
    pthread_exit(joined_ll);
}
