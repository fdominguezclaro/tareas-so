//
// Obtenido del taqeer 0 de estructuras de datos
//

// Libreria estandar de c
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "process.h"

//////////////////////////// Funciones privadas ///////////////////////////

/** Funcion que crea un nodo de la cola a partir de un valor */
static Node* node_init(Process* process) {
    // Pido la memoria para el nodo
    Node* node = malloc(sizeof(Node));

    // Inicializo los referencias en NULL
    node -> last = NULL;
    node -> next = NULL;

    // Le doy el valor correspondiente
    node -> process = process;

    // Retorno el nodo
    return node;
}

/** Funcion que libera recursivamente la memoria de la cola */
static void nodes_destroy(Node* node) {
    // Si el nodo no es NULL, llama recursivamente al siguiente
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

    // Retorno la cola
    return qe;
}

/** Funcion que agrega un elemento al final de la cola */
void queue_append(Queue* qe, Process* process) {
    // Primero creo un nodo nuevo
    Node* node = node_init(process);

    // Luego lo agrego a la cola
    if (!qe -> count) {
        // En el caso de que este vacia la lista dejo el nodo como inicial y final
        qe -> start = node;
    } else {
        // En otro caso busco su posicion segun su prioridad y lo inserto
        Node* actual = qe -> start;
        Node* previous = actual; // Para el ultimo caso
        int inserted = 0;
        for (int i = 0; i < qe -> count; i++) {
            if (actual -> process -> priority > process -> priority) {
                // Sigo buscando
                previous = actual;
                actual = actual -> next;
            }
            if (actual -> process -> priority < process -> priority) {
                // Agrego el nodo
                if (i == 0) {
                    node -> next = qe -> start;
                    qe -> start = node;
                    // printf("Nodo insertado al principio \n");
                }
                else {
                    previous -> next = node;
                    node -> next = actual;
                    // printf("Nodo insertado en el anterior \n");
                }
                inserted = 1;
                // Dejo de buscar
                break;
            }
            if (actual -> process -> priority == process -> priority) {
                // Ordeno segun PID
                int sorting = 1;
                while (sorting) {
                    if (actual -> process -> PID < process -> PID) {
                        // Lo agrego en esta posicion
                        previous -> next = node;
                        node -> next = actual;
                        sorting = 0;
                    }

                    // Sigo buscando la posicion segun PID
                    previous = actual;
                    actual = actual -> next;
                }

                break;
            }
        }
        if (!inserted){
            // En caso que insertamos de ultimo lugar
            actual -> next = node;
            // printf("Nodo insertado en ultimo \n");
            inserted = 1;
        }
    }

    // Sumo 1 al numero de nodos
    qe -> count++;
}

void queue_remove(Queue* qe, int PID) {
    if (qe -> count == 1) {
        qe -> start = NULL;
    } else {
        Node* actual = qe -> start;
        Node* previous = actual; // Para el ultimo caso
        for (int i = 0; i < qe -> count; i++) {
            if (actual -> process -> PID == PID) {
                previous -> next = actual -> next;
                qe -> count -= 1;
                free(actual);
            } else {
                previous = actual;
                actual = actual -> next;
            }
        }
    }
}

/** Funcion que obtiene el valor de la cola en la posicion dada */
Process* queue_get(Queue* qe, int position) {
    // Si no hay suficientes nodos, hago un error
    if (position >= qe -> count) {
        printf("Error, el indice al que estas accediendo supera el largo de la cola\n");
        // Este comando hace terminar el programa
        exit(1);
    }

    // Me muevo por los nodos hasta encontrar la posicion que busco
    Node* actual = qe -> start;
    for (int i = 0; i < position; i++) {
        actual = actual -> next;
    }

    // Retoro el valor del nodo correspondiente
    return actual -> process;
}


/** Funcion que destruye la cola liberando la memoria utilizada */
void queue_destroy(Queue* qe) {
    // Primero libero la memoria de todos los nodos de manera recursiva
    nodes_destroy(qe -> start);
    // Luego libero la memoria de la lista
    free(qe);
}

