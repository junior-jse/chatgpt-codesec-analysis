#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(const char **source, size_t count, size_t extract_size, size_t *result_count)
{
    char **result;
    size_t i;
    size_t valid_count = 0;

    if (source == NULL || result_count == NULL || extract_size == 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    for (i = 0; i < count; i++) {
        if (source[i] != NULL && strlen(source[i]) == extract_size) {
            valid_count++;
        }
    }

    if (valid_count == 0) {
        *result_count = 0;
        return NULL;
    }

    result = malloc(valid_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    valid_count = 0;
    for (i = 0; i < count; i++) {
        if (source[i] != NULL && strlen(source[i]) == extract_size) {
            result[valid_count] = malloc((extract_size + 1) * sizeof(char));
            if (result[valid_count] == NULL) {
                size_t j;
                for (j = 0; j < valid_count; j++) {
                    free(result[j]);
                }
                free(result);
                *result_count = 0;
                return NULL;
            }
            strcpy(result[valid_count], source[i]);
            valid_count++;
        }
    }

    *result_count = valid_count;
    return result;
}

void free_extracted(char **strings, size_t count)
{
    size_t i;
    if (strings == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
}

int main(void)
{
    const char *list[] = {
        "apple",
        "banana",
        "cherry",
        "date",
        "elderberry",
        "fig",
        "grape",
        NULL
    };
    size_t count = 7;
    size_t extract_size = 5;
    size_t result_count = 0;
    char **extracted;
    size_t i;

    extracted = extract_strings(list, count, extract_size, &result_count);

    if (extracted != NULL) {
        printf("Extracted strings of size %zu:\n", extract_size);
        for (i = 0; i < result_count; i++) {
            printf("%s\n", extracted[i]);
        }
        free_extracted(extracted, result_count);
    } else {
        printf("No strings found with size %zu or an error occurred.\n", extract_size);
    }

    return 0;
}