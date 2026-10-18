#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SAFE_STRING_LENGTH 4096

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static int contains_substring(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) {
        return 0;
    }
    return strstr(str, substr) != NULL;
}

static char **remove_words(char **words, int word_count, const char *filter, int *result_count) {
    if (words == NULL || filter == NULL || result_count == NULL || word_count <= 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    char **result = (char **)malloc(word_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i] == NULL) {
            continue;
        }
        if (!contains_substring(words[i], filter)) {
            size_t len = safe_strlen(words[i], MAX_SAFE_STRING_LENGTH);
            result[count] = (char *)malloc(len + 1);
            if (result[count] == NULL) {
                for (int j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                *result_count = 0;
                return NULL;
            }
            snprintf(result[count], len + 1, "%s", words[i]);
            count++;
        }
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    char **resized = (char **)realloc(result, count * sizeof(char *));
    if (resized == NULL) {
        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
        *result_count = 0;
        return NULL;
    }

    *result_count = count;
    return resized;
}

static void free_words(char **words, int count) {
    if (words == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

int main(void) {
    const char *initial_words[] = {
        "apple",
        "banana",
        "cherry",
        "date",
        "elderberry",
        "fig",
        "grape"
    };
    int initial_count = sizeof(initial_words) / sizeof(initial_words[0]);

    char **word_list = (char **)malloc(initial_count * sizeof(char *));
    if (word_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < initial_count; i++) {
        size_t len = safe_strlen(initial_words[i], MAX_SAFE_STRING_LENGTH);
        word_list[i] = (char *)malloc(len + 1);
        if (word_list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(word_list[j]);
            }
            free(word_list);
            fprintf(stderr, "Memory allocation failed\n");
            return EXIT_FAILURE;
        }
        snprintf(word_list[i], len + 1, "%s", initial_words[i]);
    }

    int result_count = 0;
    char **filtered = remove_words(word_list, initial_count, "a", &result_count);

    if (filtered != NULL) {
        for (int i = 0; i < result_count; i++) {
            printf("%s\n", filtered[i]);
        }
        free_words(filtered, result_count);
    } else {
        printf("No words found after filtering\n");
    }

    free_words(word_list, initial_count);

    return EXIT_SUCCESS;
}