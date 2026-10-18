#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool is_valid_password(const char *password) {
    if (password == NULL) {
        return false;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(password);
    if (len < 8 || len > 64) {
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
        bool result = is_valid_password(test_passwords[i]);
        printf("Password: %-15s Valid: %s\n", test_passwords[i], result ? "true" : "false");
    }

    /* Possible weaknesses found:
     *  Assignment 'null_result=is_valid_password(NULL)', assigned value is 0
     *  Calling function 'is_valid_password' returns 0
     */
    bool null_result = is_valid_password(NULL);
    /* Possible weaknesses found:
     *  Condition 'null_result' is always false
     *  Condition 'null_result' is always false [knownConditionTrueFalse]
     */
    printf("Password: NULL             Valid: %s\n", null_result ? "true" : "false");

    return 0;
}