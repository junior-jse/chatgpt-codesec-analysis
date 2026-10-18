#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool is_password_valid(const char *password)
{
    const size_t minimum_length = 8U;
    const size_t maximum_length = 128U;
    bool has_lowercase = false;
    bool has_uppercase = false;
    bool has_digit = false;
    bool has_special = false;
    size_t length = 0U;

    if (password == NULL) {
        return false;
    }

    for (; password[length] != '\0'; ++length) {
        unsigned char character = (unsigned char)password[length];

        if (length >= maximum_length) {
            return false;
        }

        if (character >= (unsigned char)'a' && character <= (unsigned char)'z') {
            has_lowercase = true;
        } else if (character >= (unsigned char)'A' && character <= (unsigned char)'Z') {
            has_uppercase = true;
        } else if (character >= (unsigned char)'0' && character <= (unsigned char)'9') {
            has_digit = true;
        } else if (strchr("!@#$%^&*()-_=+[]{};:,.?/", (int)character) != NULL) {
            has_special = true;
        } else {
            return false;
        }
    }

    return length >= minimum_length &&
           has_lowercase &&
           has_uppercase &&
           has_digit &&
           has_special;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char password[130];

    if (fgets(password, sizeof password, stdin) == NULL) {
        return 1;
    }

    size_t length = strcspn(password, "\r\n");

    if (password[length] == '\0') {
        if (length == sizeof password - 1U) {
            return 1;
        }
    } else {
        password[length] = '\0';
    }

    puts(is_password_valid(password) ? "true" : "false");
    return 0;
}