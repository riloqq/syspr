//
// Created by Олеся on 30.03.2025.
//
#include "func.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <максимальное количество людей>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int max_people = atoi(argv[1]);
    if (max_people <= 0) {
        fprintf(stderr, "Максимальное количество людей должно быть положительным числом\n");
        return EXIT_FAILURE;
    }

    Bathroom bathroom;
    bathroom_init(&bathroom, max_people);

    const int total_threads = 10;
    pthread_t threads[total_threads];

    for (int i = 0; i < total_threads; i++) {
        if (i % 2 == 0) {
            pthread_create(&threads[i], NULL, woman_thread, &bathroom);
        } else {
            pthread_create(&threads[i], NULL, man_thread, &bathroom);
        }
    }

    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    bathroom_destroy(&bathroom);
    return EXIT_SUCCESS;
}