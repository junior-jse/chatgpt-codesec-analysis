#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 128
#define INPUT_BUFFER_SIZE ((size_t)MAX_PASSWORD_LENGTH + 3)
#define MAX_DISCARD_COUNT ((size_t)4096)

bool is_valid_password(const char *password)
{
    size_t length;
    size_t i;
    bool has_upper;
    bool has_lower;
    bool has_digit;
    bool has_special;

    if (password == NULL) {
        return false;
    }

    length = strnlen(password, (size_t)MAX_PASSWORD_LENGTH + 1);
    if (length < (size_t)MIN_PASSWORD_LENGTH || length > (size_t)MAX_PASSWORD_LENGTH) {
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

static void discard_remaining_input(void)
{
    size_t discarded = 0;

    while (discarded < MAX_DISCARD_COUNT) {
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        int ch = getchar();

        if (ch == '\n' || ch == EOF) {
            break;
        }
        discarded++;
    }
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char password[INPUT_BUFFER_SIZE];
    size_t len;
    bool valid;

    if (fgets(password, (int)sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    len = strnlen(password, sizeof(password));

    if (len == sizeof(password) - 1 && password[len - 1] != '\n') {
        discard_remaining_input();
        memset(password, 0, sizeof(password));
        printf("Invalid password\n");
        return 0;
    }

    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }

    valid = is_valid_password(password);

    memset(password, 0, sizeof(password));

    if (valid) {
        printf("Valid password\n");
    } else {
        printf("Invalid password\n");
    }

    return 0;
}