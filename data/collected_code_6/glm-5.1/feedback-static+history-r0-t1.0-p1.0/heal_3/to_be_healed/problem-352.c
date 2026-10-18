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
    /* Possible weaknesses found:
     *  Assignment 'test3=NULL', assigned value is 0
     */
    const char *test3 = NULL;
    const char *test4 = "";

    printf("%s: %d\n", test1 ? test1 : "NULL", are_characters_unique(test1, test1 ? strnlen(test1, 256) : 0));
    printf("%s: %d\n", test2 ? test2 : "NULL", are_characters_unique(test2, test2 ? strnlen(test2, 256) : 0));
    /* Possible weaknesses found:
     *  Condition 'test3' is always false [knownConditionTrueFalse]
     *  Condition 'test3' is always false
     */
    printf("%s: %d\n", test3 ? test3 : "NULL", are_characters_unique(test3, test3 ? strnlen(test3, 256) : 0));
    printf("%s: %d\n", test4 ? test4 : "NULL", are_characters_unique(test4, test4 ? strnlen(test4, 256) : 0));

    return 0;
}