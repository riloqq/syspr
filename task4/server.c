#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_CLIENT_TO_SERVER "/tmp/fifo_client_to_server"
#define FIFO_SERVER_TO_CLIENT "/tmp/fifo_server_to_client"
#define MAX_CMD_LEN 128
#define MAX_RESPONSE_LEN 256

#define OBJ_NONE 0
#define OBJ_WOLF 1
#define OBJ_GOAT 2
#define OBJ_CABBAGE 3

typedef struct {
    int farmer;
    int wolf;
    int goat;
    int cabbage;
    int boat;
} GameState;

const char* get_object_name(int obj) {
    switch (obj) {
        case OBJ_WOLF: return "wolf";
        case OBJ_GOAT: return "goat";
        case OBJ_CABBAGE: return "cabbage";
        default: return "none";
    }
}

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
        default: return NULL;
    }
}

int check_fail(GameState* s) {
    if (s->farmer != s->wolf && s->wolf == s->goat)
        return 1;
    if (s->farmer != s->goat && s->goat == s->cabbage)
        return 2;
    return 0;
}

int check_success(GameState* s) {
    return (
            s->farmer == 1 &&
            s->wolf == 1 &&
            s->goat == 1 &&
            s->cabbage == 1 &&
            s->boat == OBJ_NONE
    );
}

void process_command(GameState* s, const char* cmd, char* response) {
    response[0] = '\0';

    if (strncmp(cmd, "take ", 5) == 0) {
        char obj_name[20];
        int obj = OBJ_NONE;
        sscanf(cmd + 5, "%s", obj_name);

        if (strcmp(obj_name, "wolf") == 0) obj = OBJ_WOLF;
        else if (strcmp(obj_name, "goat") == 0) obj = OBJ_GOAT;
        else if (strcmp(obj_name, "cabbage") == 0) obj = OBJ_CABBAGE;

        if (obj == OBJ_NONE) {
            snprintf(response, MAX_RESPONSE_LEN, "Error: Unknown object\n");
            return;
        }

        if (s->boat != OBJ_NONE) {
            snprintf(response, MAX_RESPONSE_LEN, "Error: Boat already full\n");
            return;
        }

        int* pos = get_position(s, obj);
        if (pos == NULL || *pos != s->farmer) {
            snprintf(response, MAX_RESPONSE_LEN, "Error: Object on other shore\n");
            return;
        }

        s->boat = obj;
        *pos = -1;
        snprintf(response, MAX_RESPONSE_LEN, "OK: Took %s\n", get_object_name(obj));
    }
    else if (strcmp(cmd, "put") == 0) {
        if (s->boat == OBJ_NONE) {
            snprintf(response, MAX_RESPONSE_LEN, "Error: Boat is empty\n");
            return;
        }
        int* pos = get_position(s, s->boat);
        if (pos != NULL) {
            *pos = s->farmer;
            snprintf(response, MAX_RESPONSE_LEN, "OK: Put %s\n", get_object_name(s->boat));
        } else {
            snprintf(response, MAX_RESPONSE_LEN, "Error: Invalid object in boat\n");
        }
        s->boat = OBJ_NONE;
    }
    else if (strcmp(cmd, "move") == 0) {
        s->farmer = !s->farmer;
        snprintf(response, MAX_RESPONSE_LEN, "OK: Farmer moved to %s shore\n", s->farmer ? "right" : "left");
    }
    else {
        snprintf(response, MAX_RESPONSE_LEN, "Error: Unknown command\n");
        return;
    }

    int fail = check_fail(s);
    if (fail == 1) {
        strncat(response, "Fail: Wolf ate the goat!\n", MAX_RESPONSE_LEN - strlen(response) - 1);
    } else if (fail == 2) {
        strncat(response, "Fail: Goat ate the cabbage!\n", MAX_RESPONSE_LEN - strlen(response) - 1);
    } else if (check_success(s)) {
        strncat(response, "Success: All safely delivered!\n", MAX_RESPONSE_LEN - strlen(response) - 1);
    }
}

int main(void) {
    GameState state;
    init_state(&state);

    mkfifo(FIFO_CLIENT_TO_SERVER, 0666);
    mkfifo(FIFO_SERVER_TO_CLIENT, 0666);

    char cmd[MAX_CMD_LEN];
    char response[MAX_RESPONSE_LEN];

    write(STDOUT_FILENO, "[Server] Game started. Waiting for commands...\n", 47);

    int running = 1;
    while (running) {
        int fd_read = open(FIFO_CLIENT_TO_SERVER, O_RDONLY);
        int n = read(fd_read, cmd, MAX_CMD_LEN - 1);
        close(fd_read);

        if (n <= 0)
            continue;

        cmd[n] = '\0';

        write(STDOUT_FILENO, "[Server] Received command: ", 28);
        write(STDOUT_FILENO, cmd, strlen(cmd));
        write(STDOUT_FILENO, "\n", 1);

        process_command(&state, cmd, response);

        write(STDOUT_FILENO, "[Server] Response: ", 19);
        write(STDOUT_FILENO, response, strlen(response));

        int fd_write = open(FIFO_SERVER_TO_CLIENT, O_WRONLY);
        write(fd_write, response, strlen(response));
        close(fd_write);

        if (strstr(response, "Fail") != NULL) {
            write(STDOUT_FILENO, "[Server] Game over. Player lost!\n", 33);
            running = 0;
        } else if (strstr(response, "Success") != NULL) {
            write(STDOUT_FILENO, "[Server] Game over. Player won!\n", 32);
            running = 0;
        }
    }

    unlink(FIFO_CLIENT_TO_SERVER);
    unlink(FIFO_SERVER_TO_CLIENT);

    write(STDOUT_FILENO, "[Server] Exiting...\n", 21);

    return 0;
}
