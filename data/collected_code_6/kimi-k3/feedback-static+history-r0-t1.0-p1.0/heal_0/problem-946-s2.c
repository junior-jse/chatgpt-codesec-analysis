#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_UNIQUE_WORDS 1000

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

void find_most_common_words(const char *text) {
    if (text == NULL || strlen(text) == 0) {
        printf("Invalid input text.\n");
        return;
    }

    WordCount words[MAX_UNIQUE_WORDS];
    int unique_count = 0;
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    size_t text_len = strlen(text);

    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        if (isalnum((unsigned char)c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)c);
            }
        } else if (buf_index > 0) {
            buffer[buf_index] = '\0';
            int found = 0;
            for (int j = 0; j < unique_count; j++) {
                if (strcmp(words[j].word, buffer) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            if (!found && unique_count < MAX_UNIQUE_WORDS) {
                strncpy(words[unique_count].word, buffer, MAX_WORD_LEN - 1);
                words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
                words[unique_count].count = 1;
                unique_count++;
            }
            buf_index = 0;
        }
    }

    if (unique_count == 0) {
        printf("No words found in text.\n");
        return;
    }

    int max_count = 0;
    for (int i = 0; i < unique_count; i++) {
        if (words[i].count > max_count) {
            max_count = words[i].count;
        }
    }

    printf("Most common word(s) with count %d:\n", max_count);
    for (int i = 0; i < unique_count; i++) {
        if (words[i].count == max_count) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    const char *text = "The quick brown fox jumps over the lazy dog. The dog was not amused by the fox.";
    find_most_common_words(text);
    return 0;
}