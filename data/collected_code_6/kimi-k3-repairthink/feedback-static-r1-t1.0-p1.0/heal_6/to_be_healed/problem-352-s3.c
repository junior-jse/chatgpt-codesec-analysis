#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_DISCARD 65536
#define DISCARD_CHUNK 256

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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char chunk[DISCARD_CHUNK];
    size_t discarded = 0;

    while (discarded < MAX_DISCARD) {
        size_t remaining = MAX_DISCARD - discarded;
        size_t request = (remaining + 1 < sizeof(chunk)) ? remaining + 1 : sizeof(chunk);

        if (fgets(chunk, (int)request, stdin) == NULL) {
            return 0;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t got = strlen(chunk);
        discarded += got;

        if (got > 0 && chunk[got - 1] == '\n') {
            return 0;
        }
    }

    return -1;
}

int main(void)
{
    char *buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return 1;
    }

    printf("Enter a string: ");
    fflush(stdout);

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