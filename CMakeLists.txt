cmake_minimum_required(VERSION 3.27)
project(CPUvGPU)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPUvGPU main.cpp
        saxpy/saxpy.hpp
        saxpy/saxpy_test.cpp
        CPU/cpu_simulator.hpp
        GPU/gpu_simulator.hpp
        monte_carlo/monte_carlo.hpp
        monte_carlo/monte_carlo_test.cpp)
