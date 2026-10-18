#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* find_first_max_even_word(const char* str, size_t* out_len) {
    if (!str || !out_len) {
        return NULL;
    }

    *out_len = 0;
    size_t current_len = 0;
    const char* word_start = str;
    size_t max_len = 0;
    const char* max_word_start = NULL;

    for (size_t i = 0; ; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r' || str[i] == '\0') {
            if (current_len > 0 && current_len % 2 == 0 && current_len > max_len) {
                max_len = current_len;
                max_word_start = word_start;
            }
            current_len = 0;
            if (str[i] == '\0') {
                break;
            }
        } else {
            if (current_len == 0) {
                word_start = &str[i];
            }
            current_len++;
        }
    }

    if (max_len > 0) {
        *out_len = max_len;
        char* result = malloc(max_len + 1);
        if (!result) {
            *out_len = 0;
            return NULL;
        }
        for (size_t i = 0; i < max_len; i++) {
            result[i] = max_word_start[i];
        }
        result[max_len] = '\0';
        return result;
    }

    return NULL;
}

int main(void) {
    const char* text = "This is an example string containing several even words";
    size_t len = 0;
    
    char* word = find_first_max_even_word(text, &len);
    
    if (word) {
        printf("First max even word: %s\nLength: %zu\n", word, len);
        free(word);
    } else {
        printf("No even length word found or memory allocation failed.\n");
    }

    return 0;
}