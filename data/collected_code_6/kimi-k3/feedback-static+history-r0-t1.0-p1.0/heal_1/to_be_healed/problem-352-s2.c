#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool areCharactersUnique(const char *str) {
    if (str == NULL) {
        return false;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len > 256) {
        return false;
    }
    
    bool seen[256] = {false};
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (seen[c]) {
            return false;
        }
        seen[c] = true;
    }
    
    return true;
}

int main(void) {
    const char *test1 = "abcdefg";
    const char *test2 = "hello";
    const char *test3 = "";
    
    printf("Test 1: %s\n", areCharactersUnique(test1) ? "true" : "false");
    printf("Test 2: %s\n", areCharactersUnique(test2) ? "true" : "false");
    printf("Test 3: %s\n", areCharactersUnique(test3) ? "true" : "false");
    
    return 0;
}