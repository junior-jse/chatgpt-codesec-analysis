#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** adjacent_concatenation(char** tuples, int tuple_count, int tuple_size) {
    if (tuples == NULL || tuple_count <= 0 || tuple_size <= 1) {
        return NULL;
    }

    char** result = (char**)malloc((tuple_count * (tuple_size - 1)) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int result_index = 0;
    for (int i = 0; i < tuple_count; i++) {
        if (tuples[i] == NULL) {
            for (int j = 0; j < result_index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        for (int j = 0; j < tuple_size - 1; j++) {
            char* str1 = tuples[i * tuple_size + j];
            char* str2 = tuples[i * tuple_size + j + 1];

            if (str1 == NULL || str2 == NULL) {
                for (int k = 0; k < result_index; k++) {
                    free(result[k]);
                }
                free(result);
                return NULL;
            }

            size_t len1 = strnlen(str1, 4096);
            size_t len2 = strnlen(str2, 4096);
            result[result_index] = (char*)malloc((len1 + len2 + 1) * sizeof(char));
            if (result[result_index] == NULL) {
                for (int k = 0; k < result_index; k++) {
                    free(result[k]);
                }
                free(result);
                return NULL;
            }

            snprintf(result[result_index], len1 + len2 + 1, "%s%s", str1, str2);
            result_index++;
        }
    }

    return result;
}

int main(void) {
    char* tuples[][3] = {
        {"ab", "cd", "ef"},
        {"gh", "ij", "kl"},
        {"mn", "op", "qr"}
    };

    int tuple_count = 3;
    int tuple_size = 3;

    char** flattened = (char**)malloc(tuple_count * tuple_size * sizeof(char*));
    if (flattened == NULL) {
        return 1;
    }

    for (int i = 0; i < tuple_count; i++) {
        for (int j = 0; j < tuple_size; j++) {
            flattened[i * tuple_size + j] = tuples[i][j];
        }
    }

    char** result = adjacent_concatenation(flattened, tuple_count, tuple_size);
    if (result == NULL) {
        free(flattened);
        return 1;
    }

    int result_count = tuple_count * (tuple_size - 1);
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
        free(result[i]);
    }

    free(result);
    free(flattened);

    return 0;
}