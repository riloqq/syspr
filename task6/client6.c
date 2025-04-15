#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_IN "/tmp/fifo_dir_request"
#define FIFO_OUT "/tmp/fifo_dir_response"
#define MAX_BUF 8192

int main(int argc, char* argv[]) {
    if (argc < 2) {
        write(STDERR_FILENO, "Usage: client <input_file_with_paths>\n", 39);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        write(STDERR_FILENO, "Error: Cannot open input file\n", 31);
        return 1;
    }

    char send_buf[MAX_BUF] = {0};
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\r\n') continue;
        strncat(send_buf, line, MAX_BUF - strlen(send_buf) - 1);
        if (send_buf[strlen(send_buf) - 1] != '\n') {
            strncat(send_buf, "\n", MAX_BUF - strlen(send_buf) - 1);
        }
    }

    fclose(file);

    int fd_out = open(FIFO_IN, O_WRONLY);
    if (fd_out < 0) {
        write(STDERR_FILENO, "Error: Cannot open write FIFO\n", 31);
        return 1;
    }

    write(fd_out, send_buf, strlen(send_buf));
    close(fd_out);

    char recv_buf[MAX_BUF] = {0};
    int fd_in = open(FIFO_OUT, O_RDONLY);
    if (fd_in < 0) {
        write(STDERR_FILENO, "Error: Cannot open read FIFO\n", 30);
        return 1;
    }

    ssize_t n = read(fd_in, recv_buf, MAX_BUF - 1);
    if (n > 0) {
        recv_buf[n] = '\0';
        write(STDOUT_FILENO, recv_buf, n);
    }

    close(fd_in);
    return 0;
}
