#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Metodo para calcular el promedio de la temperatura de las celdas vecinas
float calcular_promedio(float **matriz, int i, int j) {
    return (matriz[i-1][j] + matriz[i+1][j] + matriz[i][j-1] + matriz[i][j+1]) / 4.0;
}

// Metodo para crear la matriz NxN reservando memoria dinámicamente
float **crear_matriz(int N) {
    float **matriz = malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        matriz[i] = malloc(N * sizeof(float));
    }
    return matriz;
}

// Metodo para inicializar la matriz con las celdas exteriores a 100°C e interiores a 0°C.
void inicializar_matriz(float **matriz, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == N-1 || j == N-1) {
                matriz[i][j] = 100.0;
            } else {
                matriz[i][j] = 0.0;
            }
        }
    }
}

// Metodo para liberar la memoria previamente asignada a la matriz.
void liberar_matriz(float **matriz, int N) {
    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Función principal
int main(int argc, char *argv[]) {

    // Validación de los argumentos
    if (argc != 5) {
        printf("Uso: %s <N> <umbral> <max_iter> <modo: 0=secuencial | 1=paralelo>\n", argv[0]);
        return 1;
    }

    // Tamaño de la matriz
    int N = atoi(argv[1]);
    // Umbral de cambio máximo permitido
    float umbral = atof(argv[2]);
    // Iteraciones máximas permitidas
    int max_iter = atoi(argv[3]);
    // Modo de ejecución (0: secuencial, 1: paralelo)
    int modo = atoi(argv[4]);

    // Creación e inicialización de las matrices
    float **actual = crear_matriz(N);
    float **siguiente = crear_matriz(N);
    inicializar_matriz(actual, N);

    // Inicio de la medición de tiempo
    double start = omp_get_wtime();

    int iter = 0;
    float max_cambio;

    // Bucle principal de simulación
    do {
        max_cambio = 0.0;

        // Ejecución secuencial
        if (modo == 0) {
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    siguiente[i][j] = calcular_promedio(actual, i, j);
                    float cambio = fabs(siguiente[i][j] - actual[i][j]);
                    if (cambio > max_cambio) {
                        max_cambio = cambio;
                    }
                }
            }

        // Ejecución paralelizada con OpenMP
        } else {
            // Se paraleliza el bucle externo con reducción sobre la variable max_cambio
            #pragma omp parallel for reduction(max:max_cambio)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    siguiente[i][j] = calcular_promedio(actual, i, j);
                    float cambio = fabs(siguiente[i][j] - actual[i][j]);
                    if (cambio > max_cambio) {
                        max_cambio = cambio;
                    }
                }
            }
        }

        // Intercambio de matrices para la siguiente iteración
        float **temp = actual;
        actual = siguiente;
        siguiente = temp;

        iter++;
    } while (max_cambio >= umbral && iter < max_iter);

    // Fin de la medición de tiempo
    double end = omp_get_wtime();

    // Impresión de los resultados
    printf("Modo: %s\n", modo == 0 ? "Secuencial" : "Paralelo");
    printf("Iteraciones: %d\n", iter);
    printf("Tiempo: %f segundos\n", end - start);

    // Liberación de la memoria
    liberar_matriz(actual, N);
    liberar_matriz(siguiente, N);

    return 0;
}