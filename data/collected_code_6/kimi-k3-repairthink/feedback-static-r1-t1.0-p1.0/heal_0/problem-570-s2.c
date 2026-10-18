#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **remove_words_containing(char **list, size_t count, const char *substring, size_t *new_count) {
    if (list == NULL || substring == NULL || new_count == NULL) {
        return NULL;
    }

    char **result = malloc(count * sizeof(char *));
    if (result == NULL && count > 0) {
        return NULL;
    }

    size_t kept = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] == NULL) {
            continue;
        }
        if (strstr(list[i], substring) == NULL) {
            result[kept] = malloc(strlen(list[i]) + 1);
            if (result[kept] == NULL) {
                for (size_t j = 0; j < kept; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[kept], list[i]);
            kept++;
        }
    }

    *new_count = kept;

    if (kept == 0) {
        free(result);
        return NULL;
    }

    char **shrunk = realloc(result, kept * sizeof(char *));
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

void free_string_list(char **list, size_t count) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    char *words[] = {
        "apple",
        "banana",
        "cherry",
        "grape",
        "pineapple",
        "orange"
    };
    size_t count = sizeof(words) / sizeof(words[0]);
    const char *filter = "apple";
    size_t new_count = 0;

    char **filtered = remove_words_containing(words, count, filter, &new_count);

    if (filtered != NULL) {
        printf("Filtered list:\n");
        for (size_t i = 0; i < new_count; i++) {
            printf("%s\n", filtered[i]);
        }
        free_string_list(filtered, new_count);
    } else if (new_count == 0) {
        printf("No words remain after filtering.\n");
    } else {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}