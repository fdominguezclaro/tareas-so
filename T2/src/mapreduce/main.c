#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#include "../utils/utils.h"
#include "../utils/map.h"
#include "../utils/reduce.h"


// Obtenido de https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
static volatile sig_atomic_t keep_running = 1;
volatile int running_threads;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

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
        version = 0;
        // Llevo la cuenta de los threads creados
        running_threads = 0;
    } else if (strcmp(argv[3], "fork") == 0) {
        version = 1;
    } else {
        puts("Tipo de simulacion incorrecto! Puede ser threads o fork");
        exit(0);
    }

    // Seteamos algunas variables globales
    int BUFFER_SIZE = 8192;
    // assumes no word exceeds length of 45
    int WORD_SIZE = 45;
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

        char ch;
        char** array = create_array(BUFFER_SIZE, WORD_SIZE);
        int j = 0;

        while((ch = fgetc(f)) && (ch != EOF) && keep_running) {
            if(ch == ' ') {
                array[chunk_count++][j] = '\0';
                j = 0;
                continue;
            } else if(ch == '\n') {
                continue;
            }

            ch = tolower(ch);
            array[chunk_count][j++] = ch;

            // Si no queda espacio, hago map sobre el chunk y empiezo otro
            if (chunk_count == BUFFER_SIZE - 1) {
                if (!version) {
                    puts("Version thread");
                    puts("Main creating a thread");
                    init_mapper_thread(array, chunk_count);
                } else {
                    puts("Version fork");
                }

                j = 0;
                chunk_count = 0;
            }
        }

        // Proceso el ultimo chunk
        if (!version) {
            puts("Main creating the last thread");
            init_mapper_thread(array, chunk_count);
            // Espero a que terminen todos los threads
            while (running_threads > 0) {
                sleep(1);
            }

            puts("All threads finished!");
        } else {
            puts("Version fork");
        }

        for(int k = 0; k < chunk_count; k++) {
            // printf("%s\n", array[k]);
            free(array[k]);
        }

        free(array);

        // Cerrar archivo
        fclose(f);

        return 0;

    }

    puts("Stopped by signal SIGINT");
    return EXIT_SUCCESS;
}