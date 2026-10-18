/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_with_length(const char* str, int k) {
    if (str == NULL || k <= 0) {
        return NULL;
    }

    size_t len = 0;
    /* Possible weaknesses found:
     *  Either the condition 'len>1000000' is redundant or the array 'str[34]' is accessed at index 1000000, which is out of bounds. [arrayIndexOutOfBoundsCond]
     *  Array index out of bounds
     */
    while (str[len] != '\0') {
        len++;
        /* Possible weaknesses found:
         *  Assuming that condition 'len>1000000' is not redundant
         */
        if (len > 1000000) {
            return NULL;
        }
    }

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    int word_length = 0;
    int word_start = -1;

    while (i <= len) {
        if (i < len && isalnum((unsigned char)str[i])) {
            if (word_start == -1) {
                word_start = i;
            }
            word_length++;
        } else {
            if (word_start != -1) {
                if (word_length != k) {
                    if (j > 0 && result[j-1] != ' ') {
                        if (j < len) {
                            result[j++] = ' ';
                        }
                    }
                    if (j + word_length < len + 1) {
                        size_t copy_len = word_length;
                        if (copy_len > len - j + 1) {
                            copy_len = len - j + 1;
                        }
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(result + j, str + word_start, copy_len);
                        j += copy_len;
                        result[j] = '\0';
                    }
                }
                word_start = -1;
                word_length = 0;
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