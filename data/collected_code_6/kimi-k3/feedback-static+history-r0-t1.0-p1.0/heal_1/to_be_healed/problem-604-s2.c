#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void reverse_substring(char *start, char *end) {
    /* Possible weaknesses found:
     *  The scope of the variable 'temp' can be reduced. [variableScope]
     */
    char temp;
    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverse_words(char *str) {
    char *word_start = NULL;
    char *current = str;
    
    if (str == NULL || *str == '\0') {
        return;
    }
    
    while (*current) {
        if (word_start == NULL && !isspace((unsigned char)*current)) {
            word_start = current;
        }
        if (word_start && (isspace((unsigned char)*(current + 1)) || *(current + 1) == '\0')) {
            reverse_substring(word_start, current);
            word_start = NULL;
        }
        current++;
    }
    
    reverse_substring(str, current - 1);
    
    current = str;
    char *write_ptr = str;
    int space_found = 0;
    
    while (*current && isspace((unsigned char)*current)) {
        current++;
    }
    
    while (*current) {
        if (isspace((unsigned char)*current)) {
            space_found = 1;
        } else {
            if (space_found && write_ptr != str) {
                *write_ptr++ = ' ';
            }
            space_found = 0;
            *write_ptr++ = *current;
        }
        current++;
    }
    *write_ptr = '\0';
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[1024];
    
    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    if (strlen(buffer) == 0) {
        printf("Empty string provided\n");
        return EXIT_SUCCESS;
    }
    
    printf("Original: \"%s\"\n", buffer);
    reverse_words(buffer);
    printf("Reversed: \"%s\"\n", buffer);
    
    return EXIT_SUCCESS;
}