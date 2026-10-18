#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordEntry;

char *get_most_frequent_word(char **strings, int num_strings) {
    WordEntry *entries = NULL;
    int entry_count = 0;
    int entry_capacity = 0;
    char *result = NULL;

    for (int i = 0; i < num_strings; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        char *current = strings[i];
        while (*current != '\0') {
            while (*current != '\0' && !isalpha((unsigned char)*current)) {
                current++;
            }

            if (*current == '\0') {
                break;
            }

            char *start = current;
            while (*current != '\0' && isalpha((unsigned char)*current)) {
                current++;
            }

            size_t len = current - start;
            if (len == 0) {
                continue;
            }

            char *word = malloc(len + 1);
            if (word == NULL) {
                for (int j = 0; j < entry_count; j++) {
                    free(entries[j].word);
                }
                free(entries);
                return NULL;
            }

            memcpy(word, start, len);
            word[len] = '\0';

            int found_index = -1;
            for (int j = 0; j < entry_count; j++) {
                if (strcmp(entries[j].word, word) == 0) {
                    found_index = j;
                    break;
                }
            }

            if (found_index != -1) {
                free(word);
                entries[found_index].count++;
            } else {
                if (entry_count >= entry_capacity) {
                    int new_capacity = entry_capacity == 0 ? 16 : entry_capacity * 2;
                    WordEntry *new_entries = realloc(entries, new_capacity * sizeof(WordEntry));
                    if (new_entries == NULL) {
                        free(word);
                        for (int j = 0; j < entry_count; j++) {
                            free(entries[j].word);
                        }
                        free(entries);
                        return NULL;
                    }
                    entries = new_entries;
                    entry_capacity = new_capacity;
                }
                entries[entry_count].word = word;
                entries[entry_count].count = 1;
                entry_count++;
            }
        }
    }

    if (entry_count == 0) {
        free(entries);
        return NULL;
    }

    int max_index = 0;
    for (int i = 1; i < entry_count; i++) {
        if (entries[i].count > entries[max_index].count) {
            max_index = i;
        }
    }

    result = entries[max_index].word;

    for (int i = 0; i < entry_count; i++) {
        if (i != max_index) {
            free(entries[i].word);
        }
    }
    free(entries);

    return result;
}

int main() {
    char *strings[] = {
        "hello world",
        "hello there",
        "world of c",
        "hello again"
    };

    int num_strings = sizeof(strings) / sizeof(strings[0]);

    char *most_frequent = get_most_frequent_word(strings, num_strings);

    if (most_frequent != NULL) {
        printf("%s\n", most_frequent);
        free(most_frequent);
    } else {
        printf("No words found\n");
    }

    return 0;
}