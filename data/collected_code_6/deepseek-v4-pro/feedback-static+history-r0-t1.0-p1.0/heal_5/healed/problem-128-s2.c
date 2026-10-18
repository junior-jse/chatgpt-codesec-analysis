#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

char **shortlist_long_words(const char *words[], int word_count, size_t n, int *out_count) {
    if (words == NULL || out_count == NULL || word_count <= 0) {
        if (out_count != NULL) {
            *out_count = 0;
        }
        return NULL;
    }

    char **result = malloc((size_t)word_count * sizeof(char *));
    if (result == NULL) {
        *out_count = 0;
        return NULL;
    }

    int count = 0;
    size_t max_safe_len = n + 1;

    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL) {
            size_t len = safe_strlen(words[i], max_safe_len);
            if (len > n) {
                result[count] = (char *)words[i];
                count++;
            }
        }
    }

    if (count == 0) {
        free(result);
        *out_count = 0;
        return NULL;
    }

    char **shrunk = realloc(result, (size_t)count * sizeof(char *));
    if (shrunk == NULL) {
        free(result);
        *out_count = 0;
        return NULL;
    }

    *out_count = count;
    return shrunk;
}

int main(void) {
    const char *words[] = {"apple", "banana", "kiwi", "strawberry", "plum", "watermelon"};
    int word_count = (int)(sizeof(words) / sizeof(words[0]));
    size_t n = 5;
    int shortlisted_count = 0;

    char **shortlisted = shortlist_long_words(words, word_count, n, &shortlisted_count);

    if (shortlisted == NULL || shortlisted_count == 0) {
        printf("No words longer than %zu characters found.\n", n);
        return 0;
    }

    printf("Words longer than %zu characters:\n", n);
    for (int i = 0; i < shortlisted_count; i++) {
        printf("%s\n", shortlisted[i]);
    }

    free(shortlisted);
    return 0;
}