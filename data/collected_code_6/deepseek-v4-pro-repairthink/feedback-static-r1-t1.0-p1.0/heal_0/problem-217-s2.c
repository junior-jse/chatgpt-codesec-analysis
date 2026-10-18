#include <stdio.h>
#include <string.h>

#define ASCII_SIZE 256

char first_repeated_character(const char *str) {
    if (str == NULL) {
        return '\0';
    }
    
    int count[ASCII_SIZE] = {0};
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        count[(unsigned char)str[i]]++;
    }
    
    for (size_t i = 0; i < len; i++) {
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
        char result = first_repeated_character(test_strings[i]);
        if (result != '\0') {
            printf("First repeated character in \"%s\": %c\n", test_strings[i], result);
        } else {
            printf("No repeated character in \"%s\"\n", test_strings[i]);
        }
    }
    
    return 0;
}