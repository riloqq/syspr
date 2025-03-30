//
// Created by Олеся on 30.03.2025.
//

#ifndef SISTPR_4_FUNC_H
#define SISTPR_4_FUNC_H
#include "header.h"

void bathroom_init(Bathroom *bathroom, int max_people) {
    bathroom->state = EMPTY;
    bathroom->count = 0;
    bathroom->max_people = max_people;
    pthread_mutex_init(&bathroom->mutex, NULL);
    pthread_cond_init(&bathroom->cond, NULL);
}

void bathroom_destroy(Bathroom *bathroom) {
    pthread_mutex_destroy(&bathroom->mutex);
    pthread_cond_destroy(&bathroom->cond);
}

bool woman_wants_to_enter(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);

    while ((bathroom->state == MEN_ONLY && bathroom->count > 0) ||
           bathroom->count >= bathroom->max_people) {
        pthread_cond_wait(&bathroom->cond, &bathroom->mutex);
    }

    if (bathroom->count == 0) {
        bathroom->state = WOMEN_ONLY;
    }

    bathroom->count++;
    printf("Женщина вошла. Состояние: %s, количество: %d\n",
           bathroom->state == WOMEN_ONLY ? "Только женщины" : "Пусто",
           bathroom->count);

    pthread_mutex_unlock(&bathroom->mutex);
    return true;
}

bool man_wants_to_enter(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);

    while ((bathroom->state == WOMEN_ONLY && bathroom->count > 0) ||
           bathroom->count >= bathroom->max_people) {
        pthread_cond_wait(&bathroom->cond, &bathroom->mutex);
    }

    if (bathroom->count == 0) {
        bathroom->state = MEN_ONLY;
    }

    bathroom->count++;
    printf("Мужчина вошел. Состояние: %s, количество: %d\n",
           bathroom->state == MEN_ONLY ? "Только мужчины" : "Пусто",
           bathroom->count);

    pthread_mutex_unlock(&bathroom->mutex);
    return true;
}

void woman_leaves(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);

    bathroom->count--;
    if (bathroom->count == 0) {
        bathroom->state = EMPTY;
    }

    printf("Женщина вышла. Состояние: %s, количество: %d\n",
           bathroom->state == WOMEN_ONLY ? "Только женщины" : "Пусто",
           bathroom->count);

    pthread_cond_broadcast(&bathroom->cond);
    pthread_mutex_unlock(&bathroom->mutex);
}

void man_leaves(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);

    bathroom->count--;
    if (bathroom->count == 0) {
        bathroom->state = EMPTY;
    }

    printf("Мужчина вышел. Состояние: %s, количество: %d\n",
           bathroom->state == MEN_ONLY ? "Только мужчины" : "Пусто",
           bathroom->count);

    pthread_cond_broadcast(&bathroom->cond);
    pthread_mutex_unlock(&bathroom->mutex);
}

void* woman_thread(void *arg) {
    Bathroom *bathroom = (Bathroom*)arg;
    woman_wants_to_enter(bathroom);
    sleep(1 + rand() % 3);
    woman_leaves(bathroom);
    return NULL;
}

void* man_thread(void *arg) {
    Bathroom *bathroom = (Bathroom*)arg;
    man_wants_to_enter(bathroom);
    sleep(1 + rand() % 3);
    man_leaves(bathroom);
    return NULL;
}

#endif //SISTPR_4_FUNC_H
