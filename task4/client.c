#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_CLIENT_TO_SERVER "/tmp/fifo_client_to_server"
#define FIFO_SERVER_TO_CLIENT "/tmp/fifo_server_to_client"
#define MAX_CMD_LEN 128
#define MAX_RESPONSE_LEN 512

int safe_print(const char* msg) {
    return write(STDOUT_FILENO, msg, strlen(msg));
}

int safe_error(const char* msg) {
    return write(STDERR_FILENO, msg, strlen(msg));
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        safe_error("Usage: ./client <commands_file>\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        safe_error("Error: Unable to open commands file\n");
        return 1;
    }

    char cmd[MAX_CMD_LEN];
    char response[MAX_RESPONSE_LEN];
    int game_over = 0;

    while (!game_over && fgets(cmd, sizeof(cmd), file)) {
        cmd[strcspn(cmd, "\r\n")] = '\0';

        int fd_write = open(FIFO_CLIENT_TO_SERVER, O_WRONLY);
        if (fd_write == -1) {
            safe_error("Error: Unable to open write FIFO\n");
            fclose(file);
            return 1;
        }

        if (write(fd_write, cmd, strlen(cmd)) == -1) {
            safe_error("Error: Writing to FIFO failed\n");
            close(fd_write);
            fclose(file);
            return 1;
        }
        close(fd_write);

        int fd_read = open(FIFO_SERVER_TO_CLIENT, O_RDONLY);
        if (fd_read == -1) {
            safe_error("Error: Unable to open read FIFO\n");
            fclose(file);
            return 1;
        }

        int n = read(fd_read, response, sizeof(response) - 1);
        close(fd_read);

        if (n > 0) {
            response[n] = '\0';
            safe_print(">> ");
            safe_print(response);

            if (strstr(response, "Fail:") != NULL) {
                safe_print("Game lost. Client exiting.\n");
                game_over = 1;
            } else if (strstr(response, "Success:") != NULL) {
                safe_print("Game won. Client exiting.\n");
                game_over = 1;
            }
        } else {
            safe_error("Error: No response from server.\n");
            break;
        }

        sleep(1);
    }

    fclose(file);
    return 0;
}
