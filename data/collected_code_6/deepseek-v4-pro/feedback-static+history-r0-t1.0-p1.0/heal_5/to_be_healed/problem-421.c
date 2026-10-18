#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **concat_elements(char **tuple, int tuple_size, const char *delimiter, int *result_count) {
    if (tuple == NULL || delimiter == NULL || result_count == NULL) {
        return NULL;
    }

    if (tuple_size <= 0) {
        *result_count = 0;
        return NULL;
    }

    for (int i = 0; i < tuple_size; i++) {
        if (tuple[i] == NULL) {
            *result_count = 0;
            return NULL;
        }
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t delim_len = strlen(delimiter);
    
    int max_elements = tuple_size - 1;
    char **result = (char **)malloc((size_t)max_elements * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    
    for (int i = 0; i < max_elements; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len1 = strlen(tuple[i]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len2 = strlen(tuple[i + 1]);
        
        if (len1 > SIZE_MAX - delim_len || len1 + delim_len > SIZE_MAX - len2) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }
        
        size_t total_len = len1 + delim_len + len2;
        
        char *combined = (char *)malloc(total_len + 1);
        if (combined == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        char *dest = combined;
        
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, tuple[i], len1);
        dest += len1;
        
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, delimiter, delim_len);
        dest += delim_len;
        
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, tuple[i + 1], len2);
        dest += len2;
        
        *dest = '\0';
        result[count++] = combined;
    }

    *result_count = count;
    return result;
}

int main(void) {
    const char *tuple[] = {"hello", "world", "foo", "bar"};
    int result_count = 0;
    char **result = concat_elements((char **)tuple, 4, "-", &result_count);

    if (result == NULL) {
        fprintf(stderr, "Failed to concatenate elements\n");
        return 1;
    }

    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}