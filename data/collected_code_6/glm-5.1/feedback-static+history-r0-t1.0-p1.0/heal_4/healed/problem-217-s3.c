#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char find_first_repeated(const char *str, size_t max_len) {
    unsigned char *seen = calloc(256, sizeof(unsigned char));
    if (seen == NULL) {
        return '\0';
    }

    if (str == NULL || max_len == 0) {
        free(seen);
        return '\0';
    }

    for (size_t i = 0; i < max_len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c == '\0') {
            break;
        }
        if (seen[c] == 1) {
            free(seen);
            return (char)c;
        }
        seen[c] = 1;
    }

    free(seen);
    return '\0';
}

int main(int argc, char * const argv[]) {
    char result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t len = strnlen(argv[1], 4096);
    if (len == 4096) {
        fprintf(stderr, "Input string exceeds maximum allowed length\n");
        return EXIT_FAILURE;
    }

    result = find_first_repeated(argv[1], len);

    if (result != '\0') {
        printf("%c\n", result);
    } else {
        printf("No repeated character found\n");
    }

    return EXIT_SUCCESS;
}