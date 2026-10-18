#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_STRING_LENGTH 8192

bool all_unique_chars(const char *str) {
    if (str == NULL) {
        return false;
    }

    unsigned char seen[256] = {0};

    for (size_t i = 0; i < MAX_STRING_LENGTH; i++) {
        if (str[i] == '\0') {
            return true;
        }
        unsigned char ch = (unsigned char)str[i];
        if (ch >= sizeof(seen)) {
            return false;
        }
        if (seen[ch]) {
            return false;
        }
        seen[ch] = 1;
    }

    return false;
}

int main(void) {
    const char *test1 = "abcdef";
    const char *test2 = "hello";
    const char *test3 = "";
    const char *test4 = "aA";

    printf("Test 1: %s\n", all_unique_chars(test1) ? "true" : "false");
    printf("Test 2: %s\n", all_unique_chars(test2) ? "true" : "false");
    printf("Test 3: %s\n", all_unique_chars(test3) ? "true" : "false");
    printf("Test 4: %s\n", all_unique_chars(test4) ? "true" : "false");

    return 0;
}