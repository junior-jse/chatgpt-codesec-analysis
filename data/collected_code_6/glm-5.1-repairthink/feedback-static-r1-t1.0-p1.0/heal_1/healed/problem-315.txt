#include <stdio.h>
#include <stdlib.h>

char* find_first_max_even_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    const char* p = str;
    const char* max_start = NULL;
    size_t max_len = 0;

    while (*p) {
        while (*p == ' ') {
            p++;
        }
        
        if (*p == '\0') {
            break;
        }

        const char* word_start = p;
        while (*p && *p != ' ') {
            p++;
        }

        size_t word_len = p - word_start;
        if (word_len % 2 == 0) {
            if (max_start == NULL || word_len > max_len) {
                max_len = word_len;
                max_start = word_start;
            }
        }
    }

    if (max_start == NULL) {
        return NULL;
    }

    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < max_len; i++) {
        result[i] = max_start[i];
    }
    result[max_len] = '\0';

    return result;
}

int main() {
    const char* input = "This is a test string with some evenandlonger words";
    char* word = find_first_max_even_word(input);

    if (word != NULL) {
        printf("%s\n", word);
        free(word);
    } else {
        printf("No even length word found\n");
    }

    return 0;
}