#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <libgen.h>
#include <errno.h>
#include <locale.h>
#include <ctype.h>

#define FIFO_IN "/tmp/fifo_dir_request"
#define FIFO_OUT "/tmp/fifo_dir_response"
#define MAX_PATH 4096
#define MAX_BUF 16384
#define MAX_PATHS 128

void list_files(const char* dir_path, char* out_buf, size_t* out_len) {
    DIR* dir = opendir(dir_path);
    if (!dir) {
        *out_len += snprintf(out_buf + *out_len, MAX_BUF - *out_len,
                             "Error: Cannot open directory '%s' (%s)\n---\n",
                             dir_path, strerror(errno));
        return;
    }

    *out_len += snprintf(out_buf + *out_len, MAX_BUF - *out_len, "%s:\n", dir_path);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            *out_len += snprintf(out_buf + *out_len, MAX_BUF - *out_len, "%s\n", entry->d_name);
        }
    }

    *out_len += snprintf(out_buf + *out_len, MAX_BUF - *out_len, "---\n");
    closedir(dir);
}

int is_duplicate(char* dir_list[], int count, const char* dir) {
    for (int i = 0; i < count; i++) {
        if (strcmp(dir_list[i], dir) == 0) return 1;
    }
    return 0;
}

void process_paths(char* paths[], int count, char* response) {
    char* processed_dirs[MAX_PATHS];
    int dir_count = 0;
    size_t resp_len = 0;

    for (int i = 0; i < count; i++) {
        char* path = paths[i];
        if (path == NULL) continue;

        while (isspace(*path)) path++;

        size_t len = strlen(path);
        while (len > 0 && isspace(path[len-1])) {
            path[--len] = '\0';
        }

        if (len == 0) continue;

        struct stat st;
        if (stat(path, &st)) {
            resp_len += snprintf(response + resp_len, MAX_BUF - resp_len,
                                 "Error: Cannot access path '%s' (%s)\n---\n",
                                 path, strerror(errno));
            continue;
        }

        char actual_dir[MAX_PATH];
        if (S_ISDIR(st.st_mode)) {
            strncpy(actual_dir, path, MAX_PATH - 1);
        } else {
            char* temp = strdup(path);
            char* dir = dirname(temp);
            strncpy(actual_dir, dir, MAX_PATH - 1);
            free(temp);
        }
        actual_dir[MAX_PATH - 1] = '\0';

        if (!is_duplicate(processed_dirs, dir_count, actual_dir)) {
            processed_dirs[dir_count++] = strdup(actual_dir);
            list_files(actual_dir, response, &resp_len);
        }
    }

    for (int i = 0; i < dir_count; i++) {
        free(processed_dirs[i]);
    }
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    mkfifo(FIFO_IN, 0666);
    mkfifo(FIFO_OUT, 0666);

    printf("[Server] Ready to receive paths...\n");

    char input_buf[MAX_BUF];
    char response[MAX_BUF];

    while (1) {
        int fd_in = open(FIFO_IN, O_RDONLY);
        if (fd_in < 0) {
            perror("open FIFO_IN");
            continue;
        }

        ssize_t n = read(fd_in, input_buf, MAX_BUF - 1);
        close(fd_in);

        if (n <= 0) continue;

        input_buf[n] = '\0';
        memset(response, 0, MAX_BUF);

        char* paths[MAX_PATHS] = {0};
        int path_count = 0;
        char* line = input_buf;

        while (path_count < MAX_PATHS && *line != '\0') {
            char* end = strchr(line, '\n');
            if (end) *end = '\0';

            paths[path_count] = strdup(line);
            if (paths[path_count] == NULL) {
                perror("strdup");
                continue;
            }

            path_count++;

            if (!end) break;
            line = end + 1;
        }

        process_paths(paths, path_count, response);

        for (int i = 0; i < path_count; i++) {
            free(paths[i]);
        }

        int fd_out = open(FIFO_OUT, O_WRONLY);
        if (fd_out >= 0) {
            write(fd_out, response, strlen(response));
            close(fd_out);
        }
    }

    unlink(FIFO_IN);
    unlink(FIFO_OUT);

    return 0;
}