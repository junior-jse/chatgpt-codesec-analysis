#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool is_valid_password(const char *password) {
    if (password == NULL) {
        return false;
    }

    size_t len = strlen(password);
    if (len < 8 || len > 128) {
        return false;
    }

    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;

    for (size_t i = 0; i < len; i++) {
        unsigned char c = password[i];
        if (isupper(c)) {
            has_upper = true;
        } else if (islower(c)) {
            has_lower = true;
        } else if (isdigit(c)) {
            has_digit = true;
        } else if (ispunct(c)) {
            has_special = true;
        }
    }

    return has_upper && has_lower && has_digit && has_special;
}

int main(void) {
    const char *test_passwords[] = {
        "WeakPass1",
        "NoSpecial123",
        "nouppercase1!",
        "NOLOWERCASE1!",
        "NoDigitHere!!",
        "Short1!",
        "ValidPass1!",
        NULL
    };

    for (size_t i = 0; test_passwords[i] != NULL; i++) {
        bool valid = is_valid_password(test_passwords[i]);
        printf("Password: %-15s Valid: %s\n", test_passwords[i], valid ? "true" : "false");
    }

    bool null_test = is_valid_password(NULL);
    printf("Password: NULL             Valid: %s\n", null_test ? "true" : "false");

    return 0;
}