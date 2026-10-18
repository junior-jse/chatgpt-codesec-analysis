#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t safe_strnlen(const char *s, size_t maxlen)
{
    size_t i;
    if (s == NULL) {
        return 0;
    }
    for (i = 0; i < maxlen; i++) {
        if (s[i] == '\0') {
            break;
        }
    }
    return i;
}

char **shortlist_words(char **words, int count, int n, int *result_count)
{
    char **result;
    int i;
    int j = 0;
    size_t len;

    if (words == NULL || count < 0 || n < 0 || result_count == NULL) {
        return NULL;
    }

    result = calloc((size_t)count, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        if (words[i] != NULL) {
            len = safe_strnlen(words[i], (size_t)n + 2);
            if (len > (size_t)n && len <= (size_t)n + 1) {
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                len = strlen(words[i]);
                result[j] = malloc(len + 1);
                if (result[j] == NULL) {
                    while (j > 0) {
                        j--;
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
                snprintf(result[j], len + 1, "%s", words[i]);
                j++;
            }
        }
    }

    *result_count = j;
    return result;
}

void free_words(char **words, int count)
{
    int i;

    if (words == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
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
    char **result;
    int i;

    result = shortlist_words(words, count, n, &result_count);
    if (result == NULL) {
        fprintf(stderr, "Error: failed to shortlist words\n");
        return EXIT_FAILURE;
    }

    printf("Words longer than %d characters:\n", n);
    for (i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
    }

    free_words(result, result_count);
    return EXIT_SUCCESS;
}