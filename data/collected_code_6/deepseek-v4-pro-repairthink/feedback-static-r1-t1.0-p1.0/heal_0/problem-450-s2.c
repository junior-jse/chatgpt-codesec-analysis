#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(char **input_list, int list_size, int *extract_sizes, int *result_count) {
    if (input_list == NULL || extract_sizes == NULL || result_count == NULL || list_size <= 0) {
        *result_count = 0;
        return NULL;
    }

    char **result = (char **)malloc(list_size * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < list_size; i++) {
        if (input_list[i] == NULL || extract_sizes[i] < 0) {
            free(result);
            *result_count = 0;
            return NULL;
        }

        int src_len = (int)strlen(input_list[i]);
        int copy_len = extract_sizes[i] < src_len ? extract_sizes[i] : src_len;

        result[count] = (char *)malloc((copy_len + 1) * sizeof(char));
        if (result[count] == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        strncpy(result[count], input_list[i], copy_len);
        result[count][copy_len] = '\0';
        count++;
    }

    *result_count = count;
    return result;
}

int main() {
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