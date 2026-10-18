#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strndup_safe(const char *s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    char *new_s = malloc(len + 1);
    if (new_s == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(new_s, s, len);
    new_s[len] = '\0';
    return new_s;
}

char **shortlist_words(char **words, int word_count, int n, int *shortlist_count) {
    if (words == NULL || shortlist_count == NULL) {
        if (shortlist_count != NULL) {
            *shortlist_count = 0;
        }
        return NULL;
    }

    char **result = NULL;
    *shortlist_count = 0;

    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL) {
            size_t len = 0;
            while (words[i][len] != '\0' && len <= (size_t)n) {
                len++;
            }
            if ((int)len > n) {
                char **temp = realloc(result, (*shortlist_count + 1) * sizeof(char *));
                if (temp == NULL) {
                    for (int j = 0; j < *shortlist_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    *shortlist_count = 0;
                    return NULL;
                }
                result = temp;
                result[*shortlist_count] = strndup_safe(words[i], len);
                if (result[*shortlist_count] == NULL) {
                    for (int j = 0; j < *shortlist_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    *shortlist_count = 0;
                    return NULL;
                }
                (*shortlist_count)++;
            }
        }
    }

    return result;
}

void free_shortlist(char **shortlist, int shortlist_count) {
    if (shortlist != NULL) {
        for (int i = 0; i < shortlist_count; i++) {
            free(shortlist[i]);
        }
        free(shortlist);
    }
}

int main(void) {
    char *words[] = {"apple", "banana", "kiwi", "strawberry", "fig", "pineapple"};
    int word_count = sizeof(words) / sizeof(words[0]);
    int n = 5;
    int shortlist_count = 0;

    char **shortlist = shortlist_words(words, word_count, n, &shortlist_count);

    if (shortlist != NULL) {
        printf("Words longer than %d:\n", n);
        for (int i = 0; i < shortlist_count; i++) {
            printf("%s\n", shortlist[i]);
        }
        free_shortlist(shortlist, shortlist_count);
    }

    return 0;
}