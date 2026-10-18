/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_UNIQUE_WORDS 1000

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

static int is_valid_string(const char *text, size_t max_len) {
    if (text == NULL) {
        return 0;
    }
    size_t i = 0;
    while (i < max_len && text[i] != '\0') {
        i++;
    }
    return (i < max_len);
}

static size_t safe_strlen(const char *text, size_t max_len) {
    size_t len = 0;
    while (len < max_len && text[len] != '\0') {
        len++;
    }
    return len;
}

void find_most_common_words(const char *text) {
    if (!is_valid_string(text, 100000)) {
        printf("Invalid input text.\n");
        return;
    }

    size_t text_len = safe_strlen(text, 100000);
    if (text_len == 0) {
        printf("Invalid input text.\n");
        return;
    }

    WordCount words[MAX_UNIQUE_WORDS];
    int unique_count = 0;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;

    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        if (isalnum((unsigned char)c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = (char)tolower((unsigned char)c);
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
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                size_t copy_len = strlen(buffer);
                if (copy_len >= MAX_WORD_LEN) {
                    copy_len = MAX_WORD_LEN - 1;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(words[unique_count].word, buffer, copy_len);
                words[unique_count].word[copy_len] = '\0';
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