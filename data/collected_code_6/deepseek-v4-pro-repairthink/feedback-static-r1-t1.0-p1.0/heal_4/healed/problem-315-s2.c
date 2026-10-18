#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int first_max_even_word(const char *str, char *result, size_t result_size) {
    if (str == NULL || result == NULL || result_size == 0) {
        return -1;
    }

    size_t max_len = 0;
    size_t max_start = 0;
    int found = 0;
    size_t i = 0;
    size_t cur_start = 0;

    while (1) {
        unsigned char ch = (unsigned char)str[i];
        if (ch == '\0') {
            size_t word_len = i - cur_start;
            if (word_len > 0 && word_len % 2 == 0) {
                if (!found || word_len > max_len) {
                    max_len = word_len;
                    max_start = cur_start;
                    found = 1;
                }
            }
            break;
        }
        if (isspace(ch)) {
            size_t word_len = i - cur_start;
            if (word_len > 0 && word_len % 2 == 0) {
                if (!found || word_len > max_len) {
                    max_len = word_len;
                    max_start = cur_start;
                    found = 1;
                }
            }
            cur_start = i + 1;
        }
        i++;
    }

    if (!found) {
        result[0] = '\0';
        return 0;
    }

    size_t copy_len = (max_len < result_size - 1) ? max_len : result_size - 1;
    memcpy(result, str + max_start, copy_len);
    result[copy_len] = '\0';

    return 1;
}

int main(void) {
    const char *text = "The quick brown fox jumps over the lazy dog";
    size_t len = strlen(text);
    char *word = malloc(len + 1);
    if (word == NULL) {
        return 1;
    }

    int ret = first_max_even_word(text, word, len + 1);

    if (ret == 1) {
        printf("First max even word: %s\n", word);
    } else if (ret == 0) {
        printf("No even word found.\n");
    } else {
        printf("Invalid input.\n");
    }

    free(word);
    return 0;
}