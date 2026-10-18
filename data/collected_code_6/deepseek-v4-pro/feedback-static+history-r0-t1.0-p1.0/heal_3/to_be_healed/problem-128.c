/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **shortlist_words(char **words, int word_count, int n, int *result_count) {
    char **result = NULL;
    int count = 0;

    if (words == NULL || word_count <= 0 || n < 0 || result_count == NULL) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    result = (char **)malloc((size_t)word_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t len = strlen(words[i]);
            if (len > (size_t)n) {
                result[count] = (char *)malloc(len + 1);
                if (result[count] == NULL) {
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
                memcpy(result[count], words[i], len);
                result[count][len] = '\0';
                count++;
            }
        }
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    char **temp = (char **)realloc(result, (size_t)count * sizeof(char *));
    if (temp == NULL) {
        for (int j = 0; j < count; j++) {
            free(result[j]);
        }
        free(result);
        *result_count = 0;
        return NULL;
    }

    *result_count = count;
    return temp;
}

int main(void) {
    char *words[] = {"apple", "banana", "cat", "dog", "elephant"};
    int word_count = (int)(sizeof(words) / sizeof(words[0]));
    int result_count = 0;
    int threshold = 3;

    char **result = shortlist_words(words, word_count, threshold, &result_count);

    if (result == NULL) {
        printf("No words longer than %d found or error occurred.\n", threshold);
        return 0;
    }

    printf("Words longer than %d:\n", threshold);
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}