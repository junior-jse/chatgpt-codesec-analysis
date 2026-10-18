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

    size_t delim_len = strnlen(delimiter, SIZE_MAX);
    
    int max_elements = tuple_size - 1;
    char **result = (char **)malloc((size_t)max_elements * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    
    for (int i = 0; i < max_elements; i++) {
        size_t len1 = strnlen(tuple[i], SIZE_MAX);
        size_t len2 = strnlen(tuple[i + 1], SIZE_MAX);
        
        size_t total_len = len1 + delim_len + len2;
        /* Possible weaknesses found:
         *  Assuming that condition 'total_len<len1' is not redundant
         */
        if (total_len < len1 || total_len < len2 || total_len == SIZE_MAX) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }
        
        char *combined = (char *)malloc(total_len + 1);
        /* Possible weaknesses found:
         *  Assuming condition is false
         */
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
         *  remaining is assigned 'total_len' here.
         */
        size_t remaining = total_len;

        /* Possible weaknesses found:
         *  Condition 'remaining>=len1' is always true [knownConditionTrueFalse]
         *  Condition 'remaining>=len1' is always true
         */
        if (len1 > 0 && remaining >= len1) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(dest, tuple[i], len1);
            dest += len1;
            remaining -= len1;
        }
        if (delim_len > 0 && remaining >= delim_len) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(dest, delimiter, delim_len);
            dest += delim_len;
            remaining -= delim_len;
        }
        if (len2 > 0 && remaining >= len2) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(dest, tuple[i + 1], len2);
            dest += len2;
            /* Possible weaknesses found:
             *  Variable 'remaining' is assigned a value that is never used. [unreadVariable]
             *  Value stored to 'remaining' is never read [deadcode.DeadStores]
             */
            remaining -= len2;
        }
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