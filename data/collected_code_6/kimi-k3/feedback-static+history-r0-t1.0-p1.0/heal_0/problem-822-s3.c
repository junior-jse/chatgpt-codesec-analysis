#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MIN_LENGTH 8
#define MAX_LENGTH 128

bool isValidPassword(const char *password) {
    if (password == NULL) {
        return false;
    }
    
    size_t length = strlen(password);
    
    if (length < MIN_LENGTH || length > MAX_LENGTH) {
        return false;
    }
    
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    
    for (size_t i = 0; i < length; i++) {
        unsigned char c = (unsigned char)password[i];
        
        if (isupper(c)) {
            hasUpper = true;
        } else if (islower(c)) {
            hasLower = true;
        } else if (isdigit(c)) {
            hasDigit = true;
        } else if (ispunct(c)) {
            hasSpecial = true;
        } else if (isspace(c)) {
            return false;
        }
    }
    
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

int main(void) {
    char password[MAX_LENGTH + 2];
    
    printf("Enter password: ");
    
    if (fgets(password, sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        fprintf(stderr, "Password too long\n");
        return EXIT_FAILURE;
    }
    
    if (isValidPassword(password)) {
        printf("Password is valid\n");
        return EXIT_SUCCESS;
    } else {
        printf("Password is invalid\n");
        return EXIT_FAILURE;
    }
}