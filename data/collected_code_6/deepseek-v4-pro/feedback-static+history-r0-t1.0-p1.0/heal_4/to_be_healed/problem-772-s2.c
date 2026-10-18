#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1000000
#define MAX_LINE_LENGTH 1000

char* remove_words_with_length(const char* input, int k) {
    if (input == NULL || k <= 0) {
        return NULL;
    }
    
    size_t len = strnlen(input, MAX_INPUT_LENGTH);
    if (len == MAX_INPUT_LENGTH) {
        return NULL;
    }
    
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    size_t j = 0;
    size_t word_start = 0;
    size_t word_length = 0;
    int in_word = 0;
    
    while (i <= len) {
        if (i == len || isspace((unsigned char)input[i])) {
            if (in_word) {
                if (word_length != (size_t)k) {
                    if (j > 0) {
                        if (j < len) {
                            result[j++] = ' ';
                        } else {
                            free(result);
                            return NULL;
                        }
                    }
                    if (j + word_length <= len) {
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(result + j, input + word_start, word_length);
                        j += word_length;
                    } else {
                        free(result);
                        return NULL;
                    }
                }
                in_word = 0;
                word_length = 0;
            }
        } else {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
            word_length++;
        }
        i++;
    }
    
    while (j > 0 && result[j-1] == ' ') {
        j--;
    }
    result[j] = '\0';
    
    char* final_result = (char*)realloc(result, j + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    
    return final_result;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[MAX_LINE_LENGTH];
    int k;
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        return 1;
    }
    
    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != '\n' && c != EOF);
    
    char* result = remove_words_with_length(input, k);
    if (result == NULL) {
        printf("Error processing string\n");
        return 1;
    }
    
    printf("Result: %s\n", result);
    
    free(result);
    return 0;
}