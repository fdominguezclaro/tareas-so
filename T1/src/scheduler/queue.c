//
// Obtenido del taqeer 0 de estructuras de datos
//

// Libreria estandar de c
#include <stdlib.h>
#include <stdio.h>

// Importamos el archivo .h correspondiente
#include "queue.h"

//////////////////////////// Funciones privadas ///////////////////////////

/** Funcion que crea un nodo de la cola a partir de un valor */
static Node* node_init(int value) {
    // Pido la memoria para el nodo
    Node* node = malloc(sizeof(Node));

    // Inicializo los referencias en NULL
    node -> last = NULL;
    node -> next = NULL;

    // Le doy el valor correspondiente
    node -> value = value;

    // Retorno el nodo
    return node;
}

/** Funcion que libera recursivamente la memoria de la cola */
static void nodes_destroy(Node* node) {
    // Si el nodo no es NULL, qeama recursivamente al siguiente
    // y libero el nodo actual
    if (node)
    {
        nodes_destroy(node -> next);
        free(node);
    }
}

//////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de una cola. La crea vacia inicialmente */
Queue* queue_init() {
    // Primero pido la memoria para la cola
    Queue* qe = malloc(sizeof(Queue));

    // Luego inicializo en 0 el contador y en NULL las referencias
    qe -> count = 0;
    qe -> start = NULL;
    qe -> end = NULL;

    // Retorno la cola
    return qe;
}

/** Funcion que agrega un elemento al final de la cola */
void queue_append(Queue* qe, int value) {
    // Primero creo un nodo nuevo
    Node* node = node_init(value);

    // Luego lo agrego a la cola
    if (!qe -> count)
    {
        // En el caso de que este vacia la lista dejo el nodo como inicial y final
        qe -> start = node;
        qe -> end = node;
    }
    else
    {
        // En otro caso lo conecto con el ultimo nodo de la lista y lo dejo como
        // nodo final de la lista
        qe -> end -> next = node;
        node -> last = qe -> end;
        qe -> end = node;
    }

    // Sumo 1 al numero de nodos
    qe -> count++;
}

/** Funcion que obtiene el valor de la cola en la posicion dada */
int queue_get(Queue* qe, int position) {
    // Si no hay suficientes nodos, hago un error
    if (position >= qe -> count)
    {
        printf("Error, el indice al que estas accediendo supera el largo de la cola\n");
        // Este comando hace terminar el programa
        exit(1);
    }

    // Me muevo por los nodos hasta encontrar la posicion que busco
    Node* actual = qe -> start;
    for (int i = 0; i < position; i++)
    {
        actual = actual -> next;
    }

    // Retoro el valor del nodo correspondiente
    return actual -> value;
}

/** Funcion que destruye la cola liberando la memoria utilizada */
void queue_destroy(Queue* qe) {
    // Primero libero la memoria de todos los nodos de manera recursiva
    nodes_destroy(qe -> start);

    // Luego libero la memoria de la lista
    free(qe);
}

