#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char find_first_repeated(const char *str)
{
    if (str == NULL) {
        return '\0';
    }
    
    bool seen[256] = {false};
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (seen[c]) {
            return str[i];
        }
        seen[c] = true;
    }
    
    return '\0';
}

int main(void)
{
    const char *test_strings[] = {
        "hello",
        "programming",
        "abcdef",
        "aabbcc",
        ""
    };
    
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        char result = find_first_repeated(test_strings[i]);
        
        if (result != '\0') {
            printf("String: \"%s\" -> First repeated character: '%c'\n", 
                   test_strings[i], result);
        } else {
            printf("String: \"%s\" -> No repeated character found\n", 
                   test_strings[i]);
        }
    }
    
    return 0;
}