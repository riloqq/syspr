//
// Created by Олеся on 12.04.2025.
//

#ifndef SISTPR_4_FUNC_H
#define SISTPR_4_FUNC_H

#include "header.h"

int validate_args(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <file1> [file2...] <command> [command_args]\n", argv[0]);
        return 0;
    }
    return 1;
}

FileInfo* open_files(int num_files, char* file_paths[], int* success) {
    FileInfo* files = malloc(num_files * sizeof(FileInfo));
    if (!files) {
        printf("Memory allocation failed\n");
        *success = 0;
        return NULL;
    }

    for (int i = 0; i < num_files; i++) {
        files[i].path = strdup(file_paths[i]);
        files[i].file = fopen(file_paths[i], "rb");
        if (!files[i].file) {
            printf("Cannot open file: %s\n", file_paths[i]);
            free(files[i].path);
            for (int j = 0; j < i; j++) {
                fclose(files[j].file);
                free(files[j].path);
            }
            free(files);
            *success = 0;
            return NULL;
        }
    }

    *success = 1;
    return files;
}

void close_files(FileInfo* files, int num_files) {
    for (int i = 0; i < num_files; i++) {
        if (files[i].file) fclose(files[i].file);
        if (files[i].path) free(files[i].path);
    }
    free(files);
}

void execute_xorN(FileInfo* files, int num_files, int N) {
    if (N < 2 || N > 6) {
        printf("Invalid N for xorN, must be 2 to 6\n");
        return;
    }

    size_t block_bits = 1 << N;
    size_t block_bytes = (block_bits + 7) / 8;

    uint8_t* result = calloc(block_bytes, 1);
    if (!result) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < num_files; i++) {
        rewind(files[i].file);
        uint8_t* buffer = malloc(block_bytes);
        if (!buffer) {
            printf("Memory allocation failed\n");
            free(result);
            return;
        }

        size_t read_bytes;
        while ((read_bytes = fread(buffer, 1, block_bytes, files[i].file)) > 0) {
            if (read_bytes < block_bytes) {
                memset(buffer + read_bytes, 0, block_bytes - read_bytes);
            }
            for (size_t j = 0; j < block_bytes; j++) {
                result[j] ^= buffer[j];
            }
        }
        free(buffer);
    }

    printf("XOR result: ");
    if (block_bits % 8 != 0) {
        size_t bits = block_bits % 8;
        uint8_t mask = (1 << bits) - 1;
        printf("%02x", result[0] & mask);
    } else {
        for (size_t j = 0; j < block_bytes; j++) {
            printf("%02x", result[j]);
        }
    }
    printf("\n");

    free(result);
}

void execute_mask(FileInfo* files, int num_files, const char* mask_str) {
    uint32_t mask;
    if (sscanf(mask_str, "%x", &mask) != 1) {
        printf("Invalid mask value\n");
        return;
    }

    uint32_t val;
    int count = 0;

    for (int i = 0; i < num_files; i++) {
        rewind(files[i].file);
        while (fread(&val, sizeof(uint32_t), 1, files[i].file) == 1) {
            if ((val & mask) == mask) {
                count++;
            }
        }
    }

    printf("Matched values: %d\n", count);
}

void execute_copyN(FileInfo* files, int num_files, int N) {
    if (N <= 0) {
        printf("Invalid N for copyN\n");
        return;
    }

    for (int i = 0; i < num_files; i++) {
        for (int n = 1; n <= N; n++) {
            pid_t pid = fork();
            if (pid == -1) {
                printf("Fork failed\n");
                return;
            } else if (pid == 0) {
                char out_path[1024];
                snprintf(out_path, sizeof(out_path), "%s_%d", files[i].path, n);

                FILE* src = fopen(files[i].path, "rb");
                if (!src) {
                    printf("Failed to open source file: %s\n", files[i].path);
                    return;
                }

                FILE* dst = fopen(out_path, "wb");
                if (!dst) {
                    printf("Failed to create output file: %s\n", out_path);
                    fclose(src);
                    return;
                }

                char buffer[4096];
                size_t bytes;
                while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                    fwrite(buffer, 1, bytes, dst);
                }

                fclose(src);
                fclose(dst);
                return;
            }
        }

        for (int n = 1; n <= N; n++) {
            wait(NULL);
        }
    }

    printf("Copied %d times\n", N);
}

void execute_find(FileInfo* files, int num_files, const char* str) {
    int found = 0;
    for (int i = 0; i < num_files; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            printf("Fork failed\n");
            return;
        } else if (pid == 0) {
            FILE* f = fopen(files[i].path, "r");
            if (!f) {
                printf("Cannot open file: %s\n", files[i].path);
                return;
            }

            char line[MAX_STRING_LEN];
            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, str)) {
                    printf("Found in %s\n", files[i].path);
                    fclose(f);
                    return;
                }
            }

            fclose(f);
            return;
        }
    }

    for (int i = 0; i < num_files; i++) {
        wait(NULL);
    }

    if (!found) {
        printf("Not found: %s\n", str);
    }
}

#endif //SISTPR_4_FUNC_H
