#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void reverse_words(char *str) {
    if (str == NULL) {
        return;
    }

    size_t len = strnlen(str, SIZE_MAX);
    if (len == 0) {
        return;
    }
    if (len >= SIZE_MAX) {
        return;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return;
    }

    size_t word_start = len;
    size_t word_end = len;
    size_t result_index = 0;

    for (size_t i = len; i > 0; i--) {
        size_t idx = i - 1;
        if (str[idx] == ' ') {
            if (word_start > i) {
                if (result_index > 0) {
                    if (result_index >= len) {
                        free(result);
                        return;
                    }
                    result[result_index++] = ' ';
                }
                size_t words_to_copy = word_start - i;
                if (result_index + words_to_copy > len) {
                    free(result);
                    return;
                }
                memcpy(result + result_index, str + i, words_to_copy);
                result_index += words_to_copy;
            }
            word_start = idx;
            word_end = idx;
        } else {
            if (word_end == len) {
                word_start = idx;
                word_end = idx + 1;
            } else {
                word_start = idx;
            }
        }
    }

    if (word_start < word_end) {
        if (result_index > 0) {
            if (result_index >= len) {
                free(result);
                return;
            }
            result[result_index++] = ' ';
        }
        size_t words_to_copy = word_end - word_start;
        if (result_index + words_to_copy > len) {
            free(result);
            return;
        }
        memcpy(result + result_index, str + word_start, words_to_copy);
        result_index += words_to_copy;
    }

    if (result_index <= len) {
        result[result_index] = '\0';
        memmove(str, result, result_index + 1);
    }
    free(result);
}

int main(void) {
    char str1[] = "Hello World";
    reverse_words(str1);
    printf("%s\n", str1);

    char str2[] = "This is a test";
    reverse_words(str2);
    printf("%s\n", str2);

    char str3[] = "Single";
    reverse_words(str3);
    printf("%s\n", str3);

    char str4[] = "";
    reverse_words(str4);
    printf("%s\n", str4);

    char str5[] = "   Multiple   spaces   ";
    reverse_words(str5);
    printf("%s\n", str5);

    return 0;
}