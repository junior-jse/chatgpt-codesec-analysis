#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* find_first_max_even_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    size_t start = 0;
    size_t max_len = 0;
    size_t max_start = 0;
    int found = 0;

    while (start <= len) {
        if (start < len && str[start] != ' ') {
            size_t end = start;
            while (end < len && str[end] != ' ') {
                end++;
            }
            size_t word_len = end - start;
            if (word_len % 2 == 0) {
                if (!found || word_len > max_len) {
                    max_len = word_len;
                    max_start = start;
                    found = 1;
                }
            }
            start = end;
        } else {
            start++;
        }
    }

    if (!found) {
        return NULL;
    }

    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, str + max_start, max_len);
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