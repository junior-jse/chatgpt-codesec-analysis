#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_words_containing(char **words, int count, const char *substring) {
    int i, j;
    int removed = 0;

    if (words == NULL || substring == NULL || count < 0) {
        return -1;
    }

    for (i = 0, j = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }
        if (strstr(words[i], substring) != NULL) {
            free(words[i]);
            words[i] = NULL;
            removed++;
        } else {
            words[j] = words[i];
            if (i != j) {
                words[i] = NULL;
            }
            j++;
        }
    }

    return removed;
}

int main(void) {
    char **words;
    int count = 6;
    int i;
    int removed;

    words = malloc((size_t)count * sizeof(char *));
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    words[0] = strdup("apple");
    words[1] = strdup("banana");
    words[2] = strdup("grape");
    words[3] = strdup("orange");
    words[4] = strdup("pineapple");
    words[5] = strdup("cherry");

    for (i = 0; i < count; i++) {
        if (words[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int k = 0; k < i; k++) {
                free(words[k]);
            }
            free(words);
            return EXIT_FAILURE;
        }
    }

    printf("Original list:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", words[i]);
    }

    removed = remove_words_containing(words, count, "apple");
    if (removed < 0) {
        fprintf(stderr, "Error removing words\n");
        for (i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
        return EXIT_FAILURE;
    }

    printf("\nAfter removing words containing \"apple\":\n");
    for (i = 0; i < count - removed; i++) {
        printf("%s\n", words[i]);
    }

    for (i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);

    return EXIT_SUCCESS;
}