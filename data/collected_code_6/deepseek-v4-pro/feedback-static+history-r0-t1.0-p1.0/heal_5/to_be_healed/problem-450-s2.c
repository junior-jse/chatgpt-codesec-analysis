#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(char **input_list, int list_size, int *extract_sizes, int *result_count) {
    if (input_list == NULL || extract_sizes == NULL || result_count == NULL || list_size <= 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    *result_count = 0;

    char **result = (char **)malloc((size_t)list_size * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < list_size; i++) {
        if (input_list[i] == NULL || extract_sizes[i] < 0) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        size_t src_len = strnlen(input_list[i], (size_t)extract_sizes[i]);
        size_t copy_len = (extract_sizes[i] < 0) ? 0 : (size_t)extract_sizes[i];
        if (copy_len > src_len) {
            copy_len = src_len;
        }

        result[count] = (char *)malloc(copy_len + 1);
        if (result[count] == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        if (copy_len > 0) {
            if (copy_len <= src_len) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[count], input_list[i], copy_len);
            }
        }
        result[count][copy_len] = '\0';
        count++;
    }

    *result_count = count;
    return result;
}

int main(void) {
    char *input[] = {"HelloWorld", "Test", "CProgramming", "OpenAI"};
    int sizes[] = {5, 2, 4, 10};
    int count = 0;

    char **result = extract_strings(input, 4, sizes, &count);
    if (result == NULL) {
        printf("Extraction failed\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}