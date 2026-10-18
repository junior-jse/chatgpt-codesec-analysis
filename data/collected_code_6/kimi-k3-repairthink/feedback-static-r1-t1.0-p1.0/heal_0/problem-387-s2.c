#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool is_hex_even(const char *hex_str) {
    if (hex_str == NULL || *hex_str == '\0') {
        return false;
    }
    
    size_t len = strlen(hex_str);
    size_t start = 0;
    
    if (len >= 2 && hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        start = 2;
    }
    
    if (start >= len) {
        return false;
    }
    
    char last_char = '\0';
    for (size_t i = start; i < len; i++) {
        if (!isxdigit((unsigned char)hex_str[i])) {
            return false;
        }
        last_char = hex_str[i];
    }
    
    int digit;
    if (last_char >= '0' && last_char <= '9') {
        digit = last_char - '0';
    } else if (last_char >= 'a' && last_char <= 'f') {
        digit = last_char - 'a' + 10;
    } else if (last_char >= 'A' && last_char <= 'F') {
        digit = last_char - 'A' + 10;
    } else {
        return false;
    }
    
    return (digit % 2) == 0;
}

int main(void) {
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (is_hex_even(buffer)) {
            printf("Even\n");
        } else {
            printf("Odd\n");
        }
    }
    
    return 0;
}