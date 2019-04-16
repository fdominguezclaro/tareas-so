//
// Created by Felipe Dominguez Claro on 2019-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "reduce.h"
#include "../structs/hashtable.h"
#include "../structs/linked_list.h"


extern volatile int running_threads;
// extern hashtable **  volatile hastables_list;
// extern volatile int hashes_count;
extern LinkedList** volatile ll_list;
extern volatile int ll_count;


//LinkedList* reduce_to_ll(LinkedList *ll) {
//    // Copio el hash en una lista ligada, donde cada nodo tiene una palabra y su frecuencia
//    LinkedList* words = ll_init();
//    // struct node *current = NULL;
//
//    for(int i = 0; i < h -> tablesize; i++) {
//        current = h -> table[i];
//        while(current != NULL) {
//            // printf("%s\n", current -> key);
//            ll_append(words, strdup(current -> key), current -> count);
//            current = current -> next;
//        }
//    }
//
//    return words;
//}

void* reducer(void* args) {
    LinkedList* joined_ll = ll_init();

    ReducerArgs* data = args;
    puts("I'm a reducer thread, merging all hashtables!");
    printf("%d count\n", ll_count);

    char word[45];
    // transfer data to joined_ll and free ll
    for(int k = 0; k < ll_count; k++) {
        //frequencies(hastables_list[k]);
        printf("%d list\n", k);

        Node* node = ll_list[k] -> head;
        while (node) {
            strcpy(word, node -> key);
            ll_append(joined_ll, word, node -> count);

            Node* next = node -> next;
            node = next;
        }

        puts("DEBUGGER");

        ll_destroy(ll_list[k]);

//        struct node *new_node = NULL;
//        struct node *current = NULL;
//        int i;
//        for(i = 0; i < hastables_list[k] -> tablesize; i++) {
//            current = hastables_list[k] -> table[i];
//            while(current != NULL) {
//                new_node = add(frequency_hash, current -> key, 0);
//                new_node -> count = new_node -> count + current -> count;
//                current = current -> next;
//            }
//        }
//        freetable(hastables_list[k]);
    }

    //frequencies(frequency_hash);
    free(data);
    //free(hastables_list);
    puts("Sorting words!");
    //LinkedList* words = hash_to_ll(frequency_hash);
    //freetable(frequency_hash);

    ll_quicksort(joined_ll);

    puts("I'm done!");
    pthread_exit(joined_ll);
}
