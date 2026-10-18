#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1025

char* minimizeString(const char* str, size_t maxLen) {
    if (str == NULL || maxLen == 0) {
        return NULL;
    }

    size_t len = strnlen(str, maxLen);
    if (len == 0) {
        char* result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    size_t maxFreq = 0;
    unsigned char maxChar = 0;
    for (size_t i = 0; i < 256; i++) {
        if (freq[i] > maxFreq) {
            maxFreq = freq[i];
            maxChar = (unsigned char)i;
        }
    }

    size_t newLen = len - maxFreq;
    char* result = malloc(newLen + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if ((unsigned char)str[i] != maxChar) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[INPUT_BUFFER_SIZE];

    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[sizeof(input) - 1] = '\0';

        size_t len = strnlen(input, sizeof(input));
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        } else if (len == sizeof(input) - 1) {
            int ch;
            /* Possible weaknesses found:
             * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
             */
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }

        char* result = minimizeString(input, sizeof(input));
        if (result != NULL) {
            printf("%s\n", result);
            free(result);
        }
    }

    return 0;
}