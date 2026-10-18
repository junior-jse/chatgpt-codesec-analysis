#include <stdio.h>
#include <stdbool.h>

bool are_chars_unique(const char *str) {
    if (str == NULL) {
        return false;
    }

    bool char_set[256] = {false};

    while (*str != '\0') {
        unsigned char c = (unsigned char)*str;
        if (char_set[c]) {
            return false;
        }
        char_set[c] = true;
        str++;
    }

    return true;
}

int main(void) {
    const char *test1 = "abcdef";
    const char *test2 = "hello";
    const char *test3 = NULL;
    const char *test4 = "";

    printf("%d\n", are_chars_unique(test1));
    printf("%d\n", are_chars_unique(test2));
    printf("%d\n", are_chars_unique(test3));
    printf("%d\n", are_chars_unique(test4));

    return 0;
}