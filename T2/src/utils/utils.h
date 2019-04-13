//
// Created by Felipe Dominguez Claro on 2019-04-10.
//

#ifndef TAREAS_SO_UTILS_H
#define TAREAS_SO_UTILS_H

#endif //TAREAS_SO_UTILS_H

#pragma once

#include <pthread.h>


typedef struct arg_struct {
    char** array;
    int array_length;
} Args;

// Crea un array de strings
char** create_array(int BUFFER_SIZE, int WORD_SIZE);

// Inicializa la estructura para pasarle argumentos a un thread o proceso
Args* args_init(char** array, int array_length);

// Destruye la estructura
void args_destroy(Args* args);

// Imprime la lista de palabras
//void print_array(int* array, int array_length);

// Crea un thread
pthread_t init_mapper_thread(char**, int array_length);

// Crea un proceso
void create_process(int* array, int array_length);