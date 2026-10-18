#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** concatenate_adjacent_tuples(char** tuples, int* sizes, int count, int* result_count) {
    *result_count = count - 1;
    if (count < 2) {
        return NULL;
    }

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

        memcpy(result[i], tuples[i], len1);
        memcpy(result[i] + len1, tuples[i + 1], len2);
        result[i][total_len] = '\0';
    }

    return result;
}

int main() {
    char* tuples[] = {"abc", "def", "gh", "ijk"};
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