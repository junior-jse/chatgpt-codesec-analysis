#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **shortlist_words(char **words, int count, int n, int *result_count) {
    if (words == NULL || result_count == NULL || count < 0 || n < 0) {
        return NULL;
    }
    
    char **result = malloc(count * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    int j = 0;
    for (int i = 0; i < count; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        if (words[i] != NULL && strlen(words[i]) > (size_t)n) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            result[j] = malloc(strlen(words[i]) + 1);
            if (result[j] == NULL) {
                for (int k = 0; k < j; k++) {
                    free(result[k]);
                }
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(result[j], words[i]);
            j++;
        }
    }
    
    *result_count = j;
    return result;
}

void free_words(char **words, int count) {
    if (words == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

int main(void) {
    char *words[] = {"apple", "hi", "banana", "cat", "elephant", "dog"};
    int count = 6;
    int n = 3;
    int result_count = 0;
    
    char **result = shortlist_words(words, count, n, &result_count);
    if (result == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Words longer than %d characters:\n", n);
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
    }
    
    free_words(result, result_count);
    return EXIT_SUCCESS;
}