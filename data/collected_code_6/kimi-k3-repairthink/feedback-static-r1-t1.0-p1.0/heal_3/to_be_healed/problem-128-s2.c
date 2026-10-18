#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_WORD_LENGTH 4096

static size_t bounded_strlen(const char *s, size_t max_len)
{
    size_t len = 0;

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

char **shortlist_words(char **words, int count, int n, int *result_count)
{
    char **result;
    int i;
    int j = 0;

    if (words == NULL || count < 0 || n < 0 || result_count == NULL) {
        return NULL;
    }

    *result_count = 0;

    if ((size_t)count > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    result = malloc((size_t)(count > 0 ? count : 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        size_t len;
        /* Possible weaknesses found:
         *  The scope of the variable 'dest_size' can be reduced. [variableScope]
         */
        size_t dest_size;
        /* Possible weaknesses found:
         *  The scope of the variable 'k' can be reduced. [variableScope]
         */
        size_t k;

        if (words[i] == NULL) {
            continue;
        }

        len = bounded_strlen(words[i], MAX_WORD_LENGTH);
        if (len == MAX_WORD_LENGTH) {
            continue;
        }

        if ((int)len > n) {
            /* Possible weaknesses found:
             *  dest_size is assigned 'len+1' here.
             */
            dest_size = len + 1;
            result[j] = malloc(dest_size);
            /* Possible weaknesses found:
             *  Assuming condition is false
             */
            if (result[j] == NULL) {
                while (j > 0) {
                    j--;
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             *  Condition 'dest_size>=len+1' is always true
             *  Condition 'dest_size>=len+1' is always true [knownConditionTrueFalse]
             */
            if (dest_size >= len + 1) {
                for (k = 0; k <= len; k++) {
                    result[j][k] = words[i][k];
                }
            } else {
                result[j][0] = '\0';
            }
            j++;
        }
    }

    *result_count = j;
    return result;
}

void free_words(char **words, int count)
{
    int i;

    if (words == NULL || count < 0) {
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