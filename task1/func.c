//
// Created by Олеся on 21.03.2025.
//
#pragma once
#include "header.h"

int user_in_base(User_base* base, const char* lg) {
    if (base->size == 0 || base->users == NULL) {
        return 0;
    }
    for (size_t i = 0; i < base->size; i++) {
        if (!strcmp(lg, base->users[i].login)) {
            return 1;
        }
    }
    return 0;
}
int is_pincode_valid(int pin) {
    if (0 <= pin && pin <= 100000) {
        return 1;
    }
    return 0;
}
status registration(User_base* base, const char* lg, int pin) {

    if (!is_pincode_valid(pin)) {
        return WRONG_PASSWORD;
    }
    if (base->size >= base->capacity) {
        base->capacity *= 2;
        void* new_mem = realloc(base->users, base->capacity * sizeof(User));
        if (new_mem == NULL) {
            return MEMORY_ERR;
        }
        base->users = (User*)new_mem;
    }
    snprintf(base->users[base->size].login, MAX_LOGIN, "%s", lg);
    base->users[base->size].pincode = pin;
    base->users[base->size].sunctions = 0;
    base->size++;
    return SUCCESS;
}

status autorisation(User_base* base, const char* lg, int pin) {

    if (!is_pincode_valid(pin)) {
        return WRONG_PASSWORD;
    }
    for (size_t i = 0; i < base->size; i++) {
        if (!strcmp(base->users[i].login, lg)) {
            if (base->users[i].pincode == pin) {
                base->users[i].request_count = 0;
                return SUCCESS;
            }
            return WRONG_PASSWORD;
        }
    }
    return LOGIN_ALREADY_IN_BASE;
}

status init_user_base(User_base* base, size_t init_capacity) {
    base->users = (User*) malloc(init_capacity * sizeof(User));
    if (base->users == NULL) {
        return MEMORY_ERR;
    }
    base->capacity = init_capacity;
    base->size = 0;
    return SUCCESS;
}

void free_user_base(User_base* base) {
    free(base->users);
    base->users = NULL;
    base->capacity = 0;
    base->size = 0;
}

void getCurrentTime() {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Текущее время: %02d:%02d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void getCurrentDate() {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Текущая дата: %02d:%02d:%04d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
}

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int is_valid_date_format(const char* date_str) {
    if (strlen(date_str) != 10) {
        return 0;
    }

    if (date_str[2] != ':' || date_str[5] != ':') {
        return 0;
    }

    int day = (date_str[0] - '0') * 10 + (date_str[1] - '0');
    int month = (date_str[3] - '0') * 10 + (date_str[4] - '0');
    int year = (date_str[6] - '0') * 1000 + (date_str[7] - '0') * 100 +
               (date_str[8] - '0') * 10 + (date_str[9] - '0');

    if (day < 1 || month < 1 || month > 12 || year < 0) {
        return 0;
    }

    int max_days;
    if (month == 2) {
        max_days = is_leap_year(year) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        max_days = 30;
    } else {
        max_days = 31;
    }

    if (day > max_days) {
        return 0;
    }
    return 1;
}

void calculateElapsedTime(const char *date, const char *flag) {
    if (!is_valid_date_format(date)) {
        printf("Ошибка в формате даты. Используйте формат дд:мм:гггг.\n");
        return;
    }

    struct tm specifiedTime = {0};
    time_t now;
    double seconds;

    sscanf(date, "%d:%d:%d", &specifiedTime.tm_mday, &specifiedTime.tm_mon, &specifiedTime.tm_year);

    specifiedTime.tm_mon -= 1;
    specifiedTime.tm_year -= 1900;

    time(&now);

    time_t specifiedSeconds = mktime(&specifiedTime);

    seconds = difftime(now, specifiedSeconds);

    if (strcmp(flag, "-s") == 0) {
        printf("Прошло секунд: %.0f\n", seconds);
    } else if (strcmp(flag, "-m") == 0) {
        printf("Прошло минут: %.0f\n", seconds / 60);
    } else if (strcmp(flag, "-h") == 0) {
        printf("Прошло часов: %.0f\n", seconds / 3600);
    } else if (strcmp(flag, "-y") == 0) {
        printf("Прошло лет: %.0f\n", seconds / (3600 * 24 * 365));
    } else {
        printf("Неверный флаг. Используйте -s, -m, -h или -y.\n");
    }
}

status set_sanctions(User_base* base, const char* target_user, int max_requests, int confirmation_code) {
    if (confirmation_code != SECRETCODE) {
        return WRONG_PASSWORD;
    }

    for (size_t i = 0; i < base->size; i++) {
        if (strcmp(base->users[i].login, target_user) == 0) {
            base->users[i].sunctions = max_requests;
            base->users[i].request_count = 0;
            return SUCCESS;
        }
    }

    return LOGIN_NOT_IN_BASE;
}

int is_login_valid(const char* login) {
    size_t len = strlen(login);
    if (len == 0 || len > MAX_LOGIN - 1) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum(login[i])) {
            return 0;
        }
    }
    return 1;
}
