#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_vowel(char c) {
    if (c < 'A' || c > 'z') return false;
    if (c > 'Z' && c < 'a') return false;
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') return true;
    if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') return true;
    return false;
}

void reverse_vowels(char *s, size_t len) {
    if (s == NULL || len == 0) return;
    
    size_t left = 0;
    size_t right = len - 1;
    
    while (left < right) {
        while (left < right && !is_vowel(s[left])) left++;
        while (left < right && !is_vowel(s[right])) right--;
        
        if (left < right) {
            char temp = s[left];
            s[left] = s[right];
            s[right] = temp;
            left++;
            right--;
        }
    }
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(argv[1]);
    char *input = malloc(len + 1);
    if (input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    snprintf(input, len + 1, "%s", argv[1]);
    
    reverse_vowels(input, len);
    printf("%s\n", input);
    
    free(input);
    input = NULL;
    
    return EXIT_SUCCESS;
}