#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool are_characters_unique(const char *str) {
    if (str == NULL) {
        return false;
    }

    size_t len = strlen(str);
    if (len > 256) {
        return false;
    }

    bool char_set[256] = {false};

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (char_set[c]) {
            return false;
        }
        char_set[c] = true;
    }

    return true;
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