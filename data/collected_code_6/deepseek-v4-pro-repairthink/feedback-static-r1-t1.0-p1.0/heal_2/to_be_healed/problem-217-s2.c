#include <stdio.h>
#include <string.h>

#define ASCII_SIZE 256

char first_repeated_character(const char *str, size_t length) {
    if (str == NULL) {
        return '\0';
    }
    
    int count[ASCII_SIZE] = {0};
    size_t i;
    
    for (i = 0; i < length && str[i] != '\0'; i++) {
        count[(unsigned char)str[i]]++;
    }
    
    for (i = 0; i < length && str[i] != '\0'; i++) {
        if (count[(unsigned char)str[i]] > 1) {
            return str[i];
        }
    }
    
    return '\0';
}

int main(void) {
    const char *test_strings[] = {
        "hello",
        "programming",
        "abcdef",
        "",
        "aabbcc",
        NULL
    };
    
    for (int i = 0; test_strings[i] != NULL; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        char result = first_repeated_character(test_strings[i], strlen(test_strings[i]));
        if (result != '\0') {
            printf("First repeated character in \"%s\": %c\n", test_strings[i], result);
        } else {
            printf("No repeated character in \"%s\"\n", test_strings[i]);
        }
    }
    
    return 0;
}