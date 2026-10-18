#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* abbreviate_road(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    char* result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    size_t j = 0;
    
    while (i < len) {
        if (i + 3 < len && 
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1]) == 'o' &&
            tolower((unsigned char)input[i + 2]) == 'a' &&
            tolower((unsigned char)input[i + 3]) == 'd' &&
            (i == 0 || isspace((unsigned char)input[i - 1]) || ispunct((unsigned char)input[i - 1])) &&
            (i + 4 >= len || isspace((unsigned char)input[i + 4]) || ispunct((unsigned char)input[i + 4]))) {
            
            result[j++] = 'r';
            result[j++] = 'd';
            result[j++] = '.';
            i += 4;
        } else {
            result[j++] = input[i++];
        }
    }
    
    result[j] = '\0';
    
    char* final_result = realloc(result, j + 1);
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
    char input[1024];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    char* abbreviated = abbreviate_road(input);
    if (abbreviated == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Abbreviated: %s\n", abbreviated);
    
    free(abbreviated);
    
    return 0;
}