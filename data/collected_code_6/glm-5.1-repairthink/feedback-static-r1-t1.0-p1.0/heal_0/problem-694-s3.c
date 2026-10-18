#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **values;
    size_t count;
} UniqueResult;

UniqueResult extract_unique_values(char **dictionary_values, size_t count) {
    UniqueResult result;
    result.values = NULL;
    result.count = 0;

    if (dictionary_values == NULL || count == 0) {
        return result;
    }

    result.values = (char **)malloc(count * sizeof(char *));
    if (result.values == NULL) {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < count; i++) {
        if (dictionary_values[i] == NULL) {
            continue;
        }

        int is_unique = 1;
        for (size_t j = 0; j < result.count; j++) {
            if (strcmp(dictionary_values[i], result.values[j]) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            result.values[result.count] = dictionary_values[i];
            result.count++;
        }
    }

    char **reallocated_values = (char **)realloc(result.values, result.count * sizeof(char *));
    if (reallocated_values != NULL) {
        result.values = reallocated_values;
    }

    return result;
}

int main() {
    char *dict_values[] = {"apple", "banana", "apple", "orange", "banana", "grape"};
    size_t count = sizeof(dict_values) / sizeof(dict_values[0]);

    UniqueResult unique = extract_unique_values(dict_values, count);

    for (size_t i = 0; i < unique.count; i++) {
        printf("%s\n", unique.values[i]);
    }

    free(unique.values);

    return 0;
}