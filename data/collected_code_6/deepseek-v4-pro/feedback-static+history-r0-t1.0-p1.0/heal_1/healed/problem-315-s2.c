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
    size_t current_start = 0;
    int found = 0;
    size_t i = 0;

    while (1) {
        if (str[i] == '\0' || isspace((unsigned char)str[i])) {
            size_t word_len = i - current_start;
            if (word_len > 0 && word_len % 2 == 0) {
                if (!found || word_len > max_len) {
                    max_len = word_len;
                    max_start = current_start;
                    found = 1;
                }
            }
            if (str[i] == '\0') {
                break;
            }
            current_start = i + 1;
        }
        i++;
    }

    if (!found) {
        result[0] = '\0';
        return 0;
    }

    if (max_len >= result_size) {
        max_len = result_size - 1;
    }

    if (max_start + max_len > (size_t)(str + (i + 1) - str)) {
        max_len = (size_t)(str + i - str) - max_start;
        if (max_len >= result_size) {
            max_len = result_size - 1;
        }
    }

    size_t copy_len = max_len;
    if (copy_len >= result_size) {
        copy_len = result_size - 1;
    }

    size_t j;
    for (j = 0; j < copy_len; j++) {
        result[j] = str[max_start + j];
    }
    result[copy_len] = '\0';

    return 1;
}

int main(void) {
    const char *text = "The quick brown fox jumps over the lazy dog";
    char word[100];

    int ret = first_max_even_word(text, word, sizeof(word));

    if (ret == 1) {
        printf("First max even word: %s\n", word);
    } else if (ret == 0) {
        printf("No even word found.\n");
    } else {
        printf("Invalid input.\n");
    }

    return 0;
}