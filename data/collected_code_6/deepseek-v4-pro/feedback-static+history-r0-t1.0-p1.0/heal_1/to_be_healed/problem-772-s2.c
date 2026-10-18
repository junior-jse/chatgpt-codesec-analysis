#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_with_length(const char* input, int k) {
    if (input == NULL || k <= 0) {
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    size_t j = 0;
    int word_start = -1;
    int word_length = 0;
    
    while (i <= len) {
        if (i == len || isspace((unsigned char)input[i])) {
            if (word_start != -1) {
                if (word_length != k) {
                    if (j > 0) {
                        result[j++] = ' ';
                    }
                    /* Possible weaknesses found:
                     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                     */
                    strncpy(result + j, input + word_start, word_length);
                    j += word_length;
                }
                word_start = -1;
                word_length = 0;
            }
            if (i < len && j > 0 && result[j-1] != ' ') {
                result[j++] = ' ';
            }
        } else {
            if (word_start == -1) {
                word_start = i;
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

int main() {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[1000];
    int k;
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        return 1;
    }
    
    char* result = remove_words_with_length(input, k);
    if (result == NULL) {
        printf("Error processing string\n");
        return 1;
    }
    
    printf("Result: %s\n", result);
    
    free(result);
    return 0;
}