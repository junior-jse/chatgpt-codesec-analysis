#include <stdio.h>
#include <string.h>
#include <limits.h>

char find_max_occurring_char(const char *str) {
    if (str == NULL || *str == '\0') {
        return '\0';
    }
    
    int count[UCHAR_MAX + 1] = {0};
    int max_count = 0;
    char max_char = str[0];
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        count[c]++;
        
        if (count[c] > max_count || 
            /* Possible weaknesses found:
             *  Array index 'c' is used before limits check. [arrayIndexThenCheck]
             */
            (count[c] == max_count && c < (unsigned char)max_char)) {
            max_count = count[c];
            max_char = (char)c;
        }
    }
    
    return max_char;
}

int main(void) {
    const char *test_strings[] = {
        "hello world",
        "programming",
        "aabbcc",
        "test string",
        "",
        NULL
    };
    
    for (int i = 0; i < 6; i++) {
        if (test_strings[i] == NULL) {
            printf("NULL string: no max char\n");
            continue;
        }
        
        char result = find_max_occurring_char(test_strings[i]);
        if (result == '\0') {
            printf("\"%s\": empty string, no max char\n", test_strings[i]);
        } else {
            printf("\"%s\": max char is '%c'\n", test_strings[i], result);
        }
    }
    
    return 0;
}