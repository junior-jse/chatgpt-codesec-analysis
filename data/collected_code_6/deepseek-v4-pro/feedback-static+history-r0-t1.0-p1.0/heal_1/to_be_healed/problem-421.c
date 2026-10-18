#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **concat_elements(char **tuple, int tuple_size, const char *delimiter, int *result_count) {
    /* Possible weaknesses found:
     *  Assuming that condition 'result_count==NULL' is not redundant
     */
    if (tuple == NULL || delimiter == NULL || result_count == NULL) {
        /* Possible weaknesses found:
         *  Null pointer dereference
         *  Either the condition 'result_count==NULL' is redundant or there is possible null pointer dereference: result_count. [nullPointerRedundantCheck]
         */
        *result_count = 0;
        return NULL;
    }

    if (tuple_size <= 0) {
        *result_count = 0;
        return NULL;
    }

    char **result = (char **)malloc((tuple_size - 1) * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < tuple_size - 1; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len1 = strlen(tuple[i]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len2 = strlen(tuple[i + 1]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t delim_len = strlen(delimiter);
        char *combined = (char *)malloc(len1 + len2 + delim_len + 1);
        if (combined == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(combined, tuple[i], len1);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(combined + len1, delimiter, delim_len);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(combined + len1 + delim_len, tuple[i + 1], len2);
        combined[len1 + len2 + delim_len] = '\0';
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