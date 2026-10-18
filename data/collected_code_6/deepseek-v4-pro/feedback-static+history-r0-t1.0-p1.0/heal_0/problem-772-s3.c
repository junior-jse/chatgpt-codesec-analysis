#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_with_length(const char* str, int k) {
    if (str == NULL || k <= 0) {
        return NULL;
    }

    size_t len = strlen(str);
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    int word_length = 0;
    int word_start = -1;

    while (i <= len) {
        if (isalnum((unsigned char)str[i])) {
            if (word_start == -1) {
                word_start = i;
            }
            word_length++;
        } else {
            if (word_start != -1) {
                if (word_length != k) {
                    if (j > 0 && result[j-1] != ' ') {
                        result[j++] = ' ';
                    }
                    strncpy(result + j, str + word_start, word_length);
                    j += word_length;
                    result[j] = '\0';
                }
                word_start = -1;
                word_length = 0;
            }
        }
        i++;
    }

    if (j > 0 && result[j-1] == ' ') {
        result[j-1] = '\0';
    } else {
        result[j] = '\0';
    }

    return result;
}

int main(void) {
    const char* input = "hello world this is a test string";
    int k = 2;
    
    char* result = remove_words_with_length(input, k);
    if (result != NULL) {
        printf("Original: %s\n", input);
        printf("After removing words of length %d: %s\n", k, result);
        free(result);
    }

    return 0;
}