#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#include "../utils/utils.h"
#include "../utils/map.h"
#include "../utils/reduce.h"


// Seteamos algunas variables globales
int BUFFER_SIZE = 1024;
// assumes no word exceeds length of 45
int WORD_SIZE = 45;

volatile int running;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
LinkedList**  volatile ll_list;
volatile int ll_count;


// Obtenido de https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
static volatile sig_atomic_t keep_running = 1;
static void sig_handler(int _) {
    (void)_;
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sig_handler);
    puts("----- T2 SO FDOM -----\n");

    // Manejamos los parametros del programa
    // Definimos el type
    int type;
    if (argc == 5) {
        sscanf(argv[4], "%i", &type);
    } else if (argc == 4) {
        type = 1;
    } else if (argc < 4 || argc > 4){
        puts("Cantidad de argumentos incorrectos!");
        exit(0);
    }

    // Tipo de la simulacion
    int version = 0;
    if (strcmp(argv[3], "threads") == 0) {
        puts("Version thread");
        version = 0;
        // Llevo la cuenta de los threads creados
        running = 0;
        // Lista de listas ligadas
        ll_list = malloc(sizeof(LinkedList *) * 1);

    } else if (strcmp(argv[3], "fork") == 0) {
        puts("Version fork");
        version = 1;
    } else {
        puts("Tipo de simulacion incorrecto! Debe ser threads o fork");
        exit(0);
    }

    int chunk_count = 0;

    while (keep_running) {
        puts("Still running...");

        // Leemos el archivo
        FILE* f;
        f = fopen(argv[1], "r");

        if (f == NULL) {
            perror("Error while opening the file");
            exit(EXIT_FAILURE);
        }

        printf("%s opened succesfully\n\n", argv[1]);

        char* word = malloc(sizeof(char) * WORD_SIZE);
        char** array = create_array(BUFFER_SIZE, WORD_SIZE);

        while((chunk_count != BUFFER_SIZE)  && (keep_running) && (fscanf(f, "%s", word) == 1)) {
            // Lowercase
            for(int i = 0; word[i]; i++){
                word[i] = tolower(word[i]);
            }

            strcpy(array[chunk_count], word);

            chunk_count++;

            // Si no queda espacio, hago map sobre el chunk y empiezo otro
            if (chunk_count == BUFFER_SIZE - 1) {
                if (!version) {
                    ll_list = (LinkedList**)realloc(ll_list, sizeof(LinkedList*) * chunk_count);
                    init_mapper_thread(array, chunk_count);
                    array = create_array(BUFFER_SIZE, WORD_SIZE);
                } else {
                    puts("Version fork");
                }

                chunk_count = 0;
            }
        }

        free(word);

        // Cerrar archivo
        fclose(f);

        LinkedList* words = NULL;
        if (!version) {
            // Proceso el ultimo chunk
            puts("Main creating the last thread");
            init_mapper_thread(array, chunk_count);
            // Espero a que terminen todos los threads
            while (running > 0);

            puts("All threads finished!");

            // Creo un reducer_thread
            pthread_t thread = init_reducer_thread(ll_list, ll_count);
            pthread_join(thread, (void*) &words);

            puts("Reducer thread finished!");

        } else {
        }

        write_output(words, argv[2], type);

        ll_destroy(words);
        free(ll_list);

        return 0;

    }

    puts("Stopped by signal SIGINT");
    return EXIT_SUCCESS;
}