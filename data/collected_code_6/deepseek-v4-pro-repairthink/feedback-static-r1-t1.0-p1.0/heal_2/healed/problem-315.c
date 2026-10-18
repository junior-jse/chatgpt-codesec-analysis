#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* first_max_even_word(const char* str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    const char* best_start = NULL;
    size_t max_len = 0;
    const char* word_start = NULL;
    size_t current_len = 0;
    const char* p = str;

    while (*p != '\0') {
        if (isalpha((unsigned char)*p)) {
            if (word_start == NULL) {
                word_start = p;
                current_len = 0;
            }
            current_len++;
        } else {
            if (word_start != NULL && current_len % 2 == 0 && current_len > max_len) {
                best_start = word_start;
                max_len = current_len;
            }
            word_start = NULL;
            current_len = 0;
        }
        p++;
    }

    if (word_start != NULL && current_len % 2 == 0 && current_len > max_len) {
        best_start = word_start;
        max_len = current_len;
    }

    if (max_len > 0) {
        char* result = (char*)malloc(max_len + 1);
        if (result != NULL) {
            size_t i;
            for (i = 0; i < max_len; i++) {
                result[i] = best_start[i];
            }
            result[i] = '\0';
        }
        return result;
    }

    return NULL;
}

int main(void) {
    const char* text = "This is a sample sentence with words of varied lengths";
    char* word = first_max_even_word(text);

    if (word != NULL) {
        printf("First maximum length even word: %s\n", word);
        free(word);
    } else {
        printf("No even length word found.\n");
    }

    return 0;
}