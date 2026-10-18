#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void reverse_substring(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverse_words(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    
    char *word_start = NULL;
    char *current = str;
    
    while (*current != '\0') {
        if (word_start == NULL && !isspace((unsigned char)*current)) {
            word_start = current;
        }
        if (word_start != NULL && (isspace((unsigned char)*(current + 1)) || *(current + 1) == '\0')) {
            reverse_substring(word_start, current);
            word_start = NULL;
        }
        current++;
    }
    
    reverse_substring(str, current - 1);
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    reverse_words(buffer);
    
    if (printf("%s\n", buffer) < 0) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}