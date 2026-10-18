#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 128

bool is_valid_password(const char *password, size_t length)
{
    size_t i;
    bool has_upper;
    bool has_lower;
    bool has_digit;
    bool has_special;

    if (password == NULL || length == 0) {
        return false;
    }

    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
        return false;
    }

    has_upper = false;
    has_lower = false;
    has_digit = false;
    has_special = false;

    for (i = 0; i < length; i++) {
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

int main(void)
{
    char password[MAX_PASSWORD_LENGTH + 2];
    size_t len;

    if (fgets(password, sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    len = strnlen(password, sizeof(password));
    if (len == sizeof(password)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
        len--;
    }

    if (is_valid_password(password, len)) {
        printf("Valid password\n");
    } else {
        printf("Invalid password\n");
    }

    return 0;
}