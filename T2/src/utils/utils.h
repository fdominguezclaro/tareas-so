//
// Created by Felipe Dominguez Claro on 2019-04-10.
//

#ifndef TAREAS_SO_UTILS_H
#define TAREAS_SO_UTILS_H

#endif //TAREAS_SO_UTILS_H

#pragma once

#include <pthread.h>

#include "../structs/linked_list.h"


typedef struct arg_struct {
    char** array;
    int array_length;
    void** shm_data;
} Args;

typedef struct reducer_arg_struct {
    LinkedList ** array;
    int ll_count;
} ReducerArgs;

// Crea un array de strings
char** create_array(int BUFFER_SIZE, int WORD_SIZE);

// Inicializa la estructura para pasarle argumentos a un thread o proceso mapper
void* args_init(char** array, int array_length, void ** shm_data);

// Inicializa la estructura para pasarle argumentos a un thread o proceso reducer
void* reducer_args_init(LinkedList ** array, int hashes_count);

void reducer_args_destroy(ReducerArgs* args);

// Destruye array
void array_destroy(char** array, int BUFFER_SIZE);

// Destruye la estructura
void args_destroy(Args* args);

// Imprime la lista de palabras
void print_array(char** array, int array_length);

// Crea un mapper thread
pthread_t init_mapper_thread(char**, int array_length);

// Crea un reducer thread
pthread_t init_reducer_thread(LinkedList ** hastables_list, int hashes_count);

// Crea un proceso
void create_process(int* array, int array_length);

void write_output(LinkedList* ll_list, char* name, int type);