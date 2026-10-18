#include <stdio.h>
#include <stdbool.h>

bool are_characters_unique(const char *str) {
    if (str == NULL) {
        return false;
    }

    bool char_set[256] = {false};

    for (size_t i = 0; i <= 256; i++) {
        /* Possible weaknesses found:
         *  Array index out of bounds
         *  Either the condition 'i==256' is redundant or the array 'str[1]' is accessed at index 256, which is out of bounds. [arrayIndexOutOfBoundsCond]
         */
        if (str[i] == '\0') {
            return true;
        }
        /* Possible weaknesses found:
         *  Assuming that condition 'i==256' is not redundant
         */
        if (i == 256) {
            return false;
        }
        unsigned char c = (unsigned char)str[i];
        if (char_set[c]) {
            return false;
        }
        char_set[c] = true;
    }

    return false;
}

int main(void) {
    const char *test1 = "abcdef";
    const char *test2 = "hello";
    const char *test3 = "";
    const char *test4 = NULL;

    printf("%d\n", are_characters_unique(test1));
    printf("%d\n", are_characters_unique(test2));
    printf("%d\n", are_characters_unique(test3));
    printf("%d\n", are_characters_unique(test4));

    return 0;
}