#pragma once

/** Estructura de un nodo de la lista simplemente ligada */
typedef struct lnode {
    /** Nodo sigiente en la lista (NULL si no hay otro nodo) */
    struct lnode* next;
    /** valor que guarda el nodo */
    char* key;
    int count;
} Node;

/** Lista ligada. Mantiene el nodo inicial y el final */
typedef struct linked_list {
    /** Nodo inicial de la lista (posicion 0) */
    Node* head;
    /** Nodo final de la lista */
    Node* tail;
    int size;
} LinkedList;

/////// Funciones publicas de la lista ////////

/** Crea una lista ligada vacia */
LinkedList* ll_init();

/** Destruye la lista ligada */
void ll_destroy(LinkedList* ll);

/** Agrega un valor a la lista */
void ll_append(LinkedList* ll, char* key, int count);

/** Ordena la lista ligada usando quicksort */
void ll_quicksort(LinkedList* ll, int type);

int ll_get(LinkedList* ll, int position);
