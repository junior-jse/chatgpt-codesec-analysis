#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* find_first_max_even_word(const char* str, size_t* out_len) {
    if (!str || !out_len) {
        return NULL;
    }

    *out_len = 0;
    char* result = NULL;
    size_t current_len = 0;
    const char* word_start = str;

    for (size_t i = 0; ; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r' || str[i] == '\0') {
            if (current_len > 0) {
                if (current_len % 2 == 0 && current_len > *out_len) {
                    *out_len = current_len;
                    free(result);
                    result = malloc(current_len + 1);
                    if (!result) {
                        *out_len = 0;
                        return NULL;
                    }
                    memcpy(result, word_start, current_len);
                    result[current_len] = '\0';
                }
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

    return result;
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