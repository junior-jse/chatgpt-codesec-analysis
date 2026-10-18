#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_STRING_LENGTH 8192

bool all_unique_chars(const char *str) {
    if (str == NULL) {
        return false;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    unsigned char seen[256] = {0};

    /* Possible weaknesses found:
     *  Assuming that condition 'i<8192' is not redundant
     */
    for (size_t i = 0; i < MAX_STRING_LENGTH; i++) {
        /* Possible weaknesses found:
         *  Array index out of bounds
         *  Array 'str[1]' accessed at index 8191, which is out of bounds. [arrayIndexOutOfBounds]
         */
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