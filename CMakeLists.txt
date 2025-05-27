cmake_minimum_required(VERSION 3.31)
project(DifusionCalor C)

set(CMAKE_C_STANDARD 11)

# Requiere OpenMP
find_package(OpenMP REQUIRED)

# Ejecutable
add_executable(DifusionCalor main.c)

# Si se encontró OpenMP, enlazarlo
if(OpenMP_C_FOUND)
    target_link_libraries(DifusionCalor OpenMP::OpenMP_C)
endif()