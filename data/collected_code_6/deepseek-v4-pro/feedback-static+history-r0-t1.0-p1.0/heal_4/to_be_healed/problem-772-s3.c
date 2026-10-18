#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_with_length(const char* str, int k) {
    if (str == NULL || k <= 0) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    size_t word_length = 0;
    size_t word_start = 0;
    int in_word = 0;

    while (i <= len) {
        if (i < len && (isalnum((unsigned char)str[i]) || str[i] == '\'' || str[i] == '-')) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
            word_length++;
        } else {
            if (in_word) {
                if (word_length != (size_t)k) {
                    if (j > 0) {
                        result[j++] = ' ';
                    }
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result + j, str + word_start, word_length);
                    j += word_length;
                }
                word_length = 0;
                in_word = 0;
            }
        }
        i++;
    }

    if (j > 0 && result[j-1] == ' ') {
        j--;
    }
    result[j] = '\0';

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