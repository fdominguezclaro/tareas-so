// Reimplementacion del codigo obtenido del repositorio de IIC2133-2019-1 / Taller 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linked_list.h"

////// Funciones privadas ///////

/** Crea un nodo a partir de un valor */
static Node* node_init(char* key, int count) {
    Node* node = malloc(sizeof(Node));

    node -> key = malloc(strlen(key) + 1);
    strcpy(node -> key, key);
    node -> count = count;

    node -> next = NULL;

    return node;
}

/** Libera iterativamente la memoria del nodo y los nodos siguientes */
static void node_destroy(Node* node)
{
    // Esta funcion es iterativa porque puede generar stack overflow
    while (node)
    {
        Node* next = node -> next;
        free(node -> key);
        free(node);
        node = next;
    }
}

// Agrega un nodo a la lista ligada
static void ll_add_node(LinkedList* ll, Node* node)
{
    if (!ll -> head)
    {
        // Si la lista no tiene elementos, el nodo es inicial y final
        ll -> head = node;
        ll -> tail = node;
    }
    else
    {
        // Sino, lo agrego al final
        ll -> tail -> next = node;
        ll -> tail = node;
    }
}

/** Hace partition de ll y separa los datos en las otras listas */
static void partition(LinkedList* ll, LinkedList* smallers, LinkedList* equals, LinkedList* biggers, int type) {
    // Selecciono el pivote como el primer elemento de la lista
    int piv;
    char* spiv = NULL;

    if (!type) {
        piv = ll -> head -> count;
    } else {
        spiv = ll -> head -> key;
    }

    // Itero sobre la lista ll moviendo los nodos a la lista que corresponde
    Node* actual = ll -> head;
    while (actual)
    {
        // Guardo una referencia al nodo siguiente
        Node* next = actual -> next;

        // Desconecto el nodo actual de la lista
        actual -> next = NULL;

        if (!type) {
            // Veo en que lista va el nodo y lo inserto
            if (actual -> count < piv) ll_add_node(smallers, actual);
            else if (actual -> count == piv) ll_add_node(equals, actual);
            else ll_add_node(biggers, actual);
        } else {
            // Veo en que lista va el nodo y lo inserto
            if (strcmp(actual -> key, spiv) < 0) ll_add_node(smallers, actual);
            else if (strcmp(actual -> key, spiv) == 0) ll_add_node(equals, actual);
            else ll_add_node(biggers, actual);
        }

        // Actualizo el nodo actual
        actual = next;
    }
}

////// Funciones publicas ///////

/** Crea una lista ligada vacia */
LinkedList* ll_init() {
    // Creo la lista ligada
    LinkedList* ll = malloc(sizeof(LinkedList));

    // Pongo sus punteros en nulo y su contador en 0
    ll -> head = NULL;
    ll -> tail = NULL;
    ll -> size = 0;

    // Retorno la lista ligada
    return ll;
}

/** Destruye la lista ligada */
void ll_destroy(LinkedList* ll)
{
    // Primero destruyo los nodos de la lista
    node_destroy(ll -> head);

    // Luego libero la memoria de la lista
    free(ll);
}

/** Agrega un valor a la lista */
void ll_append(LinkedList* ll, char* key, int count) {
    // Busco la key a insertar
    Node* node = ll -> head;
    while (node) {
        // Si esta aumento su frecuencia
        if (strcmp(node -> key, key) == 0) {
            node -> count = node -> count + count;
         return;
        }

        Node* next = node -> next;
        node = next;
    }
    // Creo el nodo a insertar
    //printf("%s No encontrada\n", key);

    node = node_init(key, count);

    // Funcion que agrega un nodo al final de una lista ligada
    ll_add_node(ll, node);

    ll -> size = ll -> size + 1;
}

/** Ordena la lista ligada usando quicksort */
void ll_quicksort(LinkedList* ll, int type)
{
    // La idea es que al hacer partition se dividan los datos en 3 listas:
    // Menores al pivote, iguales al pivote y mayores al pivote.
    // Luego se ordenan las listas recursivamente, y se unen los nodos en ll

    // Si no tengo elementos solo retorno
    if (!ll -> head) return;

    // Creo las listas a correspondientes
    LinkedList smallers;
    smallers.head = NULL;
    smallers.tail = NULL;
    LinkedList equals;
    equals.head = NULL;
    equals.tail = NULL;
    LinkedList biggers;
    biggers.head = NULL;
    biggers.tail = NULL;

    // Llamo a partition con la lista ligada original y las nuevas listas
    partition(ll, &smallers, &equals, &biggers, type);

    // Llamo a quicksort recursivamente
    ll_quicksort(&smallers, type);
    ll_quicksort(&biggers, type);

    // Concateno los nodos de las 3 listas
    if (smallers.tail)
    {
        // Si existe al menos un nodo en smallers, concateno smallers - equals
        smallers.tail -> next = equals.head;
    }
    if (biggers.head)
    {
        // Si existe al menos un nodo en biggers, concateno equals - biggers
        equals.tail -> next = biggers.head;
    }

    // Agrego los nodos a la lista ll
    ll -> head = smallers.head ? smallers.head : equals.head;
    ll -> tail = biggers.tail ? biggers.tail : equals.tail;
}