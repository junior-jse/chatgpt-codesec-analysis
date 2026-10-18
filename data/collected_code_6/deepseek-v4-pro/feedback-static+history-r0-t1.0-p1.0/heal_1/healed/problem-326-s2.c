#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *most_frequent_word(char **strings, int count) {
    if (strings == NULL || count <= 0) {
        return NULL;
    }

    char **words = NULL;
    int *freqs = NULL;
    int unique_count = 0;
    int capacity = 0;

    for (int i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        
        char *current = strings[i];
        char *token = strtok(current, " \t\n");
        while (token != NULL) {
            int found = 0;
            for (int j = 0; j < unique_count; j++) {
                if (strcmp(words[j], token) == 0) {
                    freqs[j]++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (unique_count == capacity) {
                    capacity = capacity == 0 ? 16 : capacity * 2;
                    char **new_words = realloc(words, capacity * sizeof(char *));
                    int *new_freqs = realloc(freqs, capacity * sizeof(int));
                    if (new_words == NULL || new_freqs == NULL) {
                        free(new_words);
                        free(new_freqs);
                        for (int k = 0; k < unique_count; k++) {
                            free(words[k]);
                        }
                        free(words);
                        free(freqs);
                        return NULL;
                    }
                    words = new_words;
                    freqs = new_freqs;
                }
                
                size_t token_len = strlen(token);
                words[unique_count] = malloc(token_len + 1);
                if (words[unique_count] == NULL) {
                    for (int k = 0; k < unique_count; k++) {
                        free(words[k]);
                    }
                    free(words);
                    free(freqs);
                    return NULL;
                }
                memcpy(words[unique_count], token, token_len + 1);
                freqs[unique_count] = 1;
                unique_count++;
            }
            token = strtok(NULL, " \t\n");
        }
    }

    if (unique_count == 0) {
        free(words);
        free(freqs);
        return NULL;
    }

    int max_index = 0;
    for (int i = 1; i < unique_count; i++) {
        if (freqs[i] > freqs[max_index]) {
            max_index = i;
        }
    }

    size_t result_len = strlen(words[max_index]);
    char *result = malloc(result_len + 1);
    if (result == NULL) {
        for (int k = 0; k < unique_count; k++) {
            free(words[k]);
        }
        free(words);
        free(freqs);
        return NULL;
    }
    memcpy(result, words[max_index], result_len + 1);

    for (int k = 0; k < unique_count; k++) {
        free(words[k]);
    }
    free(words);
    free(freqs);

    return result;
}

int main(void) {
    char str1[] = "hello world hello";
    char str2[] = "world of programming";
    char str3[] = "hello from C";
    char *strings[] = {str1, str2, str3};

    char *result = most_frequent_word(strings, 3);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    return 0;
}