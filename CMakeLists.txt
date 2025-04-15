cmake_minimum_required(VERSION 3.22)
project(sistpr_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(main5 task5/main5.c)
add_executable(main1 task1/main1.c)
add_executable(main7 task7/main7.c)
add_executable(main3 task3/main3.c)
add_executable(server task4/server.c)
add_executable(client task4/client.c)
add_executable(main2 task2/main2.c)
add_executable(server6 task6/server6.c)
add_executable(client6 task6/client6.c)

target_sources(main1 PRIVATE task1/func.h task1/header.h)
target_sources(main5 PRIVATE task5/func.h task5/header.h)
target_sources(main3 PRIVATE task3/func.h task3/header.h)
target_sources(main2 PRIVATE task2/func.h task2/header.h)