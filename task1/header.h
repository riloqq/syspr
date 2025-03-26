//
// Created by Олеся on 21.03.2025.
//

#ifndef SISTPR_4_HEADER_H
#define SISTPR_4_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_LOGIN 7
#define SECRETCODE 12345

typedef struct {
    char login[MAX_LOGIN];
    int pincode;
    int sunctions;
    int request_count;
} User;

typedef struct {
    User* users;
    size_t capacity;
    size_t size;
} User_base;

typedef enum {
    SUCCESS = 0,
    LOGIN_ALREADY_IN_BASE = 1,
    MEMORY_ERR = 2,
    LOGIN_NOT_IN_BASE = 3,
    WRONG_PASSWORD = 4
} status;

int user_in_base(User_base* base, const char* lg);
status autorisation(User_base* base, const char* lg, int pin);
status init_user_base(User_base* base, size_t init_capacity);
status registration(User_base* base, const char* lg, int pin);
void free_user_base(User_base* base);
int is_pincode_valid(int pin);
void getCurrentTime();
void getCurrentDate();
void calculateElapsedTime(const char *date, const char *flag);
int is_leap_year(int year);
int is_valid_date_format(const char* date_str);
status set_sanctions(User_base* base, const char* target_user, int max_requests, int confirmation_code);
int is_login_valid(const char* login);

#endif //SISTPR_4_HEADER_H
