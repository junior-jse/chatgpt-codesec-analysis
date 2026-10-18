#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_VALUE_LENGTH 1024

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct {
    KeyValuePair *pairs;
    size_t count;
} Dictionary;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

char **get_unique_values(const Dictionary *dict, size_t *unique_count) {
    if (dict == NULL || dict->pairs == NULL || unique_count == NULL) {
        return NULL;
    }

    *unique_count = 0;

    if (dict->count == 0) {
        return NULL;
    }

    if (dict->count > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    char **unique_values = malloc(dict->count * sizeof(char *));
    if (unique_values == NULL) {
        return NULL;
    }

    size_t found = 0;

    for (size_t i = 0; i < dict->count; i++) {
        const char *value = dict->pairs[i].value;
        if (value == NULL) {
            continue;
        }

        size_t value_length = bounded_strlen(value, MAX_VALUE_LENGTH + 1);
        if (value_length > MAX_VALUE_LENGTH) {
            for (size_t k = 0; k < found; k++) {
                free(unique_values[k]);
            }
            free(unique_values);
            return NULL;
        }

        int is_duplicate = 0;

        for (size_t j = 0; j < found; j++) {
            if (strncmp(unique_values[j], value, value_length + 1) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            unique_values[found] = malloc(value_length + 1);
            if (unique_values[found] == NULL) {
                for (size_t k = 0; k < found; k++) {
                    free(unique_values[k]);
                }
                free(unique_values);
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(unique_values[found], value, value_length);
            unique_values[found][value_length] = '\0';
            found++;
        }
    }

    *unique_count = found;
    return unique_values;
}

void free_unique_values(char **values, size_t count) {
    if (values == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        free(values[i]);
    }
    free(values);
}

int main(void) {
    KeyValuePair pairs[] = {
        {"a", "apple"},
        {"b", "banana"},
        {"c", "apple"},
        {"d", "cherry"},
        {"e", "banana"},
        {"f", "date"}
    };

    Dictionary dict = {pairs, 6};
    size_t unique_count = 0;

    printf("Dictionary entries:\n");
    for (size_t i = 0; i < dict.count; i++) {
        printf("%s -> %s\n", dict.pairs[i].key, dict.pairs[i].value);
    }

    char **unique = get_unique_values(&dict, &unique_count);

    if (unique == NULL && unique_count == 0) {
        fprintf(stderr, "Error: Failed to extract unique values\n");
        return 1;
    }

    printf("Unique values (%zu):\n", unique_count);
    for (size_t i = 0; i < unique_count; i++) {
        printf("%s\n", unique[i]);
    }

    free_unique_values(unique, unique_count);

    return 0;
}