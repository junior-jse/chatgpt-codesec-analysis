#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 1024

char **shortlist_words(char **words, int word_count, int n, int *result_count) {
    char **result = NULL;
    int count = 0;

    if (words == NULL || word_count <= 0 || n < 0 || result_count == NULL) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    result = (char **)malloc(word_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL) {
            size_t wordlen = 0;
            while (wordlen < MAX_WORD_LENGTH && words[i][wordlen] != '\0') {
                wordlen++;
            }
            if (wordlen < MAX_WORD_LENGTH && wordlen > (size_t)n) {
                result[count] = (char *)malloc((wordlen + 1) * sizeof(char));
                if (result[count] == NULL) {
                    for (int j = 0; j < count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    *result_count = 0;
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                 */
                strncpy(result[count], words[i], wordlen);
                result[count][wordlen] = '\0';
                count++;
            }
        }
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    char **temp = (char **)realloc(result, count * sizeof(char *));
    if (temp == NULL) {
        for (int j = 0; j < count; j++) {
            free(result[j]);
        }
        free(result);
        *result_count = 0;
        return NULL;
    }

    result = temp;
    *result_count = count;
    return result;
}

int main(void) {
    char *words[] = {"apple", "banana", "cat", "dog", "elephant"};
    int word_count = sizeof(words) / sizeof(words[0]);
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