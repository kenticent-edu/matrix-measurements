cmake_minimum_required(VERSION 3.16)
project(matrix)

set(CMAKE_CXX_STANDARD 14)

add_executable(matrix src/main.cpp src/Matrix.h src/time_measurement.h src/MeasureExecution.h)
