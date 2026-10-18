#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

bool all_unique(const char *str, size_t len) {
    if (str == NULL) {
        return false;
    }
    
    bool seen[UCHAR_MAX + 1] = {false};
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (seen[c]) {
            return false;
        }
        seen[c] = true;
    }
    
    return true;
}

int main(void) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t bytes_read;
    
    printf("Enter a string: ");
    
    bytes_read = getline(&buffer, &buffer_size, stdin);
    
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading input\n");
        free(buffer);
        return 1;
    }
    
    size_t len = (size_t)bytes_read;
    
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (all_unique(buffer, len)) {
        printf("All characters are unique.\n");
    } else {
        printf("Duplicate characters found.\n");
    }
    
    free(buffer);
    return 0;
}