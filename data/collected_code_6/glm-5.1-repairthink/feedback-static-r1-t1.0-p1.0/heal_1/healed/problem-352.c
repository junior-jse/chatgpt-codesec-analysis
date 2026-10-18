#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool are_characters_unique(const char *str) {
    if (str == NULL) {
        return true;
    }

    bool *char_set = (bool *)calloc(256, sizeof(bool));
    if (char_set == NULL) {
        return false;
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (i >= 256) {
            free(char_set);
            return false;
        }
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

    printf("%s: %d\n", test1 ? test1 : "NULL", are_characters_unique(test1));
    printf("%s: %d\n", test2 ? test2 : "NULL", are_characters_unique(test2));
    printf("%s: %d\n", "NULL", are_characters_unique(NULL));
    printf("%s: %d\n", test4 ? test4 : "NULL", are_characters_unique(test4));

    return 0;
}