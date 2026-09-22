cmake_minimum_required(VERSION 4.1)
project(Ex4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(autotest_headers)
include_directories(tests/autotest_headers)

add_executable(
        Activation.h
        Dense.h
        main.cpp
        Matrix.h
        MlpNetwork.h
        matrix.cpp
        Activation.cpp
        Dense.cpp
        MlpNetwork.cpp)
