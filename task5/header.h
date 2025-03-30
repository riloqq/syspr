//
// Created by Олеся on 30.03.2025.
//

#ifndef SISTPR_4_HEADER_H
#define SISTPR_4_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    EMPTY,
    WOMEN_ONLY,
    MEN_ONLY
} BathroomState;

typedef struct {
    BathroomState state;
    int count;
    int max_people;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Bathroom;

void bathroom_init(Bathroom *bathroom, int max_people);
void bathroom_destroy(Bathroom *bathroom);
bool woman_wants_to_enter(Bathroom *bathroom);
bool man_wants_to_enter(Bathroom *bathroom);
void woman_leaves(Bathroom *bathroom);
void man_leaves(Bathroom *bathroom);
void* woman_thread(void *arg);
void* man_thread(void *arg);

#endif //SISTPR_4_HEADER_H
