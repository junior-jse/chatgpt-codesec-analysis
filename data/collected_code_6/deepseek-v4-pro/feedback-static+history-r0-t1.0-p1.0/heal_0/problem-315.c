#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* first_max_even_word(const char* str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    static char result[256];
    const char* word_start = NULL;
    size_t max_len = 0;
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
                max_len = current_len;
                size_t copy_len = current_len < 255 ? current_len : 255;
                memcpy(result, word_start, copy_len);
                result[copy_len] = '\0';
            }
            word_start = NULL;
            current_len = 0;
        }
        p++;
    }

    if (word_start != NULL && current_len % 2 == 0 && current_len > max_len) {
        size_t copy_len = current_len < 255 ? current_len : 255;
        memcpy(result, word_start, copy_len);
        result[copy_len] = '\0';
        return result;
    }

    if (max_len > 0) {
        return result;
    }

    return NULL;
}

int main(void) {
    const char* text = "This is a sample sentence with words of varied lengths";
    char* word = first_max_even_word(text);

    if (word != NULL) {
        printf("First maximum length even word: %s\n", word);
    } else {
        printf("No even length word found.\n");
    }

    return 0;
}