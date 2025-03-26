#include "func.c"
#pragma once

void print_status(status st) {
    switch (st) {
        case SUCCESS:
            printf("SUCCESS\n");
            break;
        case LOGIN_ALREADY_IN_BASE:
            printf("Ошибка: Логин уже существует.\n");
            break;
        case LOGIN_NOT_IN_BASE:
            printf("Ошибка: Логин не найден.\n");
            break;
        case WRONG_PASSWORD:
            printf("Ошибка: Неверный PIN-код или код подтверждения.\n");
            break;
        case MEMORY_ERR:
            printf("Ошибка: Недостаточно памяти.\n");
            break;
        default:
            printf("Неизвестная ошибка.\n");
            break;
    }
}


int main() {
    User_base base;
    status st;

    st = init_user_base(&base, 2);
    if (st != SUCCESS) {
        printf("Ошибка при инициализации базы пользователей.\n");
        return 1;
    }

    char command[100];
    char login[MAX_LOGIN];
    int pin;

    while (1) {
        printf("\nВведите команду (register, login, exit): ");
        scanf("%s", command);

        if (strcmp(command, "register") == 0) {
            while (1) {
                printf("Введите логин (не более %d символов): ", MAX_LOGIN - 1);
                scanf("%s", login);

                if (is_login_valid(login)) {
                    break;
                } else {
                    printf("Ошибка: Логин должен содержать не более %d символов.\n", MAX_LOGIN - 1);
                }
            }

            if (user_in_base(&base, login)) {
                st = LOGIN_ALREADY_IN_BASE;
                print_status(st);
                continue;
            }

            printf("Введите PIN-код (от 0 до 100000): ");
            scanf("%d", &pin);

            st = registration(&base, login, pin);
            printf("Регистрация: ");
            print_status(st);
        }
        else if (strcmp(command, "login") == 0) {
            while (1) {
                printf("Введите логин (не более %d символов): ", MAX_LOGIN - 1);
                scanf("%s", login);

                if (is_login_valid(login)) {
                    break;
                } else {
                    printf("Ошибка: Логин должен содержать не более %d символов.\n", MAX_LOGIN - 1);
                }
            }
            if (!user_in_base(&base, login)) {
                st = LOGIN_NOT_IN_BASE;
                print_status(st);
                continue;
            }
            printf("Введите PIN-код: ");
            scanf("%d", &pin);

            st = autorisation(&base, login, pin);
            printf("Авторизация: ");
            print_status(st);

            if (st == SUCCESS) {
                size_t user_index = 0;
                for (size_t i = 0; i < base.size; i++) {
                    if (strcmp(base.users[i].login, login) == 0) {
                        user_index = i;
                        break;
                    }
                }

                printf("Добро пожаловать, %s!\n", login);

                while (1) {
                    printf("\nВведите команду (Time, Date, Howmuch <дата> <флаг>, Sanctions <username> <number>, Logout): ");
                    scanf(" %[^\n]", command);

                    if (base.users[user_index].sunctions > 0) {
                        base.users[user_index].request_count++;
                        if (base.users[user_index].request_count > base.users[user_index].sunctions) {
                            printf("Превышено максимальное количество запросов (%d).\n",
                                   base.users[user_index].sunctions);
                            printf("Выход из системы...\n");
                            break;
                        }
                    }

                    if (strcmp(command, "Time") == 0) {
                        getCurrentTime();
                    }
                    else if (strcmp(command, "Date") == 0) {
                        getCurrentDate();
                    }
                    else if (strncmp(command, "Howmuch", 7) == 0) {
                        char date[11], flag[3];
                        if (sscanf(command, "Howmuch %10s %2s", date, flag) == 2) {
                            calculateElapsedTime(date, flag);
                        } else {
                            printf("Ошибка: Неверный формат команды. Используйте: Howmuch <дата> <флаг>\n");
                        }
                    }
                    else if (strncmp(command, "Sanctions", 9) == 0) {
                        char target_user[MAX_LOGIN];
                        int max_requests;
                        if (sscanf(command, "Sanctions %s %d", target_user, &max_requests) == 2) {
                            printf("Для подтверждения введите код: ");
                            int confirmation;
                            scanf("%d", &confirmation);

                            st = set_sanctions(&base, target_user, max_requests, confirmation);
                            if (st == SUCCESS) {
                                printf("Ограничения установлены: %s не сможет выполнить более %d запросов в сеансе.\n",
                                       target_user, max_requests);
                            } else {
                                printf("Ошибка при установке ограничений: ");
                                print_status(st);
                            }
                        } else {
                            printf("Ошибка: Неверный формат команды. Используйте: Sanctions username <number>\n");
                        }
                    }
                    else if (strcmp(command, "Logout") == 0) {
                        printf("Выход из системы...\n");
                        break;
                    }
                    else {
                        printf("Неизвестная команда. Попробуйте снова.\n");
                    }
                }
            }
        }
        else if (strcmp(command, "exit") == 0) {
            printf("Завершение программы.\n");
            break;
        }
        else {
            printf("Неизвестная команда. Попробуйте снова.\n");
        }
    }

    free_user_base(&base);
    printf("Память освобождена.\n");

    return 0;
}