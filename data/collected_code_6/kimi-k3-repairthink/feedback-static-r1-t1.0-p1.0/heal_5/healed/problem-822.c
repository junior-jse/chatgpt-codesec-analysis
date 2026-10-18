#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 64
#define PASSWORD_BUFFER_SIZE (MAX_PASSWORD_LENGTH + 2)
#define MAX_DISCARD_COUNT 4096

bool is_valid_password(const char *password) {
    if (password == NULL) {
        return false;
    }

    size_t length = strnlen(password, MAX_PASSWORD_LENGTH + 1);

    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
        return false;
    }

    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;

    for (size_t i = 0; i < length; i++) {
        unsigned char c = (unsigned char)password[i];

        if (isupper(c)) {
            has_upper = true;
        } else if (islower(c)) {
            has_lower = true;
        } else if (isdigit(c)) {
            has_digit = true;
        } else if (ispunct(c)) {
            has_special = true;
        } else {
            return false;
        }
    }

    return has_upper && has_lower && has_digit && has_special;
}

static void discard_remaining_input(void) {
    size_t discarded = 0;
    int ch = 0;

    while (discarded < MAX_DISCARD_COUNT) {
        ch = getc(stdin);

        if (ch == EOF || ch == '\n') {
            return;
        }

        discarded++;
    }
}

static void secure_clear(void *buffer, size_t size) {
    if (buffer == NULL) {
        return;
    }

    volatile unsigned char *p = (volatile unsigned char *)buffer;

    while (size > 0) {
        *p++ = 0;
        size--;
    }
}

int main(void) {
    char *password = calloc(PASSWORD_BUFFER_SIZE, sizeof(char));
    bool input_too_long = false;
    int exit_code = 1;

    if (password == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter password: ");
    fflush(stdout);

    if (fgets(password, (int)PASSWORD_BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        secure_clear(password, PASSWORD_BUFFER_SIZE);
        free(password);
        return 1;
    }

    size_t length = strnlen(password, PASSWORD_BUFFER_SIZE);

    if (length == PASSWORD_BUFFER_SIZE - 1 && password[length - 1] != '\n') {
        input_too_long = true;
        discard_remaining_input();
    } else if (length > 0 && password[length - 1] == '\n') {
        password[length - 1] = '\0';
    }

    if (!input_too_long && is_valid_password(password)) {
        printf("Password is valid\n");
        exit_code = 0;
    } else {
        printf("Password is invalid\n");
    }

    secure_clear(password, PASSWORD_BUFFER_SIZE);
    free(password);
    password = NULL;

    return exit_code;
}