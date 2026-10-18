#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_words_containing(char **list, int size, const char *substring) {
    int i, j;

    if (list == NULL || substring == NULL || size < 0) {
        return -1;
    }

    for (i = 0, j = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        if (strstr(list[i], substring) != NULL) {
            free(list[i]);
            list[i] = NULL;
        } else {
            list[j] = list[i];
            if (i != j) {
                list[i] = NULL;
            }
            j++;
        }
    }

    return j;
}

static char *copy_literal(const char *src, size_t size) {
    char *dst = malloc(size);
    if (dst != NULL) {
        snprintf(dst, size, "%s", src);
    }
    return dst;
}

int main(void) {
    char **words;
    int size = 6;
    int new_size;
    int i;

    words = malloc((size_t)size * sizeof(char *));
    if (words == NULL) {
        return 1;
    }

    for (i = 0; i < size; i++) {
        words[i] = NULL;
    }

    words[0] = copy_literal("apple", sizeof("apple"));
    words[1] = copy_literal("banana", sizeof("banana"));
    words[2] = copy_literal("cherry", sizeof("cherry"));
    words[3] = copy_literal("date", sizeof("date"));
    words[4] = copy_literal("elderberry", sizeof("elderberry"));
    words[5] = copy_literal("fig", sizeof("fig"));

    if (words[0] == NULL || words[1] == NULL || words[2] == NULL ||
        words[3] == NULL || words[4] == NULL || words[5] == NULL) {
        for (i = 0; i < size; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }

    printf("Original list:\n");
    for (i = 0; i < size; i++) {
        printf("%s\n", words[i]);
    }

    new_size = remove_words_containing(words, size, "an");

    if (new_size < 0) {
        for (i = 0; i < size; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }

    printf("\nList after removing words containing 'an':\n");
    for (i = 0; i < new_size; i++) {
        printf("%s\n", words[i]);
    }

    for (i = 0; i < size; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}