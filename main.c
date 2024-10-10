#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TERMS 1000000 // Número total de términos a calcular
#define NUM_THREADS 3 // Número de hilos a utilizar

// Estructura para pasar argumentos a los hilos
typedef struct {
    int start;
    int end;
    double result;
} ThreadData;

// Función que calcula una parte de pi
void* calculate_pi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double sum = 0.0;

    for (int n = data->start; n < data->end; n++) {
        sum += (n % 2 == 0 ? 1.0 : -1.0) / (2.0 * n + 1.0);
    }

    data->result = sum;
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    double pi = 0.0;

    // Dividir el trabajo entre los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = (TERMS / NUM_THREADS) * i;
        thread_data[i].end = (TERMS / NUM_THREADS) * (i + 1);
        thread_data[i].result = 0.0;

        if (pthread_create(&threads[i], NULL, calculate_pi, &thread_data[i]) != 0) {
            perror("Error creando el hilo");
            return 1;
        }
    }

    // Esperar a que terminen los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        pi += thread_data[i].result;
    }

    pi *= 4.0; // Multiplicar por 4 para obtener el valor de pi

    printf("Valor de pi calculado con %d términos: %.1000000f\n", TERMS, pi);

    return 0;
}
