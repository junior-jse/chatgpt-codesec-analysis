#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 1024

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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[BUFFER_SIZE];

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    buffer[sizeof(buffer) - 1] = '\0';

    const char *end = memchr(buffer, '\0', sizeof(buffer));
    size_t len = (end != NULL) ? (size_t)(end - buffer) : sizeof(buffer);

    const char *newline = memchr(buffer, '\n', len);
    if (newline != NULL) {
        buffer[newline - buffer] = '\0';
    } else if (len == sizeof(buffer) - 1) {
        int ch;
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }

    if (all_unique(buffer, sizeof(buffer))) {
        printf("All characters are unique.\n");
    } else {
        printf("Duplicate characters found.\n");
    }

    return 0;
}