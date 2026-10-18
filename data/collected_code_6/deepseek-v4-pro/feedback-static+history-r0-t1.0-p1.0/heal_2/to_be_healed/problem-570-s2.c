/* Repair history (previous round):
 * - Flawfinder strcpy was replaced by strncpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            size_t len = strnlen(words[i], BUFSIZ);
            if (len >= BUFSIZ) {
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
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result[count], words[i], len);
            result[count][len] = '\0';
            count++;
        }
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    char **resized = (char **)realloc(result, (size_t)count * sizeof(char *));
    if (resized == NULL && count > 0) {
        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
        *result_count = 0;
        return NULL;
    }

    *result_count = count;
    return resized != NULL ? resized : result;
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
        size_t len = strnlen(initial_words[i], BUFSIZ);
        if (len >= BUFSIZ) {
            for (int j = 0; j < i; j++) {
                free(word_list[j]);
            }
            free(word_list);
            fprintf(stderr, "String too long or not null-terminated\n");
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
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
        strncpy(word_list[i], initial_words[i], len);
        word_list[i][len] = '\0';
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