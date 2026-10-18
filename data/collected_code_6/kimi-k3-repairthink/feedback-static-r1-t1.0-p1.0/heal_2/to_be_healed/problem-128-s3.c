#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 4096

char **shortlist_words(char **words, int count, int n, int *result_count)
{
    if (words == NULL || count < 0 || n < 0 || result_count == NULL) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    char **result = malloc((size_t)count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int j = 0;
    for (int i = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }

        size_t len = strnlen(words[i], (size_t)MAX_WORD_LEN + 1);
        if (len > (size_t)MAX_WORD_LEN) {
            continue;
        }

        if ((int)len > n) {
            result[j] = malloc(len + 1);
            if (result[j] == NULL) {
                for (int k = 0; k < j; k++) {
                    free(result[k]);
                }
                free(result);
                *result_count = 0;
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[j], words[i], len + 1);
            j++;
        }
    }

    *result_count = j;
    return result;
}

void free_words(char **words, int count)
{
    if (words == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

int main(void)
{
    char *words[] = {"apple", "hi", "banana", "cat", "elephant", "dog"};
    int count = 6;
    int n = 3;
    int result_count = 0;

    char **shortlisted = shortlist_words(words, count, n, &result_count);

    if (shortlisted == NULL && result_count == 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Words longer than %d:\n", n);
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", shortlisted[i]);
    }

    free_words(shortlisted, result_count);

    return EXIT_SUCCESS;
}