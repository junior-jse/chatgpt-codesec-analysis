#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LEN 4096

typedef struct {
    char *key;
    char *value;
} Tuple;

char *flatten_tuples(const Tuple *tuples, size_t count, const char *separator) {
    if (tuples == NULL || separator == NULL) {
        return NULL;
    }

    size_t sep_len = strnlen(separator, MAX_STRING_LEN);
    if (sep_len >= MAX_STRING_LEN) {
        return NULL;
    }

    size_t total_len = 0;

    for (size_t i = 0; i < count; i++) {
        if (tuples[i].key == NULL || tuples[i].value == NULL) {
            return NULL;
        }

        size_t key_len = strnlen(tuples[i].key, MAX_STRING_LEN);
        size_t value_len = strnlen(tuples[i].value, MAX_STRING_LEN);
        if (key_len >= MAX_STRING_LEN || value_len >= MAX_STRING_LEN) {
            return NULL;
        }

        if (total_len > SIZE_MAX - key_len - value_len) {
            return NULL;
        }
        total_len += key_len + value_len;

        if (i < count - 1) {
            if (total_len > SIZE_MAX - sep_len) {
                return NULL;
            }
            total_len += sep_len;
        }
    }

    if (total_len >= SIZE_MAX) {
        return NULL;
    }

    char *result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;

    for (size_t i = 0; i < count; i++) {
        size_t key_len = strnlen(tuples[i].key, MAX_STRING_LEN);
        size_t value_len = strnlen(tuples[i].value, MAX_STRING_LEN);

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + offset, tuples[i].key, key_len);
        offset += key_len;
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + offset, tuples[i].value, value_len);
        offset += value_len;

        if (i < count - 1) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + offset, separator, sep_len);
            offset += sep_len;
        }
    }

    result[offset] = '\0';

    return result;
}

int main(void) {
    Tuple tuples[] = {
        {"name", "John"},
        {"age", "30"},
        {"city", "NYC"}
    };
    size_t count = sizeof(tuples) / sizeof(tuples[0]);

    char *flattened = flatten_tuples(tuples, count, ", ");
    if (flattened == NULL) {
        fprintf(stderr, "Error: Failed to flatten tuples\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", flattened);

    free(flattened);
    return EXIT_SUCCESS;
}