#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} Tuple;

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

char *flatten_tuples(const Tuple *tuples, size_t count, const char *separator) {
    if (tuples == NULL || separator == NULL) {
        return NULL;
    }

    size_t sep_len = safe_strlen(separator, 1024);
    size_t total_len = 0;

    for (size_t i = 0; i < count; i++) {
        if (tuples[i].key == NULL || tuples[i].value == NULL) {
            return NULL;
        }
        size_t key_len = safe_strlen(tuples[i].key, 1024);
        size_t value_len = safe_strlen(tuples[i].value, 1024);
        total_len += key_len + value_len;
        if (i < count - 1) {
            total_len += sep_len;
        }
    }

    char *result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        size_t key_len = safe_strlen(tuples[i].key, 1024);
        if (offset + key_len > total_len) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuples[i].key, key_len);
        offset += key_len;

        size_t value_len = safe_strlen(tuples[i].value, 1024);
        if (offset + value_len > total_len) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuples[i].value, value_len);
        offset += value_len;

        if (i < count - 1) {
            if (offset + sep_len > total_len) {
                free(result);
                return NULL;
            }
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