#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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

static bool has_special(const char *password) {
    while (*password) {
        if (!isalnum((unsigned char)*password)) {
            return true;
        }
        password++;
    }
    return false;
}

static bool is_password_valid(const char *password) {
    size_t len;
    
    if (password == NULL) {
        return false;
    }
    
    len = strlen(password);
    
    if (len < 8 || len > 32) {
        return false;
    }
    
    if (!has_uppercase(password)) {
        return false;
    }
    
    if (!has_lowercase(password)) {
        return false;
    }
    
    if (!has_digit(password)) {
        return false;
    }
    
    if (!has_special(password)) {
        return false;
    }
    
    return true;
}

int main(void) {
    const char *test_passwords[] = {
        "Password1!",
        "weak",
        "NoDigits!",
        "nouppercase1!",
        "NOLOWERCASE1!",
        "NoSpecial1",
        NULL
    };
    
    for (int i = 0; test_passwords[i] != NULL; i++) {
        printf("'%s': %s\n", test_passwords[i],
               is_password_valid(test_passwords[i]) ? "valid" : "invalid");
    }
    
    return 0;
}