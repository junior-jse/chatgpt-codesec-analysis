#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>

char* remove_words_with_length(const char* input, int k) {
    if (input == NULL || k <= 0) {
        return NULL;
    }

    size_t out_len = 0;
    int first_word = 1;
    const char* p = input;

    while (*p) {
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }

        const char* word_start = p;
        while (*p && !isspace((unsigned char)*p)) {
            p++;
        }
        size_t word_len = p - word_start;

        if (word_len != (size_t)k) {
            if (!first_word) {
                out_len++;
            }
            out_len += word_len;
            first_word = 0;
        }
    }

    char* result = malloc(out_len + 1);
    if (result == NULL) {
        return NULL;
    }

    p = input;
    size_t pos = 0;
    first_word = 1;

    while (*p) {
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }

        const char* word_start = p;
        while (*p && !isspace((unsigned char)*p)) {
            p++;
        }
        size_t word_len = p - word_start;

        if (word_len != (size_t)k) {
            if (!first_word) {
                result[pos++] = ' ';
            }
            for (size_t i = 0; i < word_len; i++) {
                result[pos++] = word_start[i];
            }
            first_word = 0;
        }
    }
    result[pos] = '\0';

    return result;
}

char* read_line(void) {
    size_t capacity = 32;
    size_t length = 0;
    char* buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != '\n' && c != EOF) {
        if (length >= capacity - 1) {
            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }
            capacity *= 2;
            char* new_buffer = realloc(buffer, capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[length++] = (char)c;
    }

    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void) {
    printf("Enter a string: ");
    char* input = read_line();
    if (input == NULL) {
        return 1;
    }

    int k;
    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        free(input);
        return 1;
    }

    char* result = remove_words_with_length(input, k);
    free(input);

    if (result == NULL) {
        printf("Error processing string\n");
        return 1;
    }

    printf("Result: %s\n", result);
    free(result);
    return 0;
}