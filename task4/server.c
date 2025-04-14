
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_CLIENT_TO_SERVER "/tmp/fifo_client_to_server"
#define FIFO_SERVER_TO_CLIENT "/tmp/fifo_server_to_client"

#define MAX_CMD_LEN 128
#define OBJ_NONE 0
#define OBJ_WOLF 1
#define OBJ_GOAT 2
#define OBJ_CABBAGE 3

const char* object_names[] = {"none", "wolf", "goat", "cabbage"};

typedef struct {
    int farmer;
    int wolf;
    int goat;
    int cabbage;
    int boat;
} GameState;

void init_state(GameState* s) {
    s->farmer = 0;
    s->wolf = 0;
    s->goat = 0;
    s->cabbage = 0;
    s->boat = OBJ_NONE;
}

int* get_position(GameState* s, int obj) {
    switch (obj) {
        case OBJ_WOLF: return &s->wolf;
        case OBJ_GOAT: return &s->goat;
        case OBJ_CABBAGE: return &s->cabbage;
    }
    return NULL;
}

int check_fail(GameState* s) {
    if (s->farmer != s->wolf && s->wolf == s->goat)
        return 1;
    if (s->farmer != s->goat && s->goat == s->cabbage)
        return 2;
    return 0;
}

void process_command(GameState* s, const char* cmd, char* response) {
    if (strncmp(cmd, "take ", 5) == 0) {
        char obj_name[20];
        sscanf(cmd + 5, "%s", obj_name);
        int obj = OBJ_NONE;
        for (int i = 1; i <= 3; ++i) {
            if (strcmp(obj_name, object_names[i]) == 0) {
                obj = i;
                break;
            }
        }
        if (obj == OBJ_NONE) {
            sprintf(response, "Error: Unknown object\n");
            return;
        }
        if (s->boat != OBJ_NONE) {
            sprintf(response, "Error: Boat already full\n");
            return;
        }
        int* pos = get_position(s, obj);
        if (*pos != s->farmer) {
            sprintf(response, "Error: Object on other shore\n");
            return;
        }
        s->boat = obj;
        *pos = -1;  // в лодке
        sprintf(response, "OK: Took %s\n", object_names[obj]);

    } else if (strcmp(cmd, "put") == 0) {
        if (s->boat == OBJ_NONE) {
            sprintf(response, "Error: Boat is empty\n");
            return;
        }
        int* pos = get_position(s, s->boat);
        *pos = s->farmer;
        sprintf(response, "OK: Put %s\n", object_names[s->boat]);
        s->boat = OBJ_NONE;

    } else if (strcmp(cmd, "move") == 0) {
        s->farmer = !s->farmer;
        sprintf(response, "OK: Farmer moved to %s shore\n", s->farmer ? "right" : "left");

    } else {
        sprintf(response, "Error: Unknown command\n");
        return;
    }

    int fail = check_fail(s);
    if (fail == 1) {
        sprintf(response + strlen(response), "Fail: Wolf ate the goat!\n");
    } else if (fail == 2) {
        sprintf(response + strlen(response), "Fail: Goat ate the cabbage!\n");
    } else if (
            s->farmer == 1 &&
            s->wolf == 1 &&
            s->goat == 1 &&
            s->cabbage == 1 &&
            s->boat == OBJ_NONE
            ) {
        sprintf(response + strlen(response), "Success: All safely delivered!\n");
    }

}

int main() {
    GameState state;
    init_state(&state);

    mkfifo(FIFO_CLIENT_TO_SERVER, 0666);
    mkfifo(FIFO_SERVER_TO_CLIENT, 0666);

    char cmd[MAX_CMD_LEN];
    char response[256];

    printf("[Server] Game started. Waiting for commands...\n");

    while (1) {
        int fd_read = open(FIFO_CLIENT_TO_SERVER, O_RDONLY);
        int n = read(fd_read, cmd, MAX_CMD_LEN);
        close(fd_read);

        if (n <= 0) continue;

        cmd[n] = '\0';

        printf("[Server] Received command: %s\n", cmd);

        process_command(&state, cmd, response);

        printf("[Server] Response: %s", response);

        int fd_write = open(FIFO_SERVER_TO_CLIENT, O_WRONLY);
        write(fd_write, response, strlen(response));
        close(fd_write);

        if (strstr(response, "Fail") != NULL) {
            printf("[Server] Game over. Player lost!\n");
            break;
        }

        if (strstr(response, "Success") != NULL) {
            printf("[Server] Game over. Player won!\n");
            break;
        }

    }

    unlink(FIFO_CLIENT_TO_SERVER);
    unlink(FIFO_SERVER_TO_CLIENT);

    printf("[Server] Exiting...\n");

    return 0;
}
