#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 1024

static size_t safe_strnlen(const char *s, size_t max_len) {
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

char *get_most_frequent_word(char *words[], int count) {
    if (words == NULL || count <= 0) {
        return NULL;
    }

    int max_count = 0;
    char *most_frequent = NULL;

    for (int i = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }

        int current_count = 0;
        for (int j = 0; j < count; j++) {
            if (words[j] != NULL && strncmp(words[i], words[j], MAX_WORD_LEN) == 0) {
                current_count++;
            }
        }

        if (current_count > max_count) {
            max_count = current_count;
            most_frequent = words[i];
        }
    }

    if (most_frequent == NULL) {
        return NULL;
    }

    size_t len = safe_strnlen(most_frequent, MAX_WORD_LEN);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, most_frequent, len);
    result[len] = '\0';

    return result;
}

int main(void) {
    char *words[] = {
        "apple",
        "banana",
        "apple",
        "orange",
        "banana",
        "apple",
        "grape"
    };
    int count = (int)(sizeof(words) / sizeof(words[0]));

    char *result = get_most_frequent_word(words, count);

    if (result != NULL) {
        printf("Most frequent word: %s\n", result);
        free(result);
    } else {
        printf("No words found or error occurred.\n");
    }

    return 0;
}