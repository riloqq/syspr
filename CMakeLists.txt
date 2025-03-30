cmake_minimum_required(VERSION 3.22)
project(sistpr_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(main5 task5/main5.c)
add_executable(main1 task1/main1.c)
add_executable(main7 task7/main7.c)

target_sources(main1 PRIVATE task1/func.h task1/header.h)
target_sources(main5 PRIVATE task5/func.h task5/header.h)