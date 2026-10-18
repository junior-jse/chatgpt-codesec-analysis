#include <stdio.h>
#include <string.h>
#include <limits.h>

char max_occurring_char(const char *str) {
    int count[UCHAR_MAX + 1] = {0};
    int max_count = 0;
    char max_char = '\0';
    
    if (str == NULL || *str == '\0') {
        return '\0';
    }
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)str[i];
        count[ch]++;
        
        if (count[ch] > max_count) {
            max_count = count[ch];
            max_char = str[i];
        }
    }
    
    return max_char;
}

int main(void) {
    const char *test_strings[] = {
        "hello world",
        "programming",
        "aaaaabbbbbccccc",
        "",
        "a",
        "test string"
    };
    
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        char result = max_occurring_char(test_strings[i]);
        if (result != '\0') {
            printf("String: \"%s\" -> Max occurring character: '%c'\n", 
                   test_strings[i], result);
        } else {
            printf("String: \"%s\" -> Empty or invalid string\n", 
                   test_strings[i]);
        }
    }
    
    return 0;
}