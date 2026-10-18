#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool are_characters_unique(const char *str, size_t len) {
    if (str == NULL) {
        return true;
    }

    if (len > 256) {
        return false;
    }

    bool *char_set = (bool *)calloc(256, sizeof(bool));
    if (char_set == NULL) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (char_set[c]) {
            free(char_set);
            return false;
        }
        char_set[c] = true;
    }

    free(char_set);
    return true;
}

int main(void) {
    const char *test1 = "abcdef";
    const char *test2 = "hello";
    const char *test4 = "";

    printf("%s: %d\n", test1, are_characters_unique(test1, strnlen(test1, 256)));
    printf("%s: %d\n", test2, are_characters_unique(test2, strnlen(test2, 256)));
    printf("NULL: %d\n", are_characters_unique(NULL, 0));
    printf("%s: %d\n", test4, are_characters_unique(test4, strnlen(test4, 256)));

    return 0;
}