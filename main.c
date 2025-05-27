#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

//Metodo para calcular el promedio de la temperatura de las celdas vecinas
float calcular_promedio(float **matriz, int i, int j) {
    return (matriz[i-1][j] + matriz[i+1][j] + matriz[i][j-1] + matriz[i][j+1]) / 4.0;
}

//Metodo para crear la matriz
float **crear_matriz(int N) {
    float **matriz = malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        matriz[i] = malloc(N * sizeof(float));
    }
    return matriz;
}

//Metodo para inicializar la matriz
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

//Metodo para liberar la matriz
void liberar_matriz(float **matriz, int N) {
    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

//Función principal
int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <N> <umbral> <max_iter> <modo: 0=secuencial | 1=paralelo>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    float umbral = atof(argv[2]);
    int max_iter = atoi(argv[3]);
    int modo = atoi(argv[4]);

    float **actual = crear_matriz(N);
    float **siguiente = crear_matriz(N);
    inicializar_matriz(actual, N);

    double start = omp_get_wtime();

    int iter = 0;
    float max_cambio;

    do {
        max_cambio = 0.0;

        //Ejecución secuencial
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

        //Ejecución paralelizada con OpenMP
        } else {
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

        //Intercambio de matrices
        float **temp = actual;
        actual = siguiente;
        siguiente = temp;

        iter++;
    } while (max_cambio >= umbral && iter < max_iter);

    double end = omp_get_wtime();

    printf("Modo: %s\n", modo == 0 ? "Secuencial" : "Paralelo");
    printf("Iteraciones: %d\n", iter);
    printf("Tiempo: %f segundos\n", end - start);

    liberar_matriz(actual, N);
    liberar_matriz(siguiente, N);

    return 0;
}