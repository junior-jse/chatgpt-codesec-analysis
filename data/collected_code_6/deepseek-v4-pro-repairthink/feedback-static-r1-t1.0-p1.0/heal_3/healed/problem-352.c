#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool all_unique_chars(const char *str) {
    if (str == NULL) {
        return false;
    }

    bool *seen = calloc(256, sizeof(bool));
    if (seen == NULL) {
        return false;
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)str[i];
        if (ch >= 256) {
            free(seen);
            return false;
        }
        if (seen[ch]) {
            free(seen);
            return false;
        }
        seen[ch] = true;
    }

    free(seen);
    return true;
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