#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_CLIENT_TO_SERVER "/tmp/fifo_client_to_server"
#define FIFO_SERVER_TO_CLIENT "/tmp/fifo_server_to_client"
#define MAX_CMD_LEN 128
#define MAX_RESPONSE_LEN 512

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <commands_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening command file");
        return EXIT_FAILURE;
    }

    char cmd[MAX_CMD_LEN];
    char response[MAX_RESPONSE_LEN];

    while (fgets(cmd, sizeof(cmd), file)) {
        cmd[strcspn(cmd, "\r\n")] = '\0';

        int fd_write = open(FIFO_CLIENT_TO_SERVER, O_WRONLY);
        if (fd_write == -1) {
            perror("Error opening write FIFO");
            fclose(file);
            return EXIT_FAILURE;
        }

        if (write(fd_write, cmd, strlen(cmd)) == -1) {
            perror("Error writing to FIFO");
            close(fd_write);
            fclose(file);
            return EXIT_FAILURE;
        }
        close(fd_write);

        int fd_read = open(FIFO_SERVER_TO_CLIENT, O_RDONLY);
        if (fd_read == -1) {
            perror("Error opening read FIFO");
            fclose(file);
            return EXIT_FAILURE;
        }

        int n = read(fd_read, response, sizeof(response) - 1);
        close(fd_read);

        if (n > 0) {
            response[n] = '\0';
            printf(">> %s\n", response);

            if (strstr(response, "Fail:") != NULL) {
                printf("Game lost. Client exiting.\n");
                break;
            }

            if (strstr(response, "Win:") != NULL) {
                printf("Game won. Client exiting.\n");
                break;
            }
        } else {
            fprintf(stderr, "No response from server.\n");
            break;
        }

        sleep(1);
    }

    fclose(file);
    return EXIT_SUCCESS;
}
