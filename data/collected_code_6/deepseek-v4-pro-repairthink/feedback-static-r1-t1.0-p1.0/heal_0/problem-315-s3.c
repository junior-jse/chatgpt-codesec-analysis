#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_BUFFER_SIZE 256

char *find_first_max_even_word(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    size_t pos = 0;
    size_t max_len = 0;
    char *max_word = NULL;
    size_t max_word_size = 0;

    while (pos <= len) {
        while (pos < len && !isalpha((unsigned char)str[pos])) {
            pos++;
        }

        if (pos >= len) {
            break;
        }

        size_t word_start = pos;
        while (pos < len && isalpha((unsigned char)str[pos])) {
            pos++;
        }
        size_t word_len = pos - word_start;

        if (word_len % 2 == 0 && word_len > 0 && word_len > max_len) {
            max_len = word_len;
            size_t needed = word_len + 1;
            if (needed > max_word_size) {
                char *new_word = realloc(max_word, needed);
                if (new_word == NULL) {
                    free(max_word);
                    return NULL;
                }
                max_word = new_word;
                max_word_size = needed;
            }
            memcpy(max_word, str + word_start, word_len);
            max_word[word_len] = '\0';
        }
    }

    return max_word;
}

int main(void) {
    const char *test_string = "This is a test string with several words of different lengths";
    char *result = find_first_max_even_word(test_string);

    if (result != NULL) {
        printf("First max even word: %s\n", result);
        free(result);
    } else {
        printf("No even word found or memory allocation failed.\n");
    }

    return 0;
}