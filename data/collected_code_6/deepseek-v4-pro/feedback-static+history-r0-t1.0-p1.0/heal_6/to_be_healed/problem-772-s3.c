#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_with_length(const char* str, int k) {
    if (str == NULL || k <= 0) {
        return NULL;
    }

    size_t len = strnlen(str, 4096);
    size_t word_count = 0;
    size_t in_word = 0;
    /* Possible weaknesses found:
     *  Variable 'total_chars' is assigned a value that is never used. [unreadVariable]
     */
    size_t total_chars = 0;

    for (size_t i = 0; i < len; i++) {
        int is_word_char = (isalnum((unsigned char)str[i]) || str[i] == '\'' || str[i] == '-');
        if (is_word_char && !in_word) {
            in_word = 1;
            word_count++;
        }
        if (!is_word_char) {
            in_word = 0;
        }
    }

    size_t max_spaces = (word_count > 0) ? word_count - 1 : 0;
    size_t max_result_size = len + max_spaces + 1;

    char* result = (char*)malloc(max_result_size);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    size_t word_length = 0;
    size_t word_start = 0;
    in_word = 0;

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
                        if (j + 1 < max_result_size) {
                            result[j++] = ' ';
                        } else {
                            free(result);
                            return NULL;
                        }
                    }
                    if (j + word_length < max_result_size) {
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(result + j, str + word_start, word_length);
                        j += word_length;
                    } else {
                        free(result);
                        return NULL;
                    }
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

    char* final_result = (char*)realloc(result, j + 1);
    if (final_result != NULL) {
        result = final_result;
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