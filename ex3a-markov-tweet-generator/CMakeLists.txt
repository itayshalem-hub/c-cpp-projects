cmake_minimum_required(VERSION 4.1)
project(ex3A C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
include_directories(.)

add_executable(ex3A
        linked_list.c
        linked_list.h
        markov_chain.c
        markov_chain.h
        tweets_generator.c
        snakes_and_ladders.c
)

