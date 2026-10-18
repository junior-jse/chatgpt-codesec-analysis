#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef struct {
    char *word;
    int count;
} WordCount;

void find_most_common(const char *text) {
    if (text == NULL) {
        return;
    }

    size_t len = strlen(text);
    if (len == 0) {
        return;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return;
    }

    memcpy(buffer, text, len + 1);

    WordCount *word_list = NULL;
    size_t word_list_size = 0;
    size_t word_list_capacity = 0;
    int max_count = 0;

    char *token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}<>-");
    while (token != NULL) {
        size_t start = 0;
        while (token[start] && !isalpha((unsigned char)token[start])) {
            start++;
        }
        
        size_t end = start;
        while (token[end] && isalpha((unsigned char)token[end])) {
            token[end] = (char)tolower((unsigned char)token[end]);
            end++;
        }
        
        if (start == end) {
            token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}<>-");
            continue;
        }

        token[end] = '\0';
        char *current_word = token + start;

        int found = 0;
        for (size_t i = 0; i < word_list_size; i++) {
            if (strcmp(word_list[i].word, current_word) == 0) {
                word_list[i].count++;
                if (word_list[i].count > max_count) {
                    max_count = word_list[i].count;
                }
                found = 1;
                break;
            }
        }

        if (!found) {
            if (word_list_size >= word_list_capacity) {
                size_t new_capacity = word_list_capacity == 0 ? 16 : word_list_capacity * 2;
                WordCount *new_list = (WordCount *)realloc(word_list, new_capacity * sizeof(WordCount));
                if (new_list == NULL) {
                    for (size_t i = 0; i < word_list_size; i++) {
                        free(word_list[i].word);
                    }
                    free(word_list);
                    free(buffer);
                    return;
                }
                word_list = new_list;
                word_list_capacity = new_capacity;
            }

            char *new_word = (char *)malloc(strlen(current_word) + 1);
            if (new_word == NULL) {
                for (size_t i = 0; i < word_list_size; i++) {
                    free(word_list[i].word);
                }
                free(word_list);
                free(buffer);
                return;
            }
            strcpy(new_word, current_word);

            word_list[word_list_size].word = new_word;
            word_list[word_list_size].count = 1;
            if (max_count < 1) {
                max_count = 1;
            }
            word_list_size++;
        }

        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}<>-");
    }

    if (max_count > 0) {
        printf("Most common elements (count: %d):\n", max_count);
        for (size_t i = 0; i < word_list_size; i++) {
            if (word_list[i].count == max_count) {
                printf("%s\n", word_list[i].word);
            }
        }
    }

    for (size_t i = 0; i < word_list_size; i++) {
        free(word_list[i].word);
    }
    free(word_list);
    free(buffer);
}

int main(void) {
    const char *sample_text = "Hello world! This is a test. Hello, this test is only a test.";
    find_most_common(sample_text);
    return 0;
}