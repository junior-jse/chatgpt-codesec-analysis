#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **filter_long_words(char **words, int word_count, int n, int *result_count) {
    if (words == NULL || result_count == NULL || word_count < 0 || n < 0) {
        if (result_count != NULL) *result_count = 0;
        return NULL;
    }

    char **shortlist = malloc(word_count * sizeof(char *));
    if (shortlist == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL && strlen(words[i]) > n) {
            shortlist[count] = words[i];
            count++;
        }
    }

    if (count == 0) {
        free(shortlist);
        *result_count = 0;
        return NULL;
    }

    char **resized_shortlist = realloc(shortlist, count * sizeof(char *));
    if (resized_shortlist == NULL) {
        *result_count = count;
        return shortlist;
    }

    *result_count = count;
    return resized_shortlist;
}

int main() {
    char *dictionary[] = {"apple", "banana", "kiwi", "strawberry", "fig", "pineapple"};
    int total_words = sizeof(dictionary) / sizeof(dictionary[0]);
    int n = 5;
    int result_count = 0;

    char **long_words = filter_long_words(dictionary, total_words, n, &result_count);

    if (long_words != NULL) {
        for (int i = 0; i < result_count; i++) {
            printf("%s\n", long_words[i]);
        }
        free(long_words);
    }

    return 0;
}