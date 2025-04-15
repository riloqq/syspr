#include "func.h"

int main(int argc, char* argv[]) {
    if (!validate_args(argc, argv)) return 1;

    int has_arg = 0;
    char* command = NULL;
    char* arg = NULL;
    int num_files = 0;
    int success = 0;

    if (argc >= 4 &&
        (strncmp(argv[argc - 2], "xor", 3) == 0 ||
         strncmp(argv[argc - 2], "copy", 4) == 0 ||
         strcmp(argv[argc - 2], "mask") == 0 ||
         strcmp(argv[argc - 2], "find") == 0)) {
        has_arg = 1;
    }

    if (has_arg) {
        command = argv[argc - 2];
        arg = argv[argc - 1];
        num_files = argc - 3;
    } else {
        command = argv[argc - 1];
        arg = NULL;
        num_files = argc - 2;
    }

    FileInfo* files = open_files(num_files, argv + 1, &success);
    if (!success) return 1;

    if (strncmp(command, "xor", 3) == 0) {
        int N = atoi(command + 3);
        execute_xorN(files, num_files, N);
    } else if (strcmp(command, "mask") == 0) {
        if (!arg) {
            printf("Missing mask argument\n");
            close_files(files, num_files);
            return 1;
        }
        execute_mask(files, num_files, arg);
    } else if (strncmp(command, "copy", 4) == 0) {
        int N = atoi(command + 4);
        execute_copyN(files, num_files, N);
    } else if (strcmp(command, "find") == 0) {
        if (!arg) {
            printf("Missing search string\n");
            close_files(files, num_files);
            return 1;
        }
        execute_find(files, num_files, arg);
    } else {
        printf("Unknown command: %s\n", command);
    }

    close_files(files, num_files);
    return 0;
}
