#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **split_uppercase(const char *input, int *count) {
    if (!input || !count) return NULL;
    *count = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);

    int capacity = 16;
    char **result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;

    size_t last_end = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] >= 'A' && input[i] <= 'Z') {
            if (i > 0) {
                if (*count >= capacity) {
                    capacity *= 2;
                    char **new_result = realloc(result, capacity * sizeof(char *));
                    if (!new_result) {
                        for (int j = 0; j < *count; j++) free(result[j]);
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                size_t segment_len = i - last_end;
                result[*count] = malloc(segment_len + 1);
                if (!result[*count]) {
                    for (int j = 0; j < *count; j++) free(result[j]);
                    free(result);
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[*count], input + last_end, segment_len);
                result[*count][segment_len] = '\0';
                (*count)++;
                last_end = i;
            }
        }
    }

    if (last_end < len) {
        if (*count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                for (int j = 0; j < *count; j++) free(result[j]);
                free(result);
                return NULL;
            }
            result = new_result;
        }
        size_t segment_len = len - last_end;
        result[*count] = malloc(segment_len + 1);
        if (!result[*count]) {
            for (int j = 0; j < *count; j++) free(result[j]);
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[*count], input + last_end, segment_len);
        result[*count][segment_len] = '\0';
        (*count)++;
    }

    return result;
}

void free_split_result(char **result, int count) {
    if (result) {
        for (int i = 0; i < count; i++) free(result[i]);
        free(result);
    }
}

int main(void) {
    const char *test_string = "HelloWorldThisIsATest";
    int count = 0;
    char **parts = split_uppercase(test_string, &count);

    if (parts) {
        printf("Original: %s\n", test_string);
        printf("Split into %d parts:\n", count);
        for (int i = 0; i < count; i++)
            printf("  [%d]: %s\n", i, parts[i]);
        free_split_result(parts, count);
    } else {
        fprintf(stderr, "Failed to split string\n");
        return 1;
    }

    return 0;
}