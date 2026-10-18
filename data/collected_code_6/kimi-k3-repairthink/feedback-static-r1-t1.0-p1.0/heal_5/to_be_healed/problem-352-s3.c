#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_DISCARD 65536

bool all_unique(const char *str, size_t max_len)
{
    if (str == NULL || max_len == 0) {
        return false;
    }

    bool seen[UCHAR_MAX + 1] = { false };

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

static int discard_excess_input(void)
{
    size_t discarded = 0;

    while (discarded < MAX_DISCARD) {
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        int ch = getchar();
        if (ch == '\n' || ch == EOF) {
            return 0;
        }
        discarded++;
    }

    return -1;
}

int main(void)
{
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
        if (discard_excess_input() != 0) {
            fprintf(stderr, "Warning: input too long, truncated\n");
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