#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool is_valid_password(const char *password, size_t max_len) {
    if (password == NULL) {
        return false;
    }

    size_t len = strnlen(password, max_len + 1);
    if (len < 8 || len > max_len) {
        return false;
    }

    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;

    for (size_t i = 0; i < len; ++i) {
        unsigned char ch = (unsigned char)password[i];
        if (isupper(ch)) {
            has_upper = true;
        } else if (islower(ch)) {
            has_lower = true;
        } else if (isdigit(ch)) {
            has_digit = true;
        } else if (ispunct(ch)) {
            has_special = true;
        }
    }

    if (!has_upper || !has_lower || !has_digit || !has_special) {
        return false;
    }

    return true;
}

int main(void) {
    const char *test_passwords[] = {
        "Short1!",
        "nouppercase1!",
        "NOLOWERCASE1!",
        "NoDigits!!",
        "NoSpecial1",
        "ValidPass1!",
        NULL
    };

    for (size_t i = 0; test_passwords[i] != NULL; ++i) {
        bool result = is_valid_password(test_passwords[i], 64);
        printf("Password: %-15s Valid: %s\n", test_passwords[i], result ? "true" : "false");
    }

    const char *null_password = NULL;
    bool null_result = is_valid_password(null_password, 64);
    printf("Password: NULL             Valid: %s\n", null_result ? "true" : "false");

    return 0;
}