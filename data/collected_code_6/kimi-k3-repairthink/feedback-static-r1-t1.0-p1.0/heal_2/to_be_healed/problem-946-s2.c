#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_UNIQUE_WORDS 1000
#define MAX_TEXT_LEN (1024U * 1024U)

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

static void add_word(WordCount *words, int *unique_count, const char *buffer)
{
    for (int j = 0; j < *unique_count; j++) {
        if (strcmp(words[j].word, buffer) == 0) {
            words[j].count++;
            return;
        }
    }
    if (*unique_count < MAX_UNIQUE_WORDS) {
        snprintf(words[*unique_count].word,
                 sizeof words[*unique_count].word, "%s", buffer);
        words[*unique_count].count = 1;
        (*unique_count)++;
    }
}

void find_most_common_words(const char *text)
{
    if (text == NULL) {
        printf("Invalid input text.\n");
        return;
    }

    size_t text_len = strnlen(text, MAX_TEXT_LEN);
    if (text_len == 0 || text_len >= MAX_TEXT_LEN) {
        printf("Invalid input text.\n");
        return;
    }

    WordCount words[MAX_UNIQUE_WORDS];
    int unique_count = 0;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[MAX_WORD_LEN];
    size_t buf_index = 0;

    for (size_t i = 0; i < text_len; i++) {
        unsigned char c = (unsigned char)text[i];
        if (isalnum(c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = (char)tolower(c);
            }
        } else if (buf_index > 0) {
            buffer[buf_index] = '\0';
            add_word(words, &unique_count, buffer);
            buf_index = 0;
        }
    }

    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(words, &unique_count, buffer);
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

int main(void)
{
    const char *text = "The quick brown fox jumps over the lazy dog. The dog was not amused by the fox.";
    find_most_common_words(text);
    return 0;
}