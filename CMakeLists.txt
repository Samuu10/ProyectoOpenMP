cmake_minimum_required(VERSION 3.31)
project(DifusionCalor C)

set(CMAKE_C_STANDARD 11)

add_executable(DifusionCalor main.c)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    target_link_libraries(DifusionCalor PUBLIC OpenMP::OpenMP_C)
endif()
