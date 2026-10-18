#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *first;
    char *second;
} Tuple;

char** concatenate_adjacent(Tuple *tuples, size_t count, size_t *result_count) {
    if (count < 2 || tuples == NULL) {
        *result_count = 0;
        return NULL;
    }

    size_t new_count = count - 1;
    char **result = (char **)malloc(new_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    for (size_t i = 0; i < new_count; i++) {
        size_t len1 = strlen(tuples[i].second);
        size_t len2 = strlen(tuples[i + 1].first);
        
        result[i] = (char *)malloc(len1 + len2 + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        memcpy(result[i], tuples[i].second, len1);
        memcpy(result[i] + len1, tuples[i + 1].first, len2);
        result[i][len1 + len2] = '\0';
    }

    *result_count = new_count;
    return result;
}

int main() {
    Tuple tuples[] = {
        {"a", "b"},
        {"c", "d"},
        {"e", "f"}
    };
    size_t count = sizeof(tuples) / sizeof(tuples[0]);
    size_t result_count = 0;

    char **result = concatenate_adjacent(tuples, count, &result_count);

    if (result != NULL) {
        for (size_t i = 0; i < result_count; i++) {
            printf("%s\n", result[i]);
            free(result[i]);
        }
        free(result);
    }

    return 0;
}