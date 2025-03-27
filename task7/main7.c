//
// Created by Олеся on 26.03.2025.
//
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH 1024

void list_directory(const char *dirname) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char fullpath[MAX_PATH];

    if ((dir = opendir(dirname)) == NULL) {
        fprintf(stderr, "Ошибка открытия каталога %s: %s\n", dirname, strerror(errno));
        return;
    }

    printf("Содержимое каталога %s:\n", dirname);
    printf("%-20s %-10s %-10s %s\n", "Имя", "Размер", "INODE", "Первый блок");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirname, entry->d_name);

        if (lstat(fullpath, &file_stat) == -1) {
            fprintf(stderr, "Ошибка получения информации о %s: %s\n", fullpath, strerror(errno));
            continue;
        }

        printf("%-20s %-10ld %-10lu ", entry->d_name, file_stat.st_size, file_stat.st_ino);

        if (S_ISREG(file_stat.st_mode)) {
            printf("%lu\n", file_stat.st_blocks);
        } else {
            printf("-\n");
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s каталог1 [каталог2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        list_directory(argv[i]);
        printf("\n");
    }

    return EXIT_SUCCESS;
}