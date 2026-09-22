cmake_minimum_required(VERSION 4.1)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled
        ex1/cipher.c
        ex1/main.c
        ex1/tests.c
)
