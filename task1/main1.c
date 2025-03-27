#include "func.h"
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
        print_status(st);
        return 1;
    }

    char command[MAX_INPUT_LEN];
    char login[MAX_LOGIN];
    int pin;

    while (1) {
        printf("\nВведите команду (register, login, exit): ");
        get_string_input(command, sizeof(command), "");

        if (strcmp(command, "register") == 0) {
            while (1) {
                get_string_input(login, sizeof(login), "Введите логин (не более 6 символов): ");

                if (!is_login_valid(login)) {
                    printf("Ошибка: Логин должен содержать от 1 до 6 символов (a-z, 0-9).\n");
                    continue;
                }

                if (user_in_base(&base, login)) {
                    printf("Ошибка: Логин уже существует.\n");
                    continue;
                }

                break;
            }

            pin = get_integer_input(0, 100000, "Введите PIN-код (от 0 до 100000): ");

            st = registration(&base, login, pin);
            printf("Регистрация: ");
            print_status(st);
        }
        else if (strcmp(command, "login") == 0) {
            int aut = 1;
            while (1) {
                get_string_input(login, sizeof(login), "Введите логин: ");

                if (!user_in_base(&base, login)) {
                    printf("Ошибка: Логин не найден.\n");
                    aut = 0;
                    break;
                }

                break;
            }
            if (!aut) {
                continue;
            }
            pin = get_integer_input(0, 100000, "Введите PIN-код: ");

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
                    get_string_input(command, sizeof(command), "");

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
                        if (sscanf(command + 8, "%10s %2s", date, flag) == 2) {
                            calculateElapsedTime(date, flag);
                        } else {
                            printf("Ошибка: Используйте формат: Howmuch <дата> <флаг>\n");
                        }
                    }
                    else if (strncmp(command, "Sanctions", 9) == 0) {
                        char target_user[MAX_LOGIN];
                        int max_requests;
                        if (sscanf(command + 10, "%s %d", target_user, &max_requests) == 2) {
                            int confirmation = get_integer_input(0, INT_MAX, "Для подтверждения введите код: ");
                            st = set_sanctions(&base, target_user, max_requests, confirmation);
                            if (st == SUCCESS) {
                                printf("Ограничения установлены: %s не сможет выполнить более %d запросов.\n",
                                       target_user, max_requests);
                            } else {
                                printf("Ошибка: ");
                                print_status(st);
                            }
                        } else {
                            printf("Ошибка: Используйте формат: Sanctions <username> <number>\n");
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