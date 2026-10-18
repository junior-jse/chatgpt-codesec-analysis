#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **remove_words_containing(char **words, int word_count, const char *substring, int *new_count) {
    if (words == NULL || substring == NULL || word_count <= 0 || new_count == NULL) {
        if (new_count != NULL) {
            *new_count = 0;
        }
        return NULL;
    }

    size_t substr_len = 0;
    while (substring[substr_len] != '\0') {
        if (substr_len >= (size_t)-1 - 1) {
            *new_count = 0;
            return NULL;
        }
        substr_len++;
    }

    if (substr_len == 0) {
        *new_count = 0;
        return NULL;
    }

    char **result = NULL;
    int result_count = 0;
    int result_capacity = 0;

    for (int i = 0; i < word_count; i++) {
        if (words[i] == NULL) {
            continue;
        }

        size_t word_len = 0;
        while (words[i][word_len] != '\0') {
            if (word_len >= (size_t)-1 - 1) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                *new_count = 0;
                return NULL;
            }
            word_len++;
        }

        if (word_len < substr_len) {
            if (result_count >= result_capacity) {
                int new_capacity = result_capacity == 0 ? 4 : result_capacity * 2;
                if (new_capacity < result_capacity) {
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    *new_count = 0;
                    return NULL;
                }
                char **temp = (char **)realloc(result, (size_t)new_capacity * sizeof(char *));
                if (temp == NULL) {
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    *new_count = 0;
                    return NULL;
                }
                result = temp;
                result_capacity = new_capacity;
            }

            if (word_len + 1 < word_len) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                *new_count = 0;
                return NULL;
            }
            result[result_count] = (char *)malloc(word_len + 1);
            if (result[result_count] == NULL) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                *new_count = 0;
                return NULL;
            }

            for (size_t k = 0; k < word_len; k++) {
                result[result_count][k] = words[i][k];
            }
            result[result_count][word_len] = '\0';
            result_count++;
            continue;
        }

        int found = 0;
        for (size_t pos = 0; pos <= word_len - substr_len; pos++) {
            int match = 1;
            for (size_t k = 0; k < substr_len; k++) {
                if (words[i][pos + k] != substring[k]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                found = 1;
                break;
            }
        }

        if (found) {
            continue;
        }

        if (result_count >= result_capacity) {
            int new_capacity = result_capacity == 0 ? 4 : result_capacity * 2;
            if (new_capacity < result_capacity) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                *new_count = 0;
                return NULL;
            }
            char **temp = (char **)realloc(result, (size_t)new_capacity * sizeof(char *));
            if (temp == NULL) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                *new_count = 0;
                return NULL;
            }
            result = temp;
            result_capacity = new_capacity;
        }

        if (word_len + 1 < word_len) {
            for (int j = 0; j < result_count; j++) {
                free(result[j]);
            }
            free(result);
            *new_count = 0;
            return NULL;
        }
        result[result_count] = (char *)malloc(word_len + 1);
        if (result[result_count] == NULL) {
            for (int j = 0; j < result_count; j++) {
                free(result[j]);
            }
            free(result);
            *new_count = 0;
            return NULL;
        }

        for (size_t k = 0; k < word_len; k++) {
            result[result_count][k] = words[i][k];
        }
        result[result_count][word_len] = '\0';
        result_count++;
    }

    if (result_count == 0) {
        free(result);
        *new_count = 0;
        return NULL;
    }

    *new_count = result_count;
    return result;
}

int main(void) {
    char *words[] = {
        "apple",
        "banana",
        "cherry",
        "date",
        "elderberry",
        "fig",
        "grape"
    };
    int word_count = sizeof(words) / sizeof(words[0]);
    int new_count = 0;

    char **filtered = remove_words_containing(words, word_count, "a", &new_count);

    if (filtered != NULL) {
        printf("Filtered words:\n");
        for (int i = 0; i < new_count; i++) {
            printf("%s\n", filtered[i]);
            free(filtered[i]);
        }
        free(filtered);
    } else {
        printf("No words remain after filtering.\n");
    }

    return 0;
}