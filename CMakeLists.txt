# Establece la versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.31)

# Define el nombre del proyecto y el lenguaje principal
project(DifusionCalor C)

# Establece el estándar de C requerido para compilar
set(CMAKE_C_STANDARD 11)

# Agrega un ejecutable a construir a partir del archivo main.c
add_executable(DifusionCalor main.c)

# Busca la librería OpenMP en el sistema
find_package(OpenMP REQUIRED)

# Si OpenMP fue encontrado, se enlaza al ejecutable
if(OpenMP_C_FOUND)
    target_link_libraries(DifusionCalor PUBLIC OpenMP::OpenMP_C)
endif()
