#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        if (words[i] != NULL && (int)strlen(words[i]) > n) {
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
            result[*shortlist_count] = strdup(words[i]);
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