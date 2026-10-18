#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_DISCARD 65536

bool all_unique(const char *str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return false;
    }

    bool seen[UCHAR_MAX + 1] = {false};

    for (size_t i = 0; i < max_len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c == '\0') {
            break;
        }
        if (seen[c]) {
            return false;
        }
        seen[c] = true;
    }

    return true;
}

int main(void) {
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return 1;
    }

    printf("Enter a string: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(buffer);
        return 1;
    }

    buffer[BUFFER_SIZE - 1] = '\0';

    const char *end = memchr(buffer, '\0', BUFFER_SIZE);
    size_t len = (end != NULL) ? (size_t)(end - buffer) : BUFFER_SIZE;

    const char *newline = memchr(buffer, '\n', len);
    if (newline != NULL) {
        buffer[newline - buffer] = '\0';
    } else if (len == BUFFER_SIZE - 1) {
        int ch;
        size_t discarded = 0;
        while (discarded < MAX_DISCARD && (ch = getchar()) != '\n' && ch != EOF) {
            discarded++;
        }
    }

    if (all_unique(buffer, BUFFER_SIZE)) {
        printf("All characters are unique.\n");
    } else {
        printf("Duplicate characters found.\n");
    }

    free(buffer);
    return 0;
}