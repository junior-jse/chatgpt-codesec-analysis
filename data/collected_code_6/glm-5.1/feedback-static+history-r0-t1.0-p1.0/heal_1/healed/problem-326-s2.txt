#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordEntry;

char *duplicate_string(const char *str, size_t max_len) {
    size_t len = strnlen(str, max_len);
    char *dup = malloc(len + 1);
    if (!dup) return NULL;
    memcpy(dup, str, len);
    dup[len] = '\0';
    return dup;
}

char *get_word_with_most_occurrences(char **strings, int num_strings) {
    if (!strings || num_strings <= 0) return NULL;

    int capacity = 128;
    int size = 0;
    WordEntry *entries = malloc(capacity * sizeof(WordEntry));
    if (!entries) return NULL;

    char *most_frequent_word = NULL;
    int max_count = 0;

    for (int i = 0; i < num_strings; i++) {
        if (!strings[i]) continue;

        size_t buffer_size = 1024;
        char *buffer = malloc(buffer_size);
        if (!buffer) {
            for (int e = 0; e < size; e++) free(entries[e].word);
            free(entries);
            free(most_frequent_word);
            return NULL;
        }
        strncpy(buffer, strings[i], buffer_size - 1);
        buffer[buffer_size - 1] = '\0';

        char *token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
        while (token != NULL) {
            size_t clean_capacity = 1024;
            char *clean_token = malloc(clean_capacity);
            if (!clean_token) {
                free(buffer);
                for (int e = 0; e < size; e++) free(entries[e].word);
                free(entries);
                free(most_frequent_word);
                return NULL;
            }
            int j = 0;
            for (int k = 0; token[k] != '\0'; k++) {
                if (isalpha((unsigned char)token[k])) {
                    if ((size_t)j < clean_capacity - 1) {
                        clean_token[j++] = tolower((unsigned char)token[k]);
                    }
                }
            }
            clean_token[j] = '\0';

            if (j > 0) {
                int found_index = -1;
                for (int e = 0; e < size; e++) {
                    if (strcmp(entries[e].word, clean_token) == 0) {
                        found_index = e;
                        break;
                    }
                }

                if (found_index != -1) {
                    entries[found_index].count++;
                    if (entries[found_index].count > max_count) {
                        max_count = entries[found_index].count;
                        free(most_frequent_word);
                        most_frequent_word = duplicate_string(entries[found_index].word, strlen(entries[found_index].word) + 1);
                    }
                } else {
                    if (size >= capacity) {
                        capacity *= 2;
                        WordEntry *new_entries = realloc(entries, capacity * sizeof(WordEntry));
                        if (!new_entries) {
                            free(clean_token);
                            free(buffer);
                            for (int e = 0; e < size; e++) free(entries[e].word);
                            free(entries);
                            free(most_frequent_word);
                            return NULL;
                        }
                        entries = new_entries;
                    }
                    entries[size].word = duplicate_string(clean_token, clean_capacity);
                    if (!entries[size].word) {
                        free(clean_token);
                        free(buffer);
                        for (int e = 0; e < size; e++) free(entries[e].word);
                        free(entries);
                        free(most_frequent_word);
                        return NULL;
                    }
                    entries[size].count = 1;
                    if (entries[size].count > max_count) {
                        max_count = entries[size].count;
                        free(most_frequent_word);
                        most_frequent_word = duplicate_string(entries[size].word, strlen(entries[size].word) + 1);
                    }
                    size++;
                }
            }
            free(clean_token);
            token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
        }
        free(buffer);
    }

    for (int e = 0; e < size; e++) {
        free(entries[e].word);
    }
    free(entries);

    return most_frequent_word;
}

int main() {
    char *strings[] = {
        "Hello world! Hello everyone.",
        "The world is a beautiful place.",
        "Hello again, world.",
        NULL
    };
    
    int num_strings = 3;
    
    char *result = get_word_with_most_occurrences(strings, num_strings);
    
    if (result) {
        printf("%s\n", result);
        free(result);
    } else {
        printf("No words found.\n");
    }
    
    return 0;
}