#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** remove_duplicate_words(char** words, int count, int* out_count) {
    if (words == NULL || out_count == NULL) {
        return NULL;
    }

    char** result = malloc(count * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int unique_count = 0;

    for (int i = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }

        int is_duplicate = 0;
        for (int j = 0; j < unique_count; j++) {
            if (result[j] != NULL && strcmp(words[i], result[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            result[unique_count] = words[i];
            unique_count++;
        }
    }

    char** trimmed_result = realloc(result, unique_count * sizeof(char*));
    if (trimmed_result != NULL) {
        result = trimmed_result;
    }

    *out_count = unique_count;
    return result;
}

int main() {
    char* words[] = {"apple", "banana", "apple", "orange", "banana", "grape"};
    int count = sizeof(words) / sizeof(words[0]);
    int out_count = 0;

    char** unique_words = remove_duplicate_words(words, count, &out_count);

    if (unique_words != NULL) {
        for (int i = 0; i < out_count; i++) {
            printf("%s\n", unique_words[i]);
        }
        free(unique_words);
    }

    return 0;
}