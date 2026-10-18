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

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word(WordCount *words, int unique_count, const char *word) {
    for (int i = 0; i < unique_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void find_most_common_elements(const char *text, WordCount *result, int *result_count) {
    WordCount words[MAX_UNIQUE_WORDS] = {0};
    int unique_count = 0;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN];
    int current_len = 0;
    int max_count = 0;
    int result_idx = 0;

    *result_count = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalnum((unsigned char)text[i]) || text[i] == '_' || text[i] == '-') {
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = text[i];
            }
        } else {
            if (current_len > 0) {
                current_word[current_len] = '\0';
                to_lowercase(current_word);

                int idx = find_word(words, unique_count, current_word);
                if (idx >= 0) {
                    words[idx].count++;
                } else if (unique_count < MAX_UNIQUE_WORDS) {
                    /* Possible weaknesses found:
                     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                     */
                    strncpy(words[unique_count].word, current_word, MAX_WORD_LEN - 1);
                    words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[unique_count].count = 1;
                    unique_count++;
                }

                current_len = 0;
            }
        }
    }

    if (current_len > 0) {
        current_word[current_len] = '\0';
        to_lowercase(current_word);

        int idx = find_word(words, unique_count, current_word);
        if (idx >= 0) {
            words[idx].count++;
        } else if (unique_count < MAX_UNIQUE_WORDS) {
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(words[unique_count].word, current_word, MAX_WORD_LEN - 1);
            words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            words[unique_count].count = 1;
            unique_count++;
        }
    }

    for (int i = 0; i < unique_count; i++) {
        if (words[i].count > max_count) {
            max_count = words[i].count;
        }
    }

    for (int i = 0; i < unique_count && result_idx < MAX_UNIQUE_WORDS; i++) {
        if (words[i].count == max_count) {
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result[result_idx].word, words[i].word, MAX_WORD_LEN - 1);
            result[result_idx].word[MAX_WORD_LEN - 1] = '\0';
            result[result_idx].count = words[i].count;
            result_idx++;
        }
    }

    *result_count = result_idx;
}

int main(void) {
    const char *text = "The quick brown fox jumps over the lazy dog. The dog barks, and the fox runs away. Quick brown fox!";
    WordCount results[MAX_UNIQUE_WORDS] = {0};
    int result_count = 0;

    find_most_common_elements(text, results, &result_count);

    printf("Most common word(s):\n");
    for (int i = 0; i < result_count; i++) {
        printf("%s: %d\n", results[i].word, results[i].count);
    }

    return 0;
}