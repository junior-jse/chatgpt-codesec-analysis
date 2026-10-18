#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 64

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

    for (const char *p = password; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;

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

int main(void) {
    char password[MAX_PASSWORD_LENGTH + 2];

    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strnlen(password, sizeof(password));
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }

    if (is_valid_password(password)) {
        printf("Password is valid\n");
        return 0;
    } else {
        printf("Password is invalid\n");
        return 1;
    }
}