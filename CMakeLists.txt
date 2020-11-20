cmake_minimum_required(VERSION 3.15)
project(ACO_Algorithm C)

set(CMAKE_C_STANDARD 99)

add_executable(ACO_Algorithm main.c ACO.c ACO.h)