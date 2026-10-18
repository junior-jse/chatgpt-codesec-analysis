#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool is_valid_password(const char *password) {
    size_t len;
    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;
    size_t i;

    if (password == NULL) {
        return false;
    }

    len = strlen(password);
    if (len < 8 || len > 128) {
        return false;
    }

    for (i = 0; i < len; i++) {
        if (isupper((unsigned char)password[i])) {
            has_upper = true;
        } else if (islower((unsigned char)password[i])) {
            has_lower = true;
        } else if (isdigit((unsigned char)password[i])) {
            has_digit = true;
        } else if (ispunct((unsigned char)password[i])) {
            has_special = true;
        }
    }

    return has_upper && has_lower && has_digit && has_special;
}

int main(int argc, char *argv[]) {
    const char *test_passwords[] = {
        "Weak1!",
        "NoSpecial123",
        "noupper123!",
        "NOLOWER123!",
        "NoDigit!!",
        "ValidPass1!",
        NULL
    };
    size_t i;

    (void)argc;
    (void)argv;

    for (i = 0; test_passwords[i] != NULL; i++) {
        if (is_valid_password(test_passwords[i])) {
            printf("VALID: %s\n", test_passwords[i]);
        } else {
            printf("INVALID: %s\n", test_passwords[i]);
        }
    }

    return 0;
}