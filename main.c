#include <stdio.h>      // Entrada y salida estándar
#include <stdlib.h>     // Gestión de memoria dinámica y conversión de tipos
#include <math.h>       // Funciones matemáticas como fabs()
#include <omp.h>        // Librería OpenMP para paralelización

// Función: calcular_promedio
// Propósito: Calcular la temperatura promedio de las celdas vecinas (arriba,abajo, izquierda, derecha) de una celda específica en la matriz.
// Metodo para calcular el promedio de la temperatura de las celdas vecinas
float calcular_promedio(float **matriz, int i, int j) {
    return (matriz[i-1][j] + matriz[i+1][j] + matriz[i][j-1] + matriz[i][j+1]) / 4.0;
}

// Función: crear_matriz
// Propósito: Reservar memoria para una matriz dinámica de tamaño N x N.
// Metodo para crear la matriz
float **crear_matriz(int N) {
    float **matriz = malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        matriz[i] = malloc(N * sizeof(float));
    }
    return matriz;
}

// Función: inicializar_matriz
// Propósito: Inicializar la matriz con bordes a 100°C y el interior a 0°C.
// Metodo para inicializar la matriz
void inicializar_matriz(float **matriz, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == N-1 || j == N-1) {
                matriz[i][j] = 100.0;   // Bordes calientes
            } else {
                matriz[i][j] = 0.0;     // Interior frío
            }
        }
    }
}

// Función: liberar_matriz
// Propósito: Liberar la memoria previamente asignada para la matriz.
// Metodo para liberar la matriz
void liberar_matriz(float **matriz, int N) {
    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Función principal: main
// Propósito: Ejecutar la simulación de difusión de calor, tanto en modo secuencial como en paralelo utilizando OpenMP, según lo indicado por el usuario.
int main(int argc, char *argv[]) {
    // Validación de argumentos
    if (argc != 5) {
        printf("Uso: %s <N> <umbral> <max_iter> <modo: 0=secuencial | 1=paralelo>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);         // Tamaño de la matriz
    float umbral = atof(argv[2]);  // Umbral de cambio máximo permitido
    int max_iter = atoi(argv[3]);  // Iteraciones máximas permitidas
    int modo = atoi(argv[4]);      // Modo de ejecución (0: secuencial, 1: paralelo)

    // Creación e inicialización de matrices de trabajo
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

    // Reporte de resultados
    printf("Modo: %s\n", modo == 0 ? "Secuencial" : "Paralelo");
    printf("Iteraciones: %d\n", iter);
    printf("Tiempo: %f segundos\n", end - start);

    // Liberación de memoria
    liberar_matriz(actual, N);
    liberar_matriz(siguiente, N);

    return 0;
}