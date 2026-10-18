#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int contains_substring(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) {
        return 0;
    }
    return strstr(str, substr) != NULL;
}

char **remove_words(char **words, int word_count, const char *filter, int *result_count) {
    if (words == NULL || filter == NULL || result_count == NULL || word_count <= 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    char **result = (char **)malloc((size_t)word_count * sizeof(char *));
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
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t len = strlen(words[i]);
            if (len >= (size_t)(INT_MAX - 1)) {
                continue;
            }
            result[count] = (char *)malloc(len + 1);
            if (result[count] == NULL) {
                for (int j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                *result_count = 0;
                return NULL;
            }
            if (len + 1 <= len) {
                free(result[count]);
                continue;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[count], words[i], len + 1);
            count++;
        }
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    char **resized = (char **)realloc(result, (size_t)count * sizeof(char *));
    if (resized == NULL) {
        *result_count = count;
        return result;
    }

    *result_count = count;
    return resized;
}

void free_words(char **words, int count) {
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
    int initial_count = (int)(sizeof(initial_words) / sizeof(initial_words[0]));

    char **word_list = (char **)malloc((size_t)initial_count * sizeof(char *));
    if (word_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < initial_count; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len = strlen(initial_words[i]);
        if (len >= (size_t)(INT_MAX - 1)) {
            for (int j = 0; j < i; j++) {
                free(word_list[j]);
            }
            free(word_list);
            fprintf(stderr, "String too long\n");
            return EXIT_FAILURE;
        }
        word_list[i] = (char *)malloc(len + 1);
        if (word_list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(word_list[j]);
            }
            free(word_list);
            fprintf(stderr, "Memory allocation failed\n");
            return EXIT_FAILURE;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(word_list[i], initial_words[i], len + 1);
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