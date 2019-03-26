//
// Obtenido del taller 0 de estructuras de datos
//

#pragma once

/** Estructura de un nodo de la cola. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct node {
    /** Puntero al nodo anterior */
    struct node* last;
    /** Puntero al nodo siguiente */
    struct node* next;
    /** Numero almacenado en el nodo */
    int value;
} Node;

/** Estructura de una cola. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct queue {
    /** Nodo inicial de la cola */
    Node* start;
    /** Nodo final de la cola */
    Node* end;
    /** Contador de elementos de la cola */
    int count;
} Queue;

///////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de una cola. La crea vacia inicialmente */
Queue* queue_init();

/** Funcion que agrega un elemento al final de la cola */
void queue_append(Queue* qe, int value);

/** Funcion que obtiene el valor de la cola en la posicion dada */
int queue_get(Queue* qe, int position);

/** Funcion que destruye la cola liberando la memoria utilizada */
void queue_destroy(Queue* qe);
