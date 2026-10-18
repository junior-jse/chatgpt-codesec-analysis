#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE 128

char find_max_char(const char *str, size_t len) {
    int count[ASCII_SIZE] = {0};
    int max_count = 0;
    char max_char = '\0';

    if (str == NULL || len == 0) {
        return '\0';
    }

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c >= ASCII_SIZE) {
            continue;
        }
        count[c]++;
        if (count[c] > max_count) {
            max_count = count[c];
            max_char = c;
        }
    }

    return max_char;
}

int main(int argc, char * const * argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t len = strnlen(argv[1], 1024);
    char result = find_max_char(argv[1], len);
    
    if (result != '\0') {
        printf("%c\n", result);
    } else {
        printf("No valid characters found or empty string provided.\n");
    }

    return EXIT_SUCCESS;
}