cmake_minimum_required(VERSION 3.22)
project(sistpr_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(task1 task1/func.c
        task1/header.h
)
add_executable(main1 task1/main1.c)
