#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 128

static bool has_uppercase(const char *password) {
    while (*password) {
        if (isupper((unsigned char)*password)) {
            return true;
        }
        password++;
    }
    return false;
}

static bool has_lowercase(const char *password) {
    while (*password) {
        if (islower((unsigned char)*password)) {
            return true;
        }
        password++;
    }
    return false;
}

static bool has_digit(const char *password) {
    while (*password) {
        if (isdigit((unsigned char)*password)) {
            return true;
        }
        password++;
    }
    return false;
}

static bool has_special_character(const char *password) {
    while (*password) {
        char c = *password;
        if (!isalnum((unsigned char)c)) {
            return true;
        }
        password++;
    }
    return false;
}

bool is_valid_password(const char *password) {
    if (password == NULL) {
        return false;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t length = strlen(password);
    
    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
        return false;
    }

    if (!has_uppercase(password) ||
        !has_lowercase(password) ||
        !has_digit(password) ||
        !has_special_character(password)) {
        return false;
    }

    for (size_t i = 0; i < length; i++) {
        if (!isprint((unsigned char)password[i])) {
            return false;
        }
    }

    return true;
}

int main(void) {
    const char *test_passwords[] = {
        "Password1!",
        "password1!",
        "PASSWORD1!",
        "Password!",
        "Pass1!",
        "Password1",
        "Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!Password1!",
        "Valid123!@#",
        "",
        NULL
    };

    for (int i = 0; test_passwords[i] != NULL; i++) {
        printf("Password: \"%s\" - Valid: %s\n", 
               test_passwords[i],
               is_valid_password(test_passwords[i]) ? "true" : "false");
    }

    printf("NULL password - Valid: %s\n", 
           /* Possible weaknesses found:
            *  Condition 'is_valid_password(NULL)' is always false [knownConditionTrueFalse]
            *  Condition 'is_valid_password(NULL)' is always false
            *  Calling function 'is_valid_password' returns 0
            */
           is_valid_password(NULL) ? "true" : "false");

    return 0;
}