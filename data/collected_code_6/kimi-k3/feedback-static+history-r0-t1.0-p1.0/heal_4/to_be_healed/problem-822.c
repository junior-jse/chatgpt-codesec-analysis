#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 64

bool is_valid_password(const char *password, size_t max_len) {
    if (password == NULL) {
        return false;
    }
    
    size_t length = strnlen(password, max_len);
    
    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
        return false;
    }
    
    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_special = false;
    
    for (size_t i = 0; i < length; i++) {
        unsigned char c = (unsigned char)password[i];
        
        if (c == '\0') {
            break;
        }
        
        if (isupper(c)) {
            has_upper = true;
        } else if (islower(c)) {
            has_lower = true;
        } else if (isdigit(c)) {
            has_digit = true;
        } else if (ispunct(c)) {
            has_special = true;
        } else if (isspace(c)) {
            return false;
        }
    }
    
    return has_upper && has_lower && has_digit && has_special;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char password[MAX_PASSWORD_LENGTH + 2];
    
    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(password, sizeof(password));
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    } else if (len == sizeof(password) - 1 && password[len - 1] != '\n') {
        int c;
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c == EOF) {
                break;
            }
        }
        printf("Password is invalid\n");
        return 1;
    }
    
    if (is_valid_password(password, sizeof(password))) {
        printf("Password is valid\n");
        return 0;
    } else {
        printf("Password is invalid\n");
        return 1;
    }
}