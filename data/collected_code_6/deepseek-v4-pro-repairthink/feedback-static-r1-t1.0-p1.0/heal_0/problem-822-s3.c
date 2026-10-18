#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

bool is_valid_password(const char *password) {
    if (password == NULL) {
        return false;
    }

    size_t length = strlen(password);
    if (length < 8 || length > 64) {
        return false;
    }

    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;

    for (size_t i = 0; i < length; i++) {
        unsigned char c = password[i];
        if (isupper(c)) {
            has_upper = true;
        } else if (islower(c)) {
            has_lower = true;
        } else if (isdigit(c)) {
            has_digit = true;
        } else if (ispunct(c)) {
            has_special = true;
        } else if (!isprint(c)) {
            return false;
        }
    }

    return has_upper && has_lower && has_digit && has_special;
}

int main(void) {
    const char *test_passwords[] = {
        "StrongPass1!",
        "weak",
        "NoSpecial123",
        "NoDigit!ABC",
        "NoUpper!123",
        "NoLower!123",
        "ValidP@ssw0rd",
        NULL,
        ""
    };

    for (int i = 0; i < 9; i++) {
        const char *pwd = test_passwords[i];
        printf("Password: '%s' -> %s\n", 
               pwd ? pwd : "(null)", 
               is_valid_password(pwd) ? "valid" : "invalid");
    }

    return 0;
}