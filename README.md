# Simulación de Difusión de Calor con OpenMP
Hecho por: Alberto Valera, Samuel Muñoz y Mario Serrano

Este proyecto implementa una simulación de difusión de calor en una superficie bidimensional, cumpliendo con los requisitos del ejercicio final de la asignatura **Arquitectura de Computadores** de la Universidad Alfonso X el Sabio. Se incluye una versión secuencial y otra paralelizada con **OpenMP**, así como herramientas para compilarlo fácilmente con **CMake**.

## Comparación con la Práctica

|                         Requisito del Enunciado                              |                Implementación            |
|------------------------------------------------------------------------------|------------------------------------------|
| Inicializar matriz con bordes a 100°C e interior a 0°C                       | `inicializar_matriz()`                   |
| Simular difusión iterativa con parada por umbral o iteraciones máximas       | `main()` con bucle `do-while`            |
| Implementación secuencial como referencia                                    | Activada con `modo = 0`                  |
| Paralelización usando `#pragma omp parallel for`                             | Activada con `modo = 1`                  |
| Uso de `reduction` para cambio máximo entre iteraciones                      | `reduction(max:max_cambio)`              |
| Medición de tiempo con `omp_get_wtime()`                                     | Usado antes y después del bucle principal|
| Compilación con `gcc -fopenmp`                                               | Compatible (ver instrucciones abajo)     |
| Argumentos por línea de comandos: N, umbral, iteraciones                     | `main(int argc, char *argv[])`           |


## 🧠 Descripción del Problema

Se simula una placa metálica representada por una matriz `N x N`, donde cada celda representa la temperatura de un punto.  
En cada iteración, la temperatura se actualiza como el promedio de sus 4 celdas vecinas (arriba, abajo, izquierda, derecha).

La simulación se detiene si:
- El cambio máximo de temperatura entre iteraciones < `umbral`.
- Se alcanza el número máximo de iteraciones.

## Código Principal (`main.c`)

Incluye:

- Inicialización de matrices dinámicas
- Implementación secuencial y paralela
- Cálculo de cambio máximo con `fabs()`
- Medición de tiempo con `omp_get_wtime()`
- Liberación de memoria al finalizar

## Estructura del Proyecto
- main.c              # Código fuente


- CMakeLists.txt      # Configuración CMake


- README.md           # Este archivo

