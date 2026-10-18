#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 64

bool is_valid_password(const char *password) {
    if (password == NULL) {
        return false;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     *  Return index of string to the first element that is 0
     */
    size_t length = strlen(password);
    
    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
        return false;
    }
    
    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;
    
    /* Possible weaknesses found:
     *  Assuming that condition 'i<length' is not redundant
     */
    for (size_t i = 0; i < length; i++) {
        /* Possible weaknesses found:
         *  Assignment 'c=(unsigned char)password[i]', assigned value is greater than 0
         */
        unsigned char c = (unsigned char)password[i];
        
        if (isupper(c)) {
            has_upper = true;
        } else if (islower(c)) {
            has_lower = true;
        } else if (isdigit(c)) {
            has_digit = true;
        } else if (ispunct(c)) {
            has_special = true;
        /* Possible weaknesses found:
         *  Condition 'c=='\0'' is always false
         *  Condition 'c=='\0'' is always false [knownConditionTrueFalse]
         */
        } else if (isspace(c) || c == '\0') {
            return false;
        }
    }
    
    return has_upper && has_lower && has_digit && has_special;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char password[MAX_PASSWORD_LENGTH + 1];
    
    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }
    
    if (is_valid_password(password)) {
        printf("Password is valid\n");
        return 0;
    } else {
        printf("Password is invalid\n");
        return 1;
    }
}