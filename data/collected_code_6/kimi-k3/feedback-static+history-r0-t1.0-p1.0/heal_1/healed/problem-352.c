#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool has_all_unique_chars(const char *str, size_t max_len) {
    if (str == NULL) {
        return false;
    }
    
    bool seen[256] = {false};
    size_t len = strnlen(str, max_len);
    
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
    const char *test4 = "aAbBcC";
    
    printf("\"%s\": %s\n", test1, has_all_unique_chars(test1, 256) ? "true" : "false");
    printf("\"%s\": %s\n", test2, has_all_unique_chars(test2, 256) ? "true" : "false");
    printf("\"%s\": %s\n", test3, has_all_unique_chars(test3, 256) ? "true" : "false");
    printf("\"%s\": %s\n", test4, has_all_unique_chars(test4, 256) ? "true" : "false");
    
    return 0;
}