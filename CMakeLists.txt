cmake_minimum_required(VERSION 3.18.0)
project(aoc2024 VERSION 0.1.0 LANGUAGES  CXX CUDA)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CUDA_STANDARD 20)
# set(CMAKE_CUDA_ARCHITECTURES 61)

add_executable(day01 01.cpp)
add_executable(day02 02.cpp)
add_executable(day03 03.cpp)
add_executable(day04 04.cpp)
add_executable(day05 05.cpp)
add_executable(day06 06.cpp)
add_executable(day07 07.cpp)
add_executable(day08 08.cpp)
add_executable(day09 09.cpp)
add_executable(day10 10.cpp)
add_executable(day11 11.cpp)
add_executable(day12 12.cpp)
add_executable(day13 13.cpp)
add_executable(day14 14.cpp)
add_executable(day15 15.cpp)
add_executable(day16 16.cpp)
add_executable(day17 17.cpp)

find_package(fmt)
target_link_libraries(day17 fmt::fmt)

add_executable(day17cu 17.cu)

add_executable(day18 18.cpp)
add_executable(day19 19.cpp)
add_executable(day20 20.cpp)
add_executable(day21 21.cpp)
add_executable(day22 22.cpp)
add_executable(day23 23.cpp)
add_executable(day24 24.cpp)
add_executable(day25 25.cpp)
