#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **remove_duplicate_words(char **words, int count, int *new_count) {
    if (words == NULL || new_count == NULL) {
        return NULL;
    }

    char **result = malloc(count * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    *new_count = 0;

    for (int i = 0; i < count; i++) {
        if (words[i] == NULL) {
            free(result);
            return NULL;
        }

        int is_duplicate = 0;
        for (int j = 0; j < *new_count; j++) {
            if (strcmp(words[i], result[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            result[*new_count] = words[i];
            (*new_count)++;
        }
    }

    if (*new_count < count) {
        char **trimmed_result = realloc(result, *new_count * sizeof(char *));
        if (trimmed_result != NULL) {
            result = trimmed_result;
        }
    }

    return result;
}

int main(void) {
    char *words[] = {"apple", "banana", "apple", "orange", "banana", "grape"};
    int count = sizeof(words) / sizeof(words[0]);
    int new_count = 0;

    char **unique_words = remove_duplicate_words(words, count, &new_count);
    if (unique_words == NULL) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < new_count; i++) {
        printf("%s\n", unique_words[i]);
    }

    free(unique_words);

    return EXIT_SUCCESS;
}