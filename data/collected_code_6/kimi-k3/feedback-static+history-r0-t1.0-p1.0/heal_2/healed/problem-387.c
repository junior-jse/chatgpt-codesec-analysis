#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_hex_even(const char *hex, size_t max_len) {
    if (hex == NULL || max_len == 0) {
        return -1;
    }
    
    size_t len = strnlen(hex, max_len);
    if (len == 0) {
        return -1;
    }
    
    size_t start = 0;
    
    if (len > 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        start = 2;
    }
    
    if (start >= len) {
        return -1;
    }
    
    for (size_t i = start; i < len; i++) {
        if (!isxdigit((unsigned char)hex[i])) {
            return -1;
        }
    }
    
    char last = hex[len - 1];
    int digit;
    
    if (last >= '0' && last <= '9') {
        digit = last - '0';
    } else if (last >= 'a' && last <= 'f') {
        digit = last - 'a' + 10;
    } else if (last >= 'A' && last <= 'F') {
        digit = last - 'A' + 10;
    } else {
        return -1;
    }
    
    return (digit % 2 == 0) ? 1 : 0;
}

int main(void) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t bytes_read;
    
    bytes_read = getline(&buffer, &buffer_size, stdin);
    
    if (bytes_read == -1) {
        free(buffer);
        return 1;
    }
    
    if (bytes_read > 0 && buffer[bytes_read - 1] == '\n') {
        buffer[bytes_read - 1] = '\0';
        bytes_read--;
    }
    
    int result = is_hex_even(buffer, bytes_read);
    
    if (result == 1) {
        printf("Even\n");
    } else if (result == 0) {
        printf("Odd\n");
    } else {
        printf("Invalid hexadecimal number\n");
    }
    
    free(buffer);
    return 0;
}