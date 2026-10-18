#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_k_length_words(const char* str, size_t max_len, int k) {
    if (str == NULL || k <= 0) {
        char* empty = (char*)malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t res_idx = 0;
    size_t i = 0;

    while (i < max_len && str[i] != '\0') {
        if (isalpha((unsigned char)str[i])) {
            size_t word_start = i;
            size_t word_len = 0;

            while (i < max_len && str[i] != '\0' && isalpha((unsigned char)str[i])) {
                word_len++;
                i++;
            }

            if (word_len != (size_t)k) {
                for (size_t j = word_start; j < word_start + word_len; j++) {
                    result[res_idx++] = str[j];
                }
            }
        } else {
            result[res_idx++] = str[i++];
        }
    }

    result[res_idx] = '\0';

    char* resized = (char*)realloc(result, res_idx + 1);
    if (resized != NULL) {
        result = resized;
    }

    return result;
}

int main() {
    const char* input = "This is a test string for removing words of certain length";
    int k = 4;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    
    char* output = remove_k_length_words(input, input_len, k);
    if (output != NULL) {
        printf("Original: %s\n", input);
        printf("Modified: %s\n", output);
        free(output);
    }

    return 0;
}