//
// Created by Олеся on 12.04.2025.
//

#ifndef SISTPR_4_HEADER_H
#define SISTPR_4_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_STRING_LEN 1024

typedef struct {
    char* path;
    FILE* file;
} FileInfo;

int validate_args(int argc, char* argv[]);
FileInfo* open_files(int num_files, char* file_paths[], int* success);
void close_files(FileInfo* files, int num_files);
void execute_xorN(FileInfo* files, int num_files, int N);
void execute_mask(FileInfo* files, int num_files, const char* mask_str);
void execute_copyN(FileInfo* files, int num_files, int N);
void execute_find(FileInfo* files, int num_files, const char* str);


#endif //SISTPR_4_HEADER_H
