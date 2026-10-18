#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **remove_words_containing(char **words, int word_count, const char *substring, int *new_count) {
    if (words == NULL || substring == NULL || word_count <= 0 || new_count == NULL) {
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

        if (strstr(words[i], substring) != NULL) {
            continue;
        }

        if (result_count >= result_capacity) {
            int new_capacity = result_capacity == 0 ? 4 : result_capacity * 2;
            char **temp = (char **)realloc(result, new_capacity * sizeof(char *));
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

        result[result_count] = (char *)malloc(strlen(words[i]) + 1);
        if (result[result_count] == NULL) {
            for (int j = 0; j < result_count; j++) {
                free(result[j]);
            }
            free(result);
            *new_count = 0;
            return NULL;
        }
        strcpy(result[result_count], words[i]);
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