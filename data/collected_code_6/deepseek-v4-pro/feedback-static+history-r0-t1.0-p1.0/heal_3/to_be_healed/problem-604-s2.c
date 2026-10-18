#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void reverse_words(char *str) {
    if (str == NULL) {
        return;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len > SIZE_MAX - 1) {
        return;
    }
    
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return;
    }

    int word_start = (int)len;
    int word_end = (int)len;
    int result_index = 0;

    for (int i = (int)len - 1; i >= 0; i--) {
        if (str[i] == ' ') {
            if (word_start > i + 1) {
                if (result_index > 0) {
                    if ((size_t)result_index >= len) {
                        free(result);
                        return;
                    }
                    result[result_index++] = ' ';
                }
                for (int j = i + 1; j < word_start; j++) {
                    if ((size_t)result_index >= len) {
                        free(result);
                        return;
                    }
                    result[result_index++] = str[j];
                }
            }
            word_start = i;
            word_end = i;
        } else {
            if (word_end == (int)len) {
                word_start = i;
                word_end = i + 1;
            } else {
                word_start = i;
            }
        }
    }

    if (word_start < word_end) {
        if (result_index > 0) {
            if ((size_t)result_index >= len) {
                free(result);
                return;
            }
            result[result_index++] = ' ';
        }
        for (int j = word_start; j < word_end; j++) {
            if ((size_t)result_index >= len) {
                free(result);
                return;
            }
            result[result_index++] = str[j];
        }
    }

    result[result_index] = '\0';
    
    if ((size_t)(result_index + 1) > len + 1) {
        free(result);
        return;
    }
    
    memmove(str, result, (size_t)result_index + 1);
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