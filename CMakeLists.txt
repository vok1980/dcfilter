cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project(DotCyclesFilter)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
