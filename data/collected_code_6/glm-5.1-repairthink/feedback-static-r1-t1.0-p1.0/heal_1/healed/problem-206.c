#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** concatenate_adjacent_tuples(const char** tuples, const int* sizes, int count, int* result_count) {
    if (count < 2) {
        *result_count = 0;
        return NULL;
    }

    *result_count = count - 1;
    char** result = (char**)malloc(*result_count * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < *result_count; i++) {
        int len1 = sizes[i];
        int len2 = sizes[i + 1];
        int total_len = len1 + len2;

        result[i] = (char*)malloc((total_len + 1) * sizeof(char));
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        snprintf(result[i], total_len + 1, "%.*s%.*s", len1, tuples[i], len2, tuples[i + 1]);
    }

    return result;
}

int main() {
    const char* tuples[] = {"abc", "def", "gh", "ijk"};
    int sizes[] = {3, 3, 2, 3};
    int count = sizeof(sizes) / sizeof(sizes[0]);
    int result_count = 0;

    char** result = concatenate_adjacent_tuples(tuples, sizes, count, &result_count);

    if (result != NULL) {
        for (int i = 0; i < result_count; i++) {
            printf("%s\n", result[i]);
            free(result[i]);
        }
        free(result);
    }

    return 0;
}